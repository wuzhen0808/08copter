#pragma once
#include "a8/hal/rf24.h"
#include "a8/hal/rf24/example/BothExample.h"
#include "a8/hal/rf24/example/ClientExample.h"
#include "a8/hal/rf24/example/ServerExample.h"
#include "a8/util.h"
#include "a8/util/net.h"

namespace a8::hal::rf24::example {
class ExampleRunner {
    System *sys;
    Scheduler *sch;
    LoggerFactory *logFac;

public:
    ExampleRunner(System *sys, Scheduler *sch, LoggerFactory *logFac) {
        this->sys = sys;
        this->sch = sch;
        this->logFac = logFac;
    }
    int readType() {
        Reader &input = *sys->getInput();
        Output &output = *sys->out;
        int type = 0;
        while (type < 1 || type > 2) {
            output.println("Please select example type, 1:Server, 2:Client, 3:Both.");
            char ch;
            int ret = input.read(ch);
            if (ret < 0) {
                output.println("failed to read input.");
                return ret;
            }
            type = ch - '0';
            output.println("type read:" + type);
        }

        output.println("selected type:" + type);
        return type;
    }
    Rate readRate() {
        Reader &input = *sys->getInput();
        Output &output = *sys->out;
        int rate = 0;
        while (rate < 1 || rate > 3) {
            output.println("Please select rate, 1 : 1Hz, 2 : 10Hz, 3 : 100Hz");
            char ch;
            int ret = input.read(ch);
            if (ret < 0) {
                output.println("failed to read input.");
                return ret;
            }
            rate = ch - '0';
            output.println("rate:" + rate);
        }
        Rate ret = rate == 1 ? 1.0f : (rate == 2 ? 10.0f : 100.0f);
        output.println(String() << "selected rate:" << rate << ":" << ret);
        return ret;
    }

    int start(Result &res) {

        Output &output = *sys->out;
        String str;
        str << sys << "------------\n";
        logFac->getLogger()->info(str);
        BaseExample *example;
        int type = this->readType();
        // ClientExample *example = new ClientExample(sys, logFac, sch);
        //  ServerExample *example = new ServerExample(sys, logFac, sch);
        if (type == 1) {
            example = new ServerExample(sys, logFac, sch);
        } else if (type = 2) {
            example = new ClientExample(sys, logFac, sch);
            Rate rate = this->readRate();
            static_cast<ClientExample *>(example)->setRate(rate);
        } else if (type = 3) {
            example = new BothExample(sys, logFac, sch);
        } else {
            res << "no such type:" << type;
            return -1;
        }
        return example->start(res);
    }
};
} // namespace a8::hal::rf24::example