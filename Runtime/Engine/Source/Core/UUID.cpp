#include "UUID.h"

using namespace Blueshift;
using namespace Core;

std::mt19937_64 UUID::generator;
std::uniform_int_distribution<uint64_t> UUID::distribution;