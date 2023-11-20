#pragma once
#include "a8/hal/rf24.h"
#include "a8/hal/rf24/example/BothExample.h"
#include "a8/hal/rf24/example/ClientExample.h"
#include "a8/hal/rf24/example/ServerExample.h"
#include "a8/util.h"
#include "a8/util/net.h"

namespace a8::hal::rf24::example {
class Runner {
    System *sys;
    Scheduler *sch;
    LoggerFactory *logFac;

public:
    Runner(System *sys, Scheduler *sch, LoggerFactory *logFac) {
        this->sys = sys;
        this->sch = sch;
        this->logFac = logFac;
    }
    int readType() {
        return readOption(1, 3, "Please select example type:\n 1:Server;\n 2:Client;\n 3:Both.");
    }

    int readClientNode() {
        return readOption(0, 1, "Please select client node id:\n 0: \n 1:");
    }

    int readServerNode() {
        return readOption(0, 1, "Please select server node id:\n 0:\n 1:\n?");
    }

    int readOption(int min, int max, String prompt) {
        Reader &input = *sys->getInput();
        Output &output = *sys->out;
        int type = min - 1;
        while (type < min || type > max) {
            output.println(prompt);
            char ch;
            int ret = input.read(ch);
            if (ret < 0) {
                output.println(String() << "failed to read input.");
                return ret;
            }
            type = ch - '0';
            output.println(String() << "int read:" << type);
        }

        output.println(String() << "selected:" << type);
        return type;
    }

    Rate readRate() {
        Reader &input = *sys->getInput();
        Output &output = *sys->out;
        int rate = readOption(1, 3, "Please select rate:\n 1 : 1Hz;\n 2 : 10Hz;\n 3 : 100Hz;\n");
        Rate ret = rate == 1 ? 1.0f : (rate == 2 ? 10.0f : 100.0f);
        output.println(String() << "selected rate:" << rate << ":" << ret);
        return ret;
    }

    int readConfig(int &clientNode, int &serverNode, Rate &rate) {
        int type = this->readOption(1, 4, "Please select a config:\n 1: Server,node(0);\n 2: Client,node(1),1Hz;\n 3: Both;\n 4:Other.");
        rate = 1.0f;
        clientNode = 0;
        serverNode = 1;

        switch (type) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            type = readType();
            if (type == 2 || type == 3) {
                rate = readRate();
            }
        }
        if (type == 3) {
            clientNode = 0;
            serverNode = 0;
        }
        return type;
    }

    int start(Result &res) {

        Output &output = *sys->out;
        String str;
        str << sys << "------------\n";
        logFac->getLogger()->info(str);

        BaseExample *example;
        Rate rate;
        int clientNode;
        int serverNode;
        int type = this->readConfig(clientNode, serverNode, rate);
        // ClientExample *example = new ClientExample(sys, logFac, sch);
        //  ServerExample *example = new ServerExample(sys, logFac, sch);
        if (type == 1) {
            example = new ServerExample(sys, logFac, sch);
        } else if (type == 2) {
            example = new ClientExample(sys, logFac, sch);
            static_cast<ClientExample *>(example)->setRate(rate);
        } else if (type == 3) {
            example = new BothExample(sys, logFac, sch);
            static_cast<BothExample *>(example)->setRate(rate);
        } else {
            res << "no such type:" << type;
            return -1;
        }
        example->clientNode = clientNode;
        example->serverNode = serverNode;

        return example->start(res);
    }
};
} // namespace a8::hal::rf24::example