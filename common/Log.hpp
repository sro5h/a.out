#pragma once

#include "Config.hpp"

#include <iostream>

namespace aout { namespace log {

AOUT_API std::ostream& beginDebug(std::ostream& os);
AOUT_API std::ostream& beginWarng(std::ostream& os);
AOUT_API std::ostream& beginError(std::ostream& os);

AOUT_API std::ostream& end(std::ostream& os);

} }

#define AOUT_LOG_DEBUG(msg) (std::cout << ::aout::log::beginDebug << msg << ::aout::log::end)
#define AOUT_LOG_WARNG(msg) (std::cout << ::aout::log::beginWarng << msg << ::aout::log::end)
#define AOUT_LOG_ERROR(msg) (std::cout << ::aout::log::beginError << msg << ::aout::log::end)
