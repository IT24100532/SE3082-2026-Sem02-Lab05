/* Serial version. Lennard-Jones forces between every pair of atoms closer than rcoff,
   using the nearest periodic image. Also adds up the potential energy (epot) and the
   virial (vir). The forces are scaled afterwards, in mkekin. */
#include "moldyn.h"

void forces(int npart, const double *x, double *f, double side, double rcoff,
            double *epot_out, double *vir_out) {
    double sideh = 0.5 * side, rcoffs = rcoff * rcoff;
    double epot = 0.0, vir = 0.0;

    for (int i = 0; i < 3 * npart; i++)
        f[i] = 0.0;

    for (int i = 0; i < 3 * npart; i += 3) {
        double xi = x[i], yi = x[i + 1], zi = x[i + 2];
        double fxi = 0.0, fyi = 0.0, fzi = 0.0;

        for (int j = i + 3; j < 3 * npart; j += 3) {     /* each pair once (j > i) */
            double xx = xi - x[j], yy = yi - x[j + 1], zz = zi - x[j + 2];
            if (xx < -sideh) xx += side;  if (xx > sideh) xx -= side;   /* nearest image */
            if (yy < -sideh) yy += side;  if (yy > sideh) yy -= side;
            if (zz < -sideh) zz += side;  if (zz > sideh) zz -= side;
            double rd = xx * xx + yy * yy + zz * zz;

            if (rd <= rcoffs) {
                double rrd  = 1.0 / rd;
                double rrd2 = rrd * rrd, rrd3 = rrd2 * rrd, rrd4 = rrd2 * rrd2;
                double rrd6 = rrd2 * rrd4, rrd7 = rrd6 * rrd;
                epot += rrd6 - rrd3;                     /* r^-12 - r^-6 */
                double r148 = rrd7 - 0.5 * rrd4;
                vir -= rd * r148;

                double forcex = xx * r148, forcey = yy * r148, forcez = zz * r148;
                fxi += forcex;  fyi += forcey;  fzi += forcez;
                f[j] -= forcex; f[j + 1] -= forcey; f[j + 2] -= forcez;   /* Newton's 3rd law */
            }
        }
        f[i] += fxi; f[i + 1] += fyi; f[i + 2] += fzi;
    }
    *epot_out = epot;
    *vir_out = vir;
}
