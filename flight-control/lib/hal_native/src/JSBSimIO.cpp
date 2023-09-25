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

    this->fgSocketReader = new FGSocketOuputReader(this->socketFactory);
    this->addChild(fgSocketReader, context);

    this->client = this->socketFactory->socketClient();
    log("Connecting to JSBSim ...");
    String connectHost = context.properties.getString("todo","127.0.0.1");
    int connectPort = context.properties.getInt("todo",5126);
    
    Result connected = this->client->connect(connectHost, connectPort);
    if (connected.error) {
        context.stop("Failed connect to JSBSim");
        return;
    }
    log("Successfully connected to JSBSim.");

    Runnable *runner2 = new ReadWriteRunner(new SocketReader(client), consoleWriter);
    Component *com = new RunnerComponent("rwr", runner2);

    this->addChild(com, context);
}

} // namespace a8::hal::native