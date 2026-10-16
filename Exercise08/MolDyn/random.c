/* Small portable random number generator, so every machine and every thread count
   starts from exactly the same initial velocities. */
#include "moldyn.h"

static unsigned long long state = 123456789ULL;

double rand_uniform(void) {             /* uniform in [0, 1) */
    state = state * 6364136223846793005ULL + 1442695040888963407ULL;
    return (double)(state >> 11) / 9007199254740992.0;
}
