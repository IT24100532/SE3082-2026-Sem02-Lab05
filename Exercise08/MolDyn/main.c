/* Molecular dynamics of argon atoms melting from an FCC lattice (Lennard-Jones potential,
   periodic boundary conditions). Main loop:
     1. domove   move the particles, first half of the velocity update
     2. forces   new forces, potential energy and virial
     3. mkekin   scale the forces, finish the velocity update, kinetic energy
     4. velavg   average velocity, then scale the temperature
     5. prnout   print the energies */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#else
#include <time.h>
#endif
#include "moldyn.h"

static double wtime(void) {
#ifdef _OPENMP
    return omp_get_wtime();
#else
    return (double)clock() / CLOCKS_PER_SEC;
#endif
}

int main(void) {
    int npart = NPART;
    double side = cbrt(npart / DEN);      /* box side for the requested density */
    double a = side / MM;                 /* FCC unit cell size */
    double *x  = malloc(3 * npart * sizeof(double));
    double *vh = malloc(3 * npart * sizeof(double));
    double *f  = malloc(3 * npart * sizeof(double));
    double epot, vir, ekin, vel;
    int count;

    fcc(x, npart, MM, a);
    mxwell(vh, npart, TREF);
    forces(npart, x, f, side, RCOFF, &epot, &vir);
    dscal(f, 3 * npart, 48.0);

#ifdef _OPENMP
    printf("Molecular dynamics: %d atoms, box side %.4f, %d steps, threads = %d\n",
           npart, side, MOVEMX, omp_get_max_threads());
#else
    printf("Molecular dynamics: %d atoms, box side %.4f, %d steps, serial\n", npart, side, MOVEMX);
#endif
    printf("step    ekin/N        epot/N        etot/N        temp        pres       vel     count\n");

    double tstart = wtime(), tforces = 0.0;
    for (int move = 1; move <= MOVEMX; move++) {
        domove(x, vh, f, npart, side, H);

        double t0 = wtime();
        forces(npart, x, f, side, RCOFF, &epot, &vir);
        tforces += wtime() - t0;
        epot *= 4.0;                          /* Lennard-Jones energy: 4(r^-12 - r^-6) */
        vir *= 48.0;

        ekin = mkekin(vh, f, npart, H);
        vel = velavg(vh, npart, &count);

        if (move <= ISTOP && move % IREP == 0) {   /* scale back to the reference temperature */
            double sc = sqrt(TREF / (2.0 * ekin / (3.0 * npart)));
            dscal(vh, 3 * npart, sc);
            ekin *= sc * sc;
        }
        if (move % IPRINT == 0)
            prnout(move, ekin, epot, -vir, vel, count, npart, DEN, side);
    }
    double ttotal = wtime() - tstart;
    printf("Time: total %.4f s, forces %.4f s\n", ttotal, tforces);

    free(x); free(vh); free(f);
    return 0;
}
