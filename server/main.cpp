#include <common/Log.hpp>

int main(int argc, char** argv) {
        AOUT_LOG_DEBUG("Server v" << aout::printVersion);

        return 0;
}
