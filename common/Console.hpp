#pragma once

#include "Config.hpp"

#include <functional>

namespace aout {

void onSigInt(std::function<void()> handler);

}
