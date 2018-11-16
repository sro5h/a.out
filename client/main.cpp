#include <iostream>

static constexpr int VERSION_MAJOR = 0;
static constexpr int VERSION_MINOR = 1;
static constexpr int VERSION_PATCH = 1;

std::ostream& PrintVersion(std::ostream& os) {
        return os << "Client " << VERSION_MAJOR << "." << VERSION_MINOR
                  << "." << VERSION_PATCH;
}

int main(int argc, char** argv) {
        std::cout << PrintVersion << std::endl;

        return 0;
}
