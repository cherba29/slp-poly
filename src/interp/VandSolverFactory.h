#ifndef NTRP_INTERP_VAND_SOLVER_FACTORY_H
#define NTRP_INTERP_VAND_SOLVER_FACTORY_H

/**
 * @file VandSolverFactory.h Vandermonde solver factory
 */

#include "interp/VandSolveTypeEnum.h"

#include "math/vandermonde/Solver.h"

#include <boost/shared_ptr.hpp>

namespace interp {

/**
 * @brief VandermondeSolverFactory build Vandermonde matrix
 */
template <class F>
class VandSolverFactory {
public:

  VandSolverFactory(VandSolveTypeEnum choice);

  /**
   * @brief Build Vandermonde matrix solver. Note that constructor will spend
   * O(n^2) for quad and O(n log^2 n) for fft to setup the solver.
   * If factory was created with "BEST" option, this function will built faster
   * solver for given size.
   * @param[in] size - size of vandermonde matrix, or number of entries
   * @param[in] entries - array of entries from which all matrix entries are derived
   * @param[in] nSolve - how many times this solver will be used, default=1
   *                 (useful for BEST)
   * @return FFTVandermondeSolver or QuadVandermondeSolver
   * @see VandermondeSolver
   */
  boost::shared_ptr<math::vandermonde::Solver<F> >
  newSolver(unsigned int size, const F* entries, int nSolve=1) const;

  /**
   * @brief Get the  name of the algorithm this factory would construct.
   */
  const char* getAlgName(unsigned int size, int nSolve=1) const;

private:
  VandSolveTypeEnum choice_;
};

} // namespace interp

#include "VandSolverFactory-impl.h"
#endif  // NTRP_VAND_SOLVER_FACTORY_H
