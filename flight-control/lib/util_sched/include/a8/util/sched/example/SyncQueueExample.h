#pragma once
#include "a8/util.h"
#include "a8/util/sched.h"
/**
 *
 */
namespace a8::util::sched::example {

class SyncQueueExample {
    Scheduler *sch;
    SyncQueue<int> *queue;
    Logger *logger;

public:
    SyncQueueExample(System *sys, Scheduler *sch, LoggerFactory *logFac) {
        this->sch = sch;
        this->logger = logFac->getLogger();
    }

    void runSender() {
        int i = 0;
        while (true) {

            int ret = queue->offer(i, 1000);
            if (ret < 0) {
                logger->info("timeout to offer message to queue, retrying.");
                continue;
                //
            }
            logger->info(String() << "sent message:" << i);
            i++;
        }
    }
    void runReceiver() {
        int i;
        while (true) {
            i = queue->take(1000, -1);
            if (i == -1) {
                logger->info("timeout to take message from queue, retrying.");
                continue;
            }

            logger->info(String() << "received message:" << i);
        }
    }
    int start(Result &res) {
        queue = this->sch->createSyncQueue<int>(10);
        this->sch->createTask<SyncQueueExample *>("SyncQueueExample", this, [](SyncQueueExample *this_) {
            this_->runSender();
        });

        // this->sch->createTask<SyncQueueExample *>(this, [](SyncQueueExample *this_) {
        //     this_->runReceiver();
        // });
        return 1;
    }
};
} // namespace a8::util::sched::example