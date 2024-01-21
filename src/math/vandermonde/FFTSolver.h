#ifndef NTRP_MATH_VANDERMONDE_FFT_SOLVER_H
#define NTRP_MATH_VANDERMONDE_FFT_SOLVER_H

/**
 * @file FFTSolver.h
 * @brief Implementation of Fast Fourier Vandermonde solver
 */

#include "Solver.h"

namespace math::vandermonde {

/**
 * @brief \f$O(n \log^2 n)\f$ solver of Vandermonde matrix systems
 *        from Kaltofen & Yagati 1988, Improved Sparse Multivariate Polynomial
 *        Interpolation Algorithms, ISSAC88 467-474.
 *
 * Solving transposed Vandermonde system of equations \f$A \hat{x} = \hat{a}\f$
 * where
 * \f[
 *  A=\left(\begin{array}{cccc}
 *   1 & 1 & \cdots & 1 \\
 *   v_1 & v_2 & \cdots & v_n \\
 *  \vdots & \vdots & \ddots & \vdots \\
 *   v_1^{n-1} & v_2^{n-1} & \cdots & v_n^{n-1}
 *   \end{array}\right),
 * \hat{x}=\left(\begin{array}{c}x_1\\x_2\\\vdots\\x_n\end{array}\right),
 *   \hat{a}=\left(\begin{array}{c}a_1\\a_2\\\vdots\\a_n\end{array}\right),
 * \f]
 * Let \f$B=A^T\f$, we have \f$\hat{x}=(B^{-1})^T\hat{a}\f$. If j-th column
 * \f$b_j=(b_{0,j},b_{1,j},\ldots,b_{n-1,j})^T\f$, of \f$B^{-1}\f$ is regarded
 * as the coefficients of \f$z^{0},z^{1},\ldots,z^{n-1}\f$ in the polynomial
 * \f$B_{j}(z)=b_{0,j}+b_{1,j}z+\cdots+b_{n-1,j}z^{n-1}\f$ then the (i,j)-th
 * element of \f$BB^{-1}\f$ is just
 * \f[
 *    B_j(v_i) = \left\{\begin{array}{cl}
 *      1 & \mbox{if}\ i = j \\
 *      0 & \mbox{otherwise}
 *    \end{array}\right.
 * \f]
 * Therefore,
 * \f[
 *   B_j(z) = \prod_{1\le k\le n, k\ne j}\frac{z-v_k}{v_j-v_k}, \quad 1\le j \le
 * n \f] and \f[ \hat{x}=(B^{-1})^T \hat{a} = \left(\begin{array}{c}
 *     \sum_{i=0}^{n-1} b_{i,1} \hat{a}_i\\
 *     \vdots\\
 *     \sum_{i=0}^{n-1} b_{i,n} \hat{a}_i
 *   \end{array}\right).
 * \f]
 * Notice that \f$x_i=coeff(z^n)\f$ in \f$B_i(z)D(z)\f$ where
 * \f[
 *   D(z) = a_0 z^n+a_1 z^{n-1}+\cdots+a_{n-2}z^2+a_{n-1}z.
 * \f]
 * The \f$x_i\f$ can be computed quickly without computing all the
 * \f$B_i(z)D(z)\f$ by exploiting the fact that
 * \f[
 *   B_i(z) = \frac{B(z)}{\alpha_i(z-v_i)}, \quad \mbox{where}\quad
 *   B(z)=\prod_{1\le j\le n}(z-v_j),\quad \alpha_i=\prod_{j\ne i}(v_i-v_j).
 * \f]
 * Let
 * \f[
 *   B(z) D(z) = q_{2n} z^{2n} + q_{2n-1} z^{2n-1} + \cdots + q_1 z + q_0.
 * \f]
 * The coefficient of \f$z^j\f$ in the quatient of \f$B(z)D(z)/(z-w)\f$ is given
 * by \f[ Q_j(w)=q_{2n}w^{j-1}+q_{2n-1} w^{j-2}+\cdots+q_{j+2} w + q_{j+1}. \f]
 * Clearly, \f$Q_n(v_i)=\alpha_i x_i\f$. \f$Q_n(w)\f$ is a degree \f$n-1\f$
 * polynomial and has to be evaluated at \f$n\f$ points
 * \f$v_1,v_2,\ldots,v_n.\f$
 *
 * The \f$\alpha_i\f$ still remain to be evaluated. Observe that
 * \f$B'=dB(x)/dz=\sum_i\prod_{j\ne i}(z-v_j).\f$ Therefore
 * \f$B'(v_i)=\alpha_i.\f$ All the \f$\alpha_i, 1\le i\le n,\f$ can be obtained
 * by evaluating \f$B'(z)\f$ at points \f$v_1,v_2,\ldots,v_n\f$. This can be
 * done efficiently using algorithm that was used to evaluate \f$Q_n(w)\f$.
 *
 * The Algorithm:
 * - Input: Entries \f$v_1,\cdots,v_n\f$ which make up transposed Vandermonde
 * matrix \f$A\f$; vector \f$\hat{a}=(a_1,\cdots,a_n)^T\f$
 * - Output: Vector \f$\hat{x}=(x_1,\ldots,x_n)^T\f$ where \f$x=A^{-1}a\f$
 *   -# Compute the polynomial \f$B(z)=\prod_{1\le j\le n}(z-vj)\f$ by the
 * tree multiplication algorithm.
 *   -# Let \f$D(z)=a_1 z^n+z_2 z^{n-1}+\cdots+a_{n-1} z^2+a_n z\f$.
 * Compute \f$B(z) D(z)\f$ using fast polynomial multiplication.
 * Let \f$B(z)D(z)-q_{2n}z^{2n}+q_{2n-1}z^{2n-1}+\cdots+q_0\f$. Read off
 * the polynomial \f$Q_n(w)=q_{2n}w^{n-1}+\cdots+q_{n+2}w+q_{n+1}\f$ from
 * \f$B(z)D(z)\f$
 *   -# Evaluate \f$Q_n(w)\f$ at points \f$v_1,\ldots,v_n\f$ to obtain
 * \f$\alpha_i x_i\f$. (The \f$\alpha_i\f$ are the scalars described earlier.)
 *   -# Compute the \f$\alpha_i\f$ by evaluating \f$B'(z)\f$ at points
 * \f$v_1,\ldots,v_n\f$.
 *   -# Output \f$(\alpha_i x_i/\alpha_i)_{i=1,\ldots,n}\f$
 *
 * The above algorithm is split into two stages.
 */

template <class F>
class FFTSolver : public Solver<F> {
 public:
  /**
   * @brief Setup FFT transpose Vandermode solver.
   * @param size - input matrix size
   * @param entries - first (second) row in transpose Vandermonde matrix
   * other entries are powers of these. \$fa_{i,j}=v_j^i\f$.
   * The complexity of this setup is \f$O(n\log^2 n)\f$ where n is the size.
   */
  FFTSolver(unsigned int size, const F* entries);

  virtual ~FFTSolver();

  virtual unsigned int getMatrixSize() const { return size_; }

  virtual F getEntry(unsigned int i) const { return -subprods_[0][i]; }

  virtual void solveTranspose(const F* values, F* result);

  virtual void evaluate(const F* coeffs, F* result);

 private:
  unsigned int size_;    /**< size_ number of values to solve for */
  unsigned int logsize_; /**< size_ < 2^logsize_ */
  F* scalars_;           /**< \f$\alpha_i\f$ above */
  F** subprods_;
};

}  // namespace math::vandermonde

#include "FFTSolver-Impl.h"

#endif  // NTRP_MATH_VANDERMONDE_FFT_SOLVER_H
