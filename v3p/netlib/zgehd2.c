/*  -- translated by f2c (version of 23 April 1993  18:34:30).
   You must link the resulting object file with the libraries:
        -lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int zgehd2_(n, ilo, ihi, a, lda, tau, work, info)
integer *n, *ilo, *ihi;
doublecomplex *a;
integer *lda;
doublecomplex *tau, *work;
integer *info;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    doublecomplex z__1;

    /* Builtin functions */
    void d_cnjg();

    /* Local variables */
    static integer i;
    static doublecomplex alpha;
    extern /* Subroutine */ int zlarf_(), xerbla_(), zlarfg_();


/*  -- LAPACK routine (version 2.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     September 30, 1994 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZGEHD2 reduces a complex general matrix A to upper Hessenberg form H
*/
/*  by a unitary similarity transformation:  Q' * A * Q = H . */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  ILO     (input) INTEGER */
/*  IHI     (input) INTEGER */
/*          It is assumed that A is already upper triangular in rows */
/*          and columns 1:ILO-1 and IHI+1:N. ILO and IHI are normally */
/*          set by a previous call to ZGEBAL; otherwise they should be */
/*          set to 1 and N respectively. See Further Details. */
/*          1 <= ILO <= IHI <= max(1,N). */

/*  A       (input/output) COMPLEX*16 array, dimension (LDA,N) */
/*          On entry, the n by n general matrix to be reduced. */
/*          On exit, the upper triangle and the first subdiagonal of A */
/*          are overwritten with the upper Hessenberg matrix H, and the */
/*          elements below the first subdiagonal, with the array TAU, */
/*          represent the unitary matrix Q as a product of elementary */
/*          reflectors. See Further Details. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,N). */

/*  TAU     (output) COMPLEX*16 array, dimension (N-1) */
/*          The scalar factors of the elementary reflectors (see Further
*/
/*          Details). */

/*  WORK    (workspace) COMPLEX*16 array, dimension (N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */

/*  Further Details */
/*  =============== */

/*  The matrix Q is represented as a product of (ihi-ilo) elementary */
/*  reflectors */

/*     Q = H(ilo) H(ilo+1) . . . H(ihi-1). */

/*  Each H(i) has the form */

/*     H(i) = I - tau * v * v' */

/*  where tau is a complex scalar, and v is a complex vector with */
/*  v(1:i) = 0, v(i+1) = 1 and v(ihi+1:n) = 0; v(i+2:ihi) is stored on */
/*  exit in A(i+2:ihi,i), and tau in TAU(i). */

/*  The contents of A are illustrated by the following example, with */
/*  n = 7, ilo = 2 and ihi = 6: */

/*  on entry,                        on exit, */

/*  ( a   a   a   a   a   a   a )    (  a   a   h   h   h   h   a ) */
/*  (     a   a   a   a   a   a )    (      a   h   h   h   h   a ) */
/*  (     a   a   a   a   a   a )    (      h   h   h   h   h   h ) */
/*  (     a   a   a   a   a   a )    (      v2  h   h   h   h   h ) */
/*  (     a   a   a   a   a   a )    (      v2  v3  h   h   h   h ) */
/*  (     a   a   a   a   a   a )    (      v2  v3  v4  h   h   h ) */
/*  (                         a )    (                          a ) */

/*  where a denotes an element of the original matrix A, h denotes a */
/*  modified element of the upper Hessenberg matrix H, and vi denotes an
*/
/*  element of the vector defining H(i). */

/*  =====================================================================
*/

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters */

    /* Parameter adjustments */
    --work;
    --tau;
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;

    /* Function Body */
    *info = 0;
    if (*n < 0) {
        *info = -1;
    } else if (*ilo < 1 || *ilo > max(1,*n)) {
        *info = -2;
    } else if (*ihi < min(*ilo,*n) || *ihi > *n) {
        *info = -3;
    } else if (*lda < max(1,*n)) {
        *info = -5;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("ZGEHD2", &i__1, 6L);
        return 0;
    }

    i__1 = *ihi - 1;
    for (i = *ilo; i <= i__1; ++i) {

/*        Compute elementary reflector H(i) to annihilate A(i+2:ihi,i)
 */

        i__2 = i + 1 + i * a_dim1;
        alpha.r = a[i__2].r, alpha.i = a[i__2].i;
        i__2 = *ihi - i;
/* Computing MIN */
        i__3 = i + 2;
        zlarfg_(&i__2, &alpha, &a[min(i__3,*n) + i * a_dim1], &c__1, &tau[i]);
        i__2 = i + 1 + i * a_dim1;
        a[i__2].r = 1., a[i__2].i = 0.;

/*        Apply H(i) to A(1:ihi,i+1:ihi) from the right */

        i__2 = *ihi - i;
        zlarf_("Right", ihi, &i__2, &a[i + 1 + i * a_dim1], &c__1, &tau[i], &
                a[(i + 1) * a_dim1 + 1], lda, &work[1], 5L);

/*        Apply H(i)' to A(i+1:ihi,i+1:n) from the left */

        i__2 = *ihi - i;
        i__3 = *n - i;
        d_cnjg(&z__1, &tau[i]);
        zlarf_("Left", &i__2, &i__3, &a[i + 1 + i * a_dim1], &c__1, &z__1, &a[
                i + 1 + (i + 1) * a_dim1], lda, &work[1], 4L);

        i__2 = i + 1 + i * a_dim1;
        a[i__2].r = alpha.r, a[i__2].i = alpha.i;
/* L10: */
    }

    return 0;

/*     End of ZGEHD2 */

} /* zgehd2_ */

