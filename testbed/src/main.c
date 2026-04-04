#include "core/asserts.h"
#include <core/logger.h>

int main(void) {
    T_FATAL("CHANGED MESSAGE: %d", 99);
    T_DEBUG("ALMA");

    T_ASSERT(2 < 3);
    T_ASSERT_MSG(3 < 5, "3 smaller than 5");
}
