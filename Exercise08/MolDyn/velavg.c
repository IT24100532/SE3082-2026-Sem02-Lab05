/* Average particle speed, and how many particles move faster than 1.5x the average. */
#include <math.h>
#include "moldyn.h"

double velavg(const double *vh, int npart, int *count) {
    double total = 0.0;
    for (int i = 0; i < 3 * npart; i += 3)
        total += sqrt(vh[i] * vh[i] + vh[i + 1] * vh[i + 1] + vh[i + 2] * vh[i + 2]);
    double vaver = total / npart;

    int c = 0;
    for (int i = 0; i < 3 * npart; i += 3)
        if (sqrt(vh[i] * vh[i] + vh[i + 1] * vh[i + 1] + vh[i + 2] * vh[i + 2]) > 1.5 * vaver)
            c++;
    *count = c;
    return vaver;
}
