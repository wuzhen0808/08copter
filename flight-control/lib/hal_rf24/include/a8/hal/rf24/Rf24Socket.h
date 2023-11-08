#pragma once
#include "a8/util/net.h"
namespace a8::hal::nrf24 {
enum Status {
    Free,
    Running
};
struct Rf24Socket {

    Status status;
    void close();
};

} // namespace a8::hal::nrf24