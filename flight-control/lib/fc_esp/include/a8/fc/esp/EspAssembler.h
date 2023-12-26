#pragma once
#include "a8/fc.h"
#include "a8/fc/esp/EspCommander.h"
#include "a8/hal/esp.h"
#include "a8/util.h"
#include "a8/util/sched.h"
#include <MPU9250.h>
namespace a8::fc::esp {
using namespace a8::util;
using namespace a8::hal::esp;
class EspAssembler : public Assembler {
protected:
public:
    EspAssembler() : Assembler() {
    }

    ~EspAssembler() {
    }
    virtual void populate(StagingContext *sc) override {
        Assembler::populate(sc);
        this->rpy = new EspRpy(loggerFactory);
        this->commander = new EspCommander(pm, rpy, sc->scheduler, sc->getSys(), loggerFactory);
    }
    virtual void setup(StagingContext *sc) override {
        Assembler::setup(sc);
        EspUtil::setupWire();
        this->rpy->setup();
        this->commander->setup();
    }
};

} // namespace a8::fc::esp
