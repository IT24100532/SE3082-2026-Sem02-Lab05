/* Moves the particles using their velocities and forces (velocity Verlet), applies the
   periodic boundary conditions, and does the first half of the velocity update. */
#include "moldyn.h"

void domove(double *x, double *vh, const double *f, int npart, double side, double h) {
    for (int i = 0; i < 3 * npart; i++) {
        x[i] += vh[i] * h + 0.5 * f[i] * h * h;
        if (x[i] < 0.0)   x[i] += side;    /* leaves one side, comes back the other */
        if (x[i] >= side) x[i] -= side;
        vh[i] += 0.5 * f[i] * h;           /* first half of the velocity update */
    }
}
