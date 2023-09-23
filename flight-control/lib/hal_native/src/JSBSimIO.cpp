#include "a8/hal/native/JSBSimIO.h"
#include "a8/hal/SimpleLogger.h"
#include "a8/util/ReadWriteRunner.h"
#include "a8/util/SocketReader.h"

using namespace a8::util;

namespace a8::hal::native {

void JSBSimIO::populate(Context &context) {
}

void JSBSimIO::setup(Context &context) {
    // TODO configurable
    this->setBindPort(5502);
    this->setConnectionAddress("127.0.0.1", 5126);
    // TODO move the server to the context ?
    this->server = this->socketFactory->socketServer();
    String address = "127.0.0.1";

    Result bResult = this->server->bind(address, this->bindPort); //
    if (bResult.error != 0) {
        context.stop(String::format("cannot bind to port:%i, error:%i", bindPort, bResult.error));
        return;
    }

    Result lResult = this->server->listen();

    if (lResult.error != 0) {
        context.stop(String::format("cannot listen on port:%i", bindPort));
        return;
    }

    S->out->println(String::format("Waiting the JSBSim start up and connect to the the address:%s:%i", address.getText(), bindPort));

    this->socketIn = this->server->accept();
    if (!this->socketIn) {
        context.stop("cannot accept connect.");
        return;
    }
    S->out->println("JSBSim connected in.");

    this->client = this->socketFactory->socketClient();
    S->out->println("Connecting to JSBSim ...");
    Result connected = this->client->connect(connectHost, connectPort);
    if (connected.error) {
        context.stop("Failed connect to JSBSim");
        return;
    }
    S->out->println("Successfully connected to JSBSim.");

    this->runner = new FGSocketOuputReadRunner(socketIn);

    context.schedule(new ReadWriteRunner(new SocketReader(client), consoleWriter));
    context.schedule(this->runner);
}



} // namespace a8::hal::native