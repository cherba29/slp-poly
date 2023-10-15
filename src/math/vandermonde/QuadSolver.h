#ifndef NTRP_MATH_VANDERMONDE_QUAD_SOLVER_H
#define NTRP_MATH_VANDERMONDE_QUAD_SOLVER_H

/**
 * @file QuadSolver.h
 * @brief Implementation of quadratic vandermonde solver
 */

#include "Solver.h"

namespace math {
namespace vandermonde {

/**
 * @brief Quadratic-time solver of Vandermonde matrix systems
 *        Zippel, _Effective Polynomial Computation_, p.213
 * Setup this solver precomputing langrange polynomial in O(n^2)
 */
template <typename F>
class QuadSolver : public Solver<F> {
 public:
  QuadSolver(unsigned int size, const F* entries);

  virtual ~QuadSolver();

  virtual unsigned int getMatrixSize() const { return size_; }
  virtual F getEntry(unsigned int i) const { return entries_[i]; }
  virtual void solveTranspose(const F* values, F* result);

  virtual void evaluate(const F* coeffs, F* result);

 private:
  unsigned int size_;  // Size of Vandermonde system
  F* entries_;         // Vandermonde system entries
  F* rootPoly_;
};

}  // namespace vandermonde
}  // namespace math

#include "QuadSolver-Impl.h"

#endif  // NTRP_MATH_VANDERMONDE_QUAD_SOLVER_H
