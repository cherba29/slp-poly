#ifndef NTRP_MATH_LINEAR_SVD_H_
#define NTRP_MATH_LINEAR_SVD_H_

/**
 * @file svd.h Implementation of Singular Value Decomposition
 */


namespace math {
namespace numeric {

/** @brief Solves A X = B for a vector X.
 *  where A = U W V^T as returned by svdcmp. Complexity O(m*n+n^2)
 * @param U - input mxn matrix result of svdcmp (U x U^T = 1)
 * @param W - input vector of size n, representing diagonal matrix
 * @param V - input nxn matrix result of svdcmp (V x V^T = 1)
 * @param B - input vector of size n
 * @param X - output, solution, vector of size n
 * @see svdcmp
 */
void svbksb(
    const long double** U,
    const long double W[],
    const long double** V,
    int m,
    int n,
    const long double B[],
    long double X[]);


void solveViaSVD(
    long double** A, long double X[], long double B[], int m, int n);

/**
 * @brief This routine computes its singular value decomposition,  A = U W V^T.
 * @param a - input/output mxn matrix, the mxn matrix U replaces a on output.
 * @param w - output nxn diagonal matrix of singular values W is output as a vector w[0..n-1].
 * @param v - output nxn matrix V (not the transpose V^T ) is output
 */
bool svdcmp(long double **a, int m, int n, long double w[], long double **v);

}  // namespace numeric
}  // namespace math

#endif  // NTRP_MATH_LINEAR_SVD_H_
