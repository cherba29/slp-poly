#ifndef NTRP_MATH_VANDERMONDE_SOLVER_H
#define NTRP_MATH_VANDERMONDE_SOLVER_H

/**
 * @file Solver.h
 * @brief Abstract interface for Vandermonde Solver
 */

namespace math {
namespace vandermonde {
/**
 * @brief Abstract parent of Vandermonde matrix solver classes
 * Solving transposed Vandermonde system of equations \f$A \hat{x} = \hat{a}\f$
 * where
 * \f[
 *  A=\left(\begin{array}{cccc}
 *   1 & 1 & \cdots & 1 \\
 *   v_1 & v_2 & \cdots & v_n \\
 *  \vdots & \vdots & \ddots & \vdots \\
 *   v_1^{n-1} & v_2^{n-1} & \cdots & v_n^{n-1}
 *   \end{array}\right), \hat{x}=\left(\begin{array}{c}x_1\\x_2\\\vdots\\x_n\end{array}\right),
 *   \hat{a}=\left(\begin{array}{c}a_1\\a_2\\\vdots\\a_n\end{array}\right),
 * \f]
 */
template <class F>
class Solver {
public:
  virtual ~Solver() {}

  /**
   * @brief Return number of values this solver was setup with.
   */
  virtual unsigned int getMatrixSize() const	= 0;

  /**
   * @brief Return ith value this solver was setup with.
   * @see getMatrixSize(), FFTVandermondeSolver()
   */
  virtual F getEntry(unsigned int i) const = 0;

  /**
   * @brief Solve for \f$\hat{x}\f$ in \f$A\hat{x}=\hat{a}\f$.
   * Matrix A is setup via constructor. This can be called multiple times for
   * different values.
   * @param values input \f$\hat{a}\f$ above, of size set in constructor
   * @param result output \f$\hat{x}\f$ above, same size as values
   */
  virtual void solveTranspose(const F* values, F* result) = 0;

  /**
   * Evaluate given polynomial with coefficients coeffs on entry values given in
   * the constructor.
   * @param coeffs - Coefficients of polynomial \f$\sum_{i=0}^{n-1} c_i z^i\f$
   * @param result - \f$r_j = \sum_{i=0}^{n-1} c_i e_j^i\quad\mbox{ for }\quad j=0,\ldots,n-1\f$
   *   where n is size with which this object was setup and e_j are entries.
   */
  virtual void evaluate(const F* coeffs, F* result) = 0;

private:

};

}  // namespace vandermonde
}  // namespace math

#endif  // NTRP_MATH_VANDERMONDE_SOLVER_H
