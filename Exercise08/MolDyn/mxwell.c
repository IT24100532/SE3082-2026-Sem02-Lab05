/* Gives the atoms random initial velocities with zero total momentum, scaled to the
   reference temperature tref. */
#include <math.h>
#include "moldyn.h"

void mxwell(double *vh, int npart, double tref) {
    int n3 = 3 * npart;
    double sum[3] = {0, 0, 0}, ekin = 0.0;

    for (int i = 0; i < n3; i++) {
        vh[i] = rand_uniform() - 0.5;
        sum[i % 3] += vh[i];
    }
    for (int i = 0; i < n3; i++) {      /* remove the drift of the whole box */
        vh[i] -= sum[i % 3] / npart;
        ekin += vh[i] * vh[i];
    }
    double ts = ekin / (3.0 * npart);   /* temperature T = sum(v^2) / (3N) */
    double sc = sqrt(tref / ts);
    for (int i = 0; i < n3; i++)
        vh[i] *= sc;
}
