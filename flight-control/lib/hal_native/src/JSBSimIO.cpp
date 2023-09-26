#include "a8/hal/native/JSBSimIO.h"
#include "a8/hal/SimpleLogger.h"
#include "a8/util/ReadWriteRunner.h"
#include "a8/util/RunnerComponent.h"
#include "a8/util/SocketReader.h"

using namespace a8::util;

namespace a8::hal::native {

void JSBSimIO::populate(Context &context) {
}

void JSBSimIO::setup(Context &context) {

    this->fgSocketReader = this->addChild<FGSocketOuputReader>(context, new FGSocketOuputReader(this->sockets));

    client = this->sockets->socket();
    if (client == 0) {
        context.stop("Error, cannot create socket.");
        return;
    }
    log("Connecting to JSBSim ...");
    String connectHost = context.properties.getString("todo", "127.0.0.1");
    int connectPort = context.properties.getInt("todo", 5126);

    Result connected = this->sockets->connect(client, connectHost, connectPort);
    if (connected.error) {
        context.stop("Failed connect to JSBSim");
        return;
    }
    log("Successfully connected to JSBSim.");

    Runnable *runner2 = new ReadWriteRunner(new SocketReader(sockets, client), consoleWriter);
    Component *com = this->addChild<RunnerComponent>(context, new RunnerComponent("rwr", runner2));
}

} // namespace a8::hal::native