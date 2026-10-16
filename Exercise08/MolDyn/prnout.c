/* Prints the energies, temperature and pressure (per particle where useful). */
#include <stdio.h>
#include "moldyn.h"

void prnout(int move, double ekin, double epot, double vir, double vel, int count,
            int npart, double den, double side) {
    double temp = 2.0 * ekin / (3.0 * npart);
    double pres = den * temp + vir / (3.0 * side * side * side);
    printf("%4d  %12.6f  %12.6f  %12.6f  %10.6f  %10.6f  %8.5f  %5d\n",
           move, ekin / npart, epot / npart, (ekin + epot) / npart, temp, pres, vel, count);
}
