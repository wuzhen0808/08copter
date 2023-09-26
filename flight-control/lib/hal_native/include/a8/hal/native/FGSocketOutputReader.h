#pragma once
#include "a8/core/defines.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/ConsoleWriter.h"
#include "a8/hal/native/FileWriter.h"
#include "a8/util/Component.h"
#include "a8/util/Math.h"
#include "a8/util/Rate.h"
#include "a8/util/Result.h"
#include "a8/util/Runnable.h"
#include "a8/util/Sockets.h"
#include "a8/util/String.h"
#include "a8/util/Vector3f.h"
#include "input_output/net_fdm.hxx"
#include "winsock.h"

using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {
class FGSocketOuputReader : public Component {

private:
    Sockets *sockets;
    SOCK server;
    SOCK sockIn;
    FGNetFDM lastFGNetData;
    uint32_t cur_us;
    Writer *dataFileWriter;

    static void byteSwap(char *buffer, int len) {
        uint32_t *buf = (uint32_t *)buffer;
        for (int i = 0; i < len / 4; i++) {
            buf[i] = ntohl(buf[i]);
        }
        // fixup the 3 doubles
        doubleSwap(buf, offsetof(FGNetFDM, longitude));
        doubleSwap(buf, offsetof(FGNetFDM, altitude));
        doubleSwap(buf, offsetof(FGNetFDM, latitude));
    }
    static void doubleSwap(uint32_t *buf, int offset) {
        uint32_t tmp = buf[offset];
        buf[offset] = buf[offset + 1];
        buf[offset + 1] = tmp;
    }

public:
    FGSocketOuputReader(Sockets *socketFactory) : Component("sor") {
        rate = Rate::ForEver;
        this->sockets = socketFactory;
        this->server = 0;
        this->sockIn = 0;
    }

    FGNetFDM getLastFGNetData() {
        return lastFGNetData;
    }

    virtual void setup(Context &context) override {

        int bindPort = context.properties.getInt(P_jsb_bind_port, 5502);
        String dataFile = context.properties.getString(P_jsb_data_file, "");

        this->dataFileWriter = new FileWriter(dataFile); // todo write data log

        // TODO move the server to the context ?
        server = this->sockets->socket();
        if (server == 0) {
            context.stop(String::format("cannot create new socket from socket factory."));
            return;
        }
        String address = "127.0.0.1";

        Result bResult = this->sockets->bind(server, address, bindPort); //
        if (bResult.error != 0) {
            context.stop(String::format("cannot bind to port:%i, error:%i", bindPort, bResult.error));
            return;
        }

        Result lResult = this->sockets->listen(server);

        if (lResult.error != 0) {
            context.stop(String::format("cannot listen on port:%i", bindPort));
            return;
        }

        log(String::format("Waiting the JSBSim start up and connect to the the address:%s:%i", address.getText(), bindPort));

        this->sockIn = this->sockets->accept(server);
        if (sockIn == 0) {
            context.stop("cannot accept connect from JSBSim.");
            return;
        }
        log("JSBSim connected in, trying to receive the first package of data.");

        bool success = receive(lastFGNetData);
        if (!success) {
            context.stop("Failed to the get the connection ready from JSBSim.");
        }
    }

    virtual void run() override {
        while (true) {
            bool success = receive(lastFGNetData);
            if (success) {
                continue;
            }
            // exit loop?
            break;
        }
        log("Error, JSBSim socket connection broken.");
    }

    bool receive(FGNetFDM &data) {
        int size = sizeof(FGNetFDM);
        char buf[size];

        int received = 0;
        char *from = buf;
        bool broken = false;
        while (true) {

            int len = sockets->receive(sockIn, from, size - received);
            if (len <= 0) {
                if (len == 0) {
                    // remote closed the connection
                }
                // error occurred while reading.
                broken = true;
                break;
            }

            received += len;
            if (received == size) {
                break;
            }
        }
        if (broken) {
            return false;
        }
        // byteSwap(buf, size);
        data = *(FGNetFDM *)buf;
        String line;
        log("data received, going to log it....");

        log(buildDataLine(data, line));
        log("Done of log data.");
        return true;
    }

    static String &buildDataLine(const FGNetFDM &data, String &line) {

        if (false) {
            return line << "SKIP";
        }
        String SL = "\t";
        String SM = "=\t\t";
        String SR = "\n";
        int num = 0;
        line << SL << num++ << "(version)" << SM << data.version << SR // increment when data values change
             << SL << num++ << "" << SM << data.padding << SR          // padding
             // Positions
             << SL << num++ << "" << SM << data.longitude << SR          // geodetic (radians)
             << SL << num++ << "" << SM << data.latitude << SR           // geodetic (radians)
             << SL << num++ << "(altitude)" << SM << data.altitude << SR // above sea level (meters)
             << SL << num++ << "(agl)" << SM << data.agl << SR           // above ground level (meters)
             << SL << num++ << "(roll)" << SM << data.phi << SR          // roll (radians)
             << SL << num++ << "(pitch)" << SM << data.theta << SR       // pitch (radians)
             << SL << num++ << "(yaw)" << SM << data.psi << SR           // yaw or true heading (radians)
             << SL << num++ << "" << SM << data.alpha << SR              // angle of attack (radians)
             << SL << num++ << "" << SM << data.beta << SR               // side slip angle (radians)

             // Velocities
             << SL << num++ << "(phidot)" << SM << data.phidot << SR // roll rate (radians/sec)
             << SL << num++ << "" << SM << data.thetadot << SR       // pitch rate (radians/sec)
             << SL << num++ << "" << SM << data.psidot << SR         // yaw rate (radians/sec)
             << SL << num++ << "" << SM << data.vcas << SR           // calibrated airspeed
             << SL << num++ << "" << SM << data.climb_rate << SR     // feet per second
             << SL << num++ << "" << SM << data.v_north << SR        // north velocity in local/body frame, fps
             << SL << num++ << "" << SM << data.v_east << SR         // east velocity in local/body frame, fps
             << SL << num++ << "" << SM << data.v_down << SR         // down/vertical velocity in local/body frame, fps
             << SL << num++ << "" << SM << data.v_body_u << SR       // ECEF velocity in body axis
             << SL << num++ << "" << SM << data.v_body_v << SR       // ECEF velocity in body axis
             << SL << num++ << "" << SM << data.v_body_w << SR       // ECEF velocity in body axis

             // Accelerations
             << SL << num++ << "(A_X_pilot)" << SM << data.A_X_pilot << SR // X accel in body frame ft/sec^2
             << SL << num++ << "" << SM << data.A_Y_pilot << SR            // Y accel in body frame ft/sec^2
             << SL << num++ << "" << SM << data.A_Z_pilot << SR            // Z accel in body frame ft/sec^2

             // Stall
             << SL << num++ << "" << SM << data.stall_warning << SR // 0.0 - 1.0 indicating the amount of stall
             << SL << num++ << "" << SM << data.slip_deg << SR      // slip ball deflection

             // Pressure

             // Engine status
             << SL << num++ << "(num_engines)" << SM << data.num_engines << SR // Number of valid engines
            ;
        for (int i = 0; i < FGNetFDM::FG_MAX_ENGINES; i++) {
            int j = 0;
            line << SL << num << "[" << i << "][" << j++ << "]" << SM << data.eng_state[i] << SR // Engine state (off, cranking, running)
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.rpm[i] << SR       // Engine RPM rev/min
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.fuel_flow[i] << SR // Fuel flow gallons/hr
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.fuel_px[i] << SR   // Fuel pressure psi
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.egt[i] << SR       // Exhuast gas temp deg F
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.cht[i] << SR       // Cylinder head temp deg F
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.mp_osi[i] << SR    // Manifold pressure
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.tit[i] << SR       // Turbine Inlet Temperature
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.oil_temp[i] << SR  // Oil temp deg F
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.oil_px[i] << SR    // Oil pressure psi
                ;
        }

        // Consumables
        line << SL << num++ << "(num_tanks)" << SM << data.num_tanks << SR // Max num++ber of fuel tanks
            ;
        for (int i = 0; i < FGNetFDM::FG_MAX_TANKS; i++) {
            int j = 0;
            line << SL << num << "[" << i << "][" << j++ << "]" << SM << data.fuel_quantity[i] << SR //
                ;
        }
        // Gear status
        line << SL << num++ << "(num_wheels)" << SM << data.num_wheels << SR;
        for (int i = 0; i < FGNetFDM::FG_MAX_WHEELS; i++) {
            int j = 0;
            line << SL << num << "[" << i << "][" << j++ << "]" << SM << data.wow[i] << SR              //
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.gear_pos[i] << SR         //
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.gear_steer[i] << SR       //
                 << SL << num << "[" << i << "][" << j++ << "]" << SM << data.gear_compression[i] << SR //
                ;
        }

        // Environment
        line << SL << num++ << "(cur_time)" << SM << data.cur_time << SR // current simulation time
             << SL << num++ << "" << SM << data.warp << SR               // offset in seconds to unix time
             << SL << num++ << "" << SM << data.visibility << SR         // visibility in meters (for env. effects)

             // Control surface positions (normalized values)
             << SL << num++ << "" << SM << data.elevator << SR          //
             << SL << num++ << "" << SM << data.elevator_trim_tab << SR //
             << SL << num++ << "" << SM << data.left_flap << SR         //
             << SL << num++ << "" << SM << data.right_flap << SR        //
             << SL << num++ << "" << SM << data.left_aileron << SR      //
             << SL << num++ << "" << SM << data.right_aileron << SR     //
             << SL << num++ << "" << SM << data.rudder << SR            //
             << SL << num++ << "" << SM << data.nose_wheel << SR        //
             << SL << num++ << "" << SM << data.speedbrake << SR        //
             << SL << num++ << "" << SM << data.spoilers << SR          //
            ;
        return line;
    }
};

} // namespace a8::hal::native