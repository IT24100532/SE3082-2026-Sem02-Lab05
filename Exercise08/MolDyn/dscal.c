/* Multiplies every element of v by s. */
#include "moldyn.h"

void dscal(double *v, int n, double s) {
    for (int i = 0; i < n; i++)
        v[i] *= s;
}
