#pragma once
#include "a8/link/LineApi.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include <string>
using namespace a8::link;
using namespace a8::util::comp;

namespace a8::fc::native {
/**
 * Accept the simulator's output which is as the sensor feedback for the attitude control to reaction with.
 */
class SimInSkeleton : public LineApi, public FlyWeight {

    int Col_Time = -2;     // Time
    int Col_Altitude = -2; //
    int Col_Phi = -2;
    int Col_Tht = -2;
    int Col_Psi = -2;
    int Col_Alpha = -2;
    int Col_Beta = -2;
    int Col_Latitude = -2;
    int Col_Longitude = -2;

    int lines = 0;

    MovePtr<String> lastLineRaw;
    MovePtr<Buffer<String>> lastLineParsed;
    Buffer<String> headers;

    int findIndex(int &idxR, String name, Result &rst) {
        int idx = headers.indexOf(name);
        if (idx < 0) {
            rst.errorMessage << "No index found from header of the JSBSim socket data output for column:" + name;
            return -1;
        }
        idxR = idx - 1;
        return 1;
    }

public:
    static void release(void *skeleton) {
        delete static_cast<SimInSkeleton *>(skeleton);
    }
    SimInSkeleton(LoggerFactory *logFac) : FlyWeight(logFac) {
    }
    ~SimInSkeleton() {
    }
    void line(String line) override {
        if (lines == 0) {
            headers = StringUtil::split(line, ',');
        } else {
            lastLineRaw = new String(line);
        }
        lines++;
    }

    int get(int &fIdx, String fName, String &fValue, Result &rst) {
        if (fIdx == -2) {
            Result rst;
            int ret = findIndex(fIdx, fName, rst);
            if (ret < 0) {
                fIdx = -1;
            }
        }
        if (fIdx < 0) {
            return -1;
        }
        if (lastLineRaw == 0) {
            return -1;
        }
        if (lastLineParsed == 0) {
            lastLineParsed = new Buffer<String>(StringUtil::split(lastLineRaw.ptr, ','));
        }
        fValue = lastLineParsed.ptr->get(fIdx);
        return 1;
    }
    int readDouble(int &fIdx, String fName, double &dValue, Result &rst) {

        String str;
        int ret = get(fIdx, fName, str, rst);
        if (ret < 0) {
            return ret;
        }

        dValue = std::atof(str.text());
        return 1;
    }
    int readFloat(int &fIdx, String fName, float &fValue, Result &rst) {
        double dValue;
        int ret = readDouble(fIdx, fName, dValue, rst);
        if (ret < 0) {
            return ret;
        }
        fValue = dValue;
        return ret;
    }

    int readLong(int &fIdx, String fName, long &lValue, Result &rst) {

        double dValue;
        int ret = readDouble(fIdx, fName, dValue, rst);
        if (ret < 0) {
            return ret;
        }
        lValue = dValue;
        return ret;
    }

    int getAltitude(double &altitude, Result &rst) {
        return readDouble(Col_Altitude, "Altitude", altitude, rst);
    }
    int getLatitude(double &latitude, Result &rst) {
        return readDouble(Col_Altitude, "Latitude (deg)", latitude, rst);
    }
    int getLongitude(double &longitude, Result &rst) {
        return readDouble(Col_Altitude, "Longitude (deg)", longitude, rst);
    }

    int getTime(long &time, Result &rst) {
        return readLong(Col_Altitude, "Time", time, rst);
    }
    int getPhi(float &phi, Result &rst) {
        return readFloat(Col_Phi, "Phi (deg)", phi, rst);
    }

    int getTheta(float &theta, Result &rst) { return readFloat(Col_Phi, "Tht (deg)", theta, rst); }
    int getPsi(float &psi, Result &rst) { return readFloat(Col_Phi, "Psi (deg)", psi, rst); }
    int getAccVel(Vector3f &vel, Result &rst) {
        return -1;
    }
    int getAngVel(Vector3f &vel, Result &rst) {
        return -1;
    }
};
} // namespace a8::fc::native