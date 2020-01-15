
#include <cstdint>
#include "Pokitto.h"
#include "System.h"

uint32_t System::currentTimeMillis() {
    // getTime() value updates only every 10 milliseconds. This improves precision to 1 ms.
    return Pokitto::Core::getTime() + (480000 - 1 - SysTick->VAL) / 48000;
}
