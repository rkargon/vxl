/* dorm2r.f -- translated by f2c (version of 4 June 1993  1:43:59).
   You must link the resulting object file with the libraries:
        -lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;

/*<    >*/
/* Subroutine */ int dorm2r_(char *side, char *trans, integer *m, integer *n,
        integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *
        c, integer *ldc, doublereal *work, integer *info, ftnlen side_len,
        ftnlen trans_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, c_dim1, c_offset, i__1, i__2;

    /* Local variables */
    static logical left;
    static integer i;
    extern /* Subroutine */ int dlarf_(char *, integer *, integer *,
            doublereal *, integer *, doublereal *, doublereal *, integer *,
            doublereal *, ftnlen);
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer i1, i2, i3, ic, jc, mi, ni, nq;
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static logical notran;
    static doublereal aii;


/*  -- LAPACK routine (version 2.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     February 29, 1992 */

/*     .. Scalar Arguments .. */
/*<       CHARACTER          SIDE, TRANS >*/
/*<       INTEGER            INFO, K, LDA, LDC, M, N >*/
/*     .. */
/*     .. Array Arguments .. */
/*<       DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), TAU( * ), WORK( * ) >*/
/*     .. */

/*  Purpose */
/*  ======= */

/*  DORM2R overwrites the general real m by n matrix C with */

/*        Q * C  if SIDE = 'L' and TRANS = 'N', or */

/*        Q'* C  if SIDE = 'L' and TRANS = 'T', or */

/*        C * Q  if SIDE = 'R' and TRANS = 'N', or */

/*        C * Q' if SIDE = 'R' and TRANS = 'T', */

/*  where Q is a real orthogonal matrix defined as the product of k */
/*  elementary reflectors */

/*        Q = H(1) H(2) . . . H(k) */

/*  as returned by DGEQRF. Q is of order m if SIDE = 'L' and of order n */
/*  if SIDE = 'R'. */

/*  Arguments */
/*  ========= */

/*  SIDE    (input) CHARACTER*1 */
/*          = 'L': apply Q or Q' from the Left */
/*          = 'R': apply Q or Q' from the Right */

/*  TRANS   (input) CHARACTER*1 */
/*          = 'N': apply Q  (No transpose) */
/*          = 'T': apply Q' (Transpose) */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix C. M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix C. N >= 0. */

/*  K       (input) INTEGER */
/*          The number of elementary reflectors whose product defines */
/*          the matrix Q. */
/*          If SIDE = 'L', M >= K >= 0; */
/*          if SIDE = 'R', N >= K >= 0. */

/*  A       (input) DOUBLE PRECISION array, dimension (LDA,K) */
/*          The i-th column must contain the vector which defines the */
/*          elementary reflector H(i), for i = 1,2,...,k, as returned by
*/
/*          DGEQRF in the first k columns of its array argument A. */
/*          A is modified by the routine but restored on exit. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A. */
/*          If SIDE = 'L', LDA >= max(1,M); */
/*          if SIDE = 'R', LDA >= max(1,N). */

/*  TAU     (input) DOUBLE PRECISION array, dimension (K) */
/*          TAU(i) must contain the scalar factor of the elementary */
/*          reflector H(i), as returned by DGEQRF. */

/*  C       (input/output) DOUBLE PRECISION array, dimension (LDC,N) */
/*          On entry, the m by n matrix C. */
/*          On exit, C is overwritten by Q*C or Q'*C or C*Q' or C*Q. */

/*  LDC     (input) INTEGER */
/*          The leading dimension of the array C. LDC >= max(1,M). */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension */
/*                                   (N) if SIDE = 'L', */
/*                                   (M) if SIDE = 'R' */

/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          < 0: if INFO = -i, the i-th argument had an illegal value */

/*  =====================================================================
*/

/*     .. Parameters .. */
/*<       DOUBLE PRECISION   ONE >*/
/*<       PARAMETER          ( ONE = 1.0D+0 ) >*/
/*     .. */
/*     .. Local Scalars .. */
/*<       LOGICAL            LEFT, NOTRAN >*/
/*<       INTEGER            I, I1, I2, I3, IC, JC, MI, NI, NQ >*/
/*<       DOUBLE PRECISION   AII >*/
/*     .. */
/*     .. External Functions .. */
/*<       LOGICAL            LSAME >*/
/*<       EXTERNAL           LSAME >*/
/*     .. */
/*     .. External Subroutines .. */
/*<       EXTERNAL           DLARF, XERBLA >*/
/*     .. */
/*     .. Intrinsic Functions .. */
/*<       INTRINSIC          MAX >*/
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input arguments */

/*<       INFO = 0 >*/
    /* Parameter adjustments */
    --work;
    c_dim1 = *ldc;
    c_offset = c_dim1 + 1;
    c -= c_offset;
    --tau;
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;

    /* Function Body */
    *info = 0;
/*<       LEFT = LSAME( SIDE, 'L' ) >*/
    left = lsame_(side, "L", 1L, 1L);
/*<       NOTRAN = LSAME( TRANS, 'N' ) >*/
    notran = lsame_(trans, "N", 1L, 1L);

/*     NQ is the order of Q */

/*<       IF( LEFT ) THEN >*/
    if (left) {
/*<          NQ = M >*/
        nq = *m;
/*<       ELSE >*/
    } else {
/*<          NQ = N >*/
        nq = *n;
/*<       END IF >*/
    }
/*<       IF( .NOT.LEFT .AND. .NOT.LSAME( SIDE, 'R' ) ) THEN >*/
    if (! left && ! lsame_(side, "R", 1L, 1L)) {
/*<          INFO = -1 >*/
        *info = -1;
/*<       ELSE IF( .NOT.NOTRAN .AND. .NOT.LSAME( TRANS, 'T' ) ) THEN >*/
    } else if (! notran && ! lsame_(trans, "T", 1L, 1L)) {
/*<          INFO = -2 >*/
        *info = -2;
/*<       ELSE IF( M.LT.0 ) THEN >*/
    } else if (*m < 0) {
/*<          INFO = -3 >*/
        *info = -3;
/*<       ELSE IF( N.LT.0 ) THEN >*/
    } else if (*n < 0) {
/*<          INFO = -4 >*/
        *info = -4;
/*<       ELSE IF( K.LT.0 .OR. K.GT.NQ ) THEN >*/
    } else if (*k < 0 || *k > nq) {
/*<          INFO = -5 >*/
        *info = -5;
/*<       ELSE IF( LDA.LT.MAX( 1, NQ ) ) THEN >*/
    } else if (*lda < max(1,nq)) {
/*<          INFO = -7 >*/
        *info = -7;
/*<       ELSE IF( LDC.LT.MAX( 1, M ) ) THEN >*/
    } else if (*ldc < max(1,*m)) {
/*<          INFO = -10 >*/
        *info = -10;
/*<       END IF >*/
    }
/*<       IF( INFO.NE.0 ) THEN >*/
    if (*info != 0) {
/*<          CALL XERBLA( 'DORM2R', -INFO ) >*/
        i__1 = -(*info);
        xerbla_("DORM2R", &i__1, 6L);
/*<          RETURN >*/
        return 0;
/*<       END IF >*/
    }

/*     Quick return if possible */

/*<    >*/
    if (*m == 0 || *n == 0 || *k == 0) {
        return 0;
    }

/*<    >*/
    if (left && ! notran || ! left && notran) {
/*<          I1 = 1 >*/
        i1 = 1;
/*<          I2 = K >*/
        i2 = *k;
/*<          I3 = 1 >*/
        i3 = 1;
/*<       ELSE >*/
    } else {
/*<          I1 = K >*/
        i1 = *k;
/*<          I2 = 1 >*/
        i2 = 1;
/*<          I3 = -1 >*/
        i3 = -1;
/*<       END IF >*/
    }

/*<       IF( LEFT ) THEN >*/
    if (left) {
/*<          NI = N >*/
        ni = *n;
/*<          JC = 1 >*/
        jc = 1;
/*<       ELSE >*/
    } else {
/*<          MI = M >*/
        mi = *m;
/*<          IC = 1 >*/
        ic = 1;
/*<       END IF >*/
    }

/*<       DO 10 I = I1, I2, I3 >*/
    i__1 = i2;
    i__2 = i3;
    for (i = i1; i__2 < 0 ? i >= i__1 : i <= i__1; i += i__2) {
/*<          IF( LEFT ) THEN >*/
        if (left) {

/*           H(i) is applied to C(i:m,1:n) */

/*<             MI = M - I + 1 >*/
            mi = *m - i + 1;
/*<             IC = I >*/
            ic = i;
/*<          ELSE >*/
        } else {

/*           H(i) is applied to C(1:m,i:n) */

/*<             NI = N - I + 1 >*/
            ni = *n - i + 1;
/*<             JC = I >*/
            jc = i;
/*<          END IF >*/
        }

/*        Apply H(i) */

/*<          AII = A( I, I ) >*/
        aii = a[i + i * a_dim1];
/*<          A( I, I ) = ONE >*/
        a[i + i * a_dim1] = 1.;
/*<    >*/
        dlarf_(side, &mi, &ni, &a[i + i * a_dim1], &c__1, &tau[i], &c[ic + jc
                * c_dim1], ldc, &work[1], 1L);
/*<          A( I, I ) = AII >*/
        a[i + i * a_dim1] = aii;
/*<    10 CONTINUE >*/
/* L10: */
    }
/*<       RETURN >*/
    return 0;

/*     End of DORM2R */

/*<       END >*/
} /* dorm2r_ */

