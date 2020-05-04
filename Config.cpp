#include "Config.h"

using namespace std::chrono_literals;
using namespace std::chrono;

milliseconds Config::cellTime = 250ms;
const int Config::mesh = 7;
const int Config::bombTime = 9 * mesh;
const int Config::fireTime = 6 * mesh;
const int Config::poisonTime = 30 * mesh;
