/* Places the atoms on a face-centred cubic (FCC) lattice: 4 atoms per unit cell of side a. */
#include "moldyn.h"

void fcc(double *x, int npart, int mm, double a) {
    const double offset[4][3] = {{0, 0, 0}, {0.5, 0.5, 0}, {0.5, 0, 0.5}, {0, 0.5, 0.5}};
    int ijk = 0;
    for (int i = 0; i < mm; i++)
        for (int j = 0; j < mm; j++)
            for (int k = 0; k < mm; k++)
                for (int b = 0; b < 4; b++) {
                    x[ijk]     = (i + offset[b][0]) * a;
                    x[ijk + 1] = (j + offset[b][1]) * a;
                    x[ijk + 2] = (k + offset[b][2]) * a;
                    ijk += 3;
                }
    (void)npart;
}
