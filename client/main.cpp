#include <common/Log.hpp>

int main(int argc, char** argv) {
        AOUT_LOG_DEBUG("Client v" << aout::printVersion);

        return 0;
}
