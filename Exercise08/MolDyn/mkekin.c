/* Scales the forces to Lennard-Jones units (x48), completes the velocity update and
   returns the kinetic energy. */
#include "moldyn.h"

double mkekin(double *vh, double *f, int npart, double h) {
    double sum = 0.0;
    dscal(f, 3 * npart, 48.0);
    for (int i = 0; i < 3 * npart; i++) {
        vh[i] += 0.5 * f[i] * h;          /* second half of the velocity update */
        sum += vh[i] * vh[i];
    }
    return 0.5 * sum;                     /* kinetic energy = sum(v^2) / 2 */
}
