#include "Console.hpp"

namespace aout {

namespace priv {

std::function<void()> sigIntHandler;

}

#if defined(AOUT_PLATFORM_LINUX)
#include <signal.h>
#include <unistd.h>

void myHandler(int signal) {
        priv::sigIntHandler();
}

void onSigInt(std::function<void()> handler) {
        priv::sigIntHandler = handler;

        struct sigaction sigAction;
        sigAction.sa_handler = myHandler;
        sigemptyset(&sigAction.sa_mask);
        sigAction.sa_flags = 0;

        sigaction(SIGINT, &sigAction, NULL);
}
#else
#error Unsupported platform
#endif

}
