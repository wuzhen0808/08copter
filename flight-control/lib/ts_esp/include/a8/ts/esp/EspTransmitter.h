#pragma once
#include "a8/link.h"
#include "a8/ts/Transmitter.h"
#include "a8/util/schedule.h"

namespace a8::ts::esp {
using namespace a8::ts;

class EspTransmitter : public Transmitter {
private:
public:
    EspTransmitter(Links *links);
    ~EspTransmitter();
    void populate(StagingContext *context) override;
    void start(StagingContext *context) override;
};

} // namespace a8::ts::esp
