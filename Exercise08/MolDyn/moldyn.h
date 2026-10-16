/* Molecular dynamics of argon atoms in a box with periodic boundary conditions.
   Reduced Lennard-Jones units (sigma = epsilon = mass = 1). Positions, velocities and
   forces are stored as x0,y0,z0, x1,y1,z1, ... in arrays of length 3*npart. */
#ifndef MOLDYN_H
#define MOLDYN_H

#define MM      13                  /* FCC unit cells per side          */
#define NPART   (4 * MM * MM * MM)  /* 4 atoms per cell: 8788 atoms     */
#define DEN     0.83134             /* density                          */
#define TREF    0.722               /* reference temperature            */
#define RCOFF   2.5                 /* cut-off radius of the potential  */
#define H       0.005               /* time step                        */
#define IREP    10                  /* rescale the temperature every IREP steps... */
#define ISTOP   20                  /* ...until step ISTOP              */
#define IPRINT  5                   /* print every IPRINT steps         */
#define MOVEMX  20                  /* number of time steps             */

void   fcc(double *x, int npart, int mm, double a);
void   mxwell(double *vh, int npart, double tref);
void   domove(double *x, double *vh, const double *f, int npart, double side, double h);
void   forces(int npart, const double *x, double *f, double side, double rcoff,
              double *epot, double *vir);
double mkekin(double *vh, double *f, int npart, double h);
double velavg(const double *vh, int npart, int *count);
void   dscal(double *v, int n, double s);
void   prnout(int move, double ekin, double epot, double vir, double vel, int count,
              int npart, double den, double side);
double rand_uniform(void);

#endif
