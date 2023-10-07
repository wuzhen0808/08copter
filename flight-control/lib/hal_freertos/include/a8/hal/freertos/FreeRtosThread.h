#pragma once
#include "a8/util.h"
#include "a8/util/thread.h"

using namespace a8::util;
using namespace a8::util::thread;

namespace a8::hal::freertos {

#define DEFAULT_THREAD_NAME ("My Thread")

class FreeRtosThread : public Thread {
    using run = void (*)(void *);

private:
    static const int LOCAL_THIS = 0;
    static const int STACK_DEPTH = 521 / 4;
    static const int DEFAULT_PRIORITY = 1;
    //TaskHandle_t handle;
    void * handle_;

    run run_;
    void *context;
    
    static void taskFunction(void *pvParameters) ;

public:
    static Thread *start(run runF, void *context) ;

    static Thread *getCurrentThread() ;
    FreeRtosThread(run runF, void *context) ;
    ~FreeRtosThread() ;
    virtual void start() ;
};

} // namespace a8::hal::freertos
