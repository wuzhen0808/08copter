#pragma once
namespace a8::util::comp {
enum Stage {
    Zero = 0,
    Boot,
    Populate,
    PostPopulate,
    Setup,
    PostSetup,
    Start,
    PostStart,
    Shutdown,
    PostShutdown
};
}