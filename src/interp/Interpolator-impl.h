#ifndef NTRP_INTERP_INTERPOLATOR_H
#error "Do not include this file. This is used internally by Interepolator.h"
#endif

/**
 * @file Interpolator-impl.h Implementation of interp::Interpolator::intepolate
 * and sparseInterpolateStage.
 */

#include "complexity/Perf.h"

#include "eval/UnivEvaluator.h"
#include "eval/util/InstructionMaxDegree.h"

#include "util/Timer.h"
#include "util/SmartAssert.h"

#include <iomanip>

#define LOG_MODULE ::logging::LogModuleEnum::INTERPOLATOR

namespace interp {

namespace detail {
/**
 * @brief Check if given polynomial matches evaluator at n random eval points.
 * @param[in] verifyCount evaluate evaluator and polynomial at that many points
 *            and compare results. If single mismatch occurs returns false
 * @param[in] ppol partial polynomial to evaluate
 * @param[in] evaluator black box representing polynomial
 * @return false if blackbox mismatches polynomial
 */
template <typename F, typename E>
bool validatePolynomial(
  unsigned int verifyCount,
  boost::shared_ptr<algebra::PartialPolynomial<F> const> ppol,
  E& evaluator)
{
  int nVars = ppol->getTotalNumberOfVariables();
  boost::scoped_array<F> evalPoints(new F[nVars]);
  for (unsigned int i = 0; i < verifyCount; ++i) {
    for (int j = 0; j < nVars; ++j) {
      evalPoints[j] = ppol->getFixedVal(j);
      //evalPoints[j].setTo(math::primes[j]);
      LTRC_ << "Fixed evaluation point " << j << ". "
            << evaluator.getAnnotation().getDataName(j)
            << " = " << evalPoints[j];
    }
    boost::shared_ptr<algebra::Polynomial<F> const> mpol
      = ppol->getPolynomial();
    int mNVars = mpol->getNumberOfVariables();
    for (int j = 0; j < mNVars; ++j) {
      int idx = ppol->getVarIdx(j);
      evalPoints[idx].invertableRandom();
      LTRC_ << "Random evaluation point " << idx << ". "
            << evaluator.getAnnotation().getDataName(idx)
            << " = " << evalPoints[idx];
    }
    F polVal = (*ppol)(evalPoints.get());
    F evalVal = evaluator(evalPoints.get());
    LTRC_ << "Evaluated pol=="  << polVal << " bbox==" << evalVal;
    if (polVal != evalVal) {
      LERR_ << "Did not match Black box polVal=" << polVal
            << " evalVal=" << evalVal;
      return false;
    }
  }
  return true;
}
}   // namespace detail

template <typename F>  template <typename E>
boost::shared_ptr<algebra::Polynomial<F> const>
Interpolator<F>::interpolate(
    E& evaluator,
    boost::shared_ptr<algebra::PartialPolynomial<F> > ppol) {
  int nVars = evaluator.getNumberOfVariables();

  if (nVars < 1) {
    LAPP2_ << "Got constant polynomial ...";
    boost::shared_ptr<algebra::Polynomial<F> > constPol(
        new algebra::Polynomial<F>(std::vector<std::string>(), 1));
    constPol->setCoeff(0, evaluator(NULL));
    return constPol;
  }

  // Get an upper bound on maximum degree of  each variable
  eval::InstructionMaxDegree maxDegree(nVars, evaluator.getDataSize());
  evaluator.traceWith(maxDegree);

  const unsigned int* varMaxDegrees = maxDegree.get();
  LAPP_ << "Estimated Max degree bounds for " << nVars << " variable(s)";
  long double maxNTerms = 1.0;
  for (int i = 0; i < nVars; i++) {
    LAPP_ << std::setw(3) << std::right << (i+1) << ". "
          << evaluator.getAnnotation().getDataName(i)
          << " <= " << varMaxDegrees[i];
    maxNTerms *= (varMaxDegrees[i]+1);
  }
  LAPP_ << "Maximum number of terms is: " << maxNTerms;
//  LAPP_ << "  can be solved directly in dense mode in "
//        << util::formatSecs(this->vf_->getPredictedTime(maxNTerms));
//  LAPP_ << "  can be solved directly in dense mode in "
//        << util::formatSecs(complexity::Perf::predicted(
//            "VandSolve",maxNTerms,1));

  // Use random/(or better picked primes) non-zero points for evaluation
  // all vars are fixed at these values except one
  boost::scoped_array<F> evalPoints(new F[nVars]);
  for (int i = 0; i < nVars; ++i) {
    //evalPoints[i].invertableRandom();
    evalPoints[i].setTo(math::primes[i]);
    LDBG_ << "Random evaluation point " << i << ". "
          << evaluator.getAnnotation().getDataName(i)
          << " = " << evalPoints[i];
  }

  // Build a pool of partial polynomials which can be merged to larger ones
  typedef boost::shared_ptr<algebra::PartialPolynomial<F> >
      PartialPolynomialPtr;

  if (nVars == 1) {  // Perform univariate interpolation
    std::vector<int> varIdx(1, 0);  // univariate
    LAPP2_ << "Single variable, newton interpolating ...";
    PartialPolynomialPtr univPartialPol(
        new algebra::PartialPolynomial<F>(
            nVars,             // Total number of variables
            evalPoints.get(),  // Values to which variables have been fixed
            varIdx,            // Variable indices composing this polynomial
            newtonInterpolateUnivariateEvaluator<F>(
                varMaxDegrees[0],
                evaluator.getAnnotation().getDataName(0),
                evaluator)));

    LAPP2_ << "Verifying stage " << verifyStage_ << " time(s)...";
    if (!detail::validatePolynomial<F,E>(
    		verifyStage_, univPartialPol, evaluator)) {
      throw std::runtime_error(
          "Intermediate polynomial did not match black box");
    }

    return univPartialPol->getPolynomial();
  }

  typedef std::multiset<
    boost::shared_ptr<algebra::PartialPolynomial<F> >,
    typename algebra::PartialPolynomial<F>::PtrOrder
  > PartialPolynomialSet;

  PartialPolynomialSet polPool(polOrderSortBy_);
  if (ppol) {  // Add passed in partial polynomial to the pool
    polPool.insert(ppol);
  }

  boost::timer tm;
  int maxTotalDegree = 0;
  maxNTerms = 1.0;
  for (int i = 0; i < nVars; ++i) {
    tm.restart();
    LAPP1_ << "Interpolating variable " << (i+1) << ". "
           << evaluator.getAnnotation().getDataName(i);
    // Create univariate evaluator out og multivariate by fixing all but one var
    eval::UnivEvaluator<F> ue(evaluator, i, evalPoints.get());
    std::vector<int> varIdx(1, i);  // univariate
    // Create partial univariate polynomial
    PartialPolynomialPtr univPartialPol(
      new algebra::PartialPolynomial<F>(
        nVars,            // Total number of variables
        evalPoints.get(), // Values to which variables have been fixed
        varIdx,           // Variable indices composing this polynomial
        newtonInterpolateUnivariateEvaluator<F>(  // New interpolated univ pol
            varMaxDegrees[i],  // Maximum degree
            evaluator.getAnnotation().getDataName(i),  // variable name
            ue)  // black box univariate evaluator
      )
    );
    LTRC_ << "  got polynomial " << *(univPartialPol->getPolynomial());
    maxTotalDegree += univPartialPol->getPolynomial()->getMaxDegree(0);
    maxNTerms *= (univPartialPol->getPolynomial()->getMaxDegree(0)
        - univPartialPol->getPolynomial()->getMinDegree(0) + 1);
    LAPP1_ << "  Max Deg = " << univPartialPol->getPolynomial()->getMaxDegree(0)
           << ", Min Deg = " << univPartialPol->getPolynomial()->getMinDegree(0)
           << ", nTerms  = "
           << univPartialPol->getPolynomial()->getNumberOfTerms()
           << " time taken: " << tm.elapsed();

    LAPP2_ << "Verifying stage " << verifyStage_ << " time(s)...";
    if (!detail::validatePolynomial<F, E>(
        verifyStage_, univPartialPol, evaluator)) {
      throw std::runtime_error(
          "Intermediate polynomial did not match black box");
    }

    polPool.insert(univPartialPol); // Add it to the pool of partial polynomials
  }
  LAPP_ << "Maximum total degree: " << maxTotalDegree;
  LAPP_ << "Maximum number of terms: " << maxNTerms;

  ASSERT1(polPool.size() > 0)(polPool.size())
    .msg("By now there should have been at least one polynomial in the pool");

  while (polPool.size() > 1) {  // Pool has more than one polynomial in it.
    LAPP1_ << "The following " << polPool.size()
           << " polynomials are in the pool ...";
    typename PartialPolynomialSet::const_iterator it = polPool.begin();
    for (; it != polPool.end(); ++it) {
      const algebra::PartialPolynomial<F>& ppol = *(*it);
      const algebra::Polynomial<F>& pol = *(ppol.getPolynomial());
      std::ostringstream ss;
      for (unsigned int i = 0; i < pol.getNumberOfVariables(); ++i) {
        if (i) ss << ",";
        ss << pol.getVarName(i);
      }
      LAPP1_ << "  [" << ss.str() << "]. "
             << pol.getNumberOfTerms() << " terms "
             << " tot deg " << pol.getTotDegree();
             // << " min tot deg " << pol.getMinTotalDegree();
             // LAPP1_ << p;
    }

    PartialPolynomialPtr polA = *(polPool.begin());
    polPool.erase(polPool.begin());

    PartialPolynomialPtr polB = *(polPool.begin());
    polPool.erase(polPool.begin());

    LAPP1_ << "Removed and merging two polynomials "
    	   << polA->getPolynomial()->getShortName()
           << " and " << polB->getPolynomial()->getShortName() << ". max size: "
           << polA->getPolynomial()->getNumberOfTerms()
              *polB->getPolynomial()->getNumberOfTerms() << " ...";

    PartialPolynomialPtr merged = sparseInterpolateStage(
        evaluator, *polA, *polB);

    int nTerms = merged->getPolynomial()->getNumberOfTerms();
    long double prob = static_cast<long double>(maxTotalDegree)
        * nTerms * (nTerms - 1) / (2*(F::getSize()-1));

    if (prob > 0.1) {
      LWRN_ << "Bound on probability of error is " << ((prob>1.0)?1.0:prob) ;
    } else {
      LAPP_ << "Bound on probability of error is " << prob;
    }

    LAPP2_ << "Verifying stage " << verifyStage_ << " time(s)...";
    if (!detail::validatePolynomial<F,E>(verifyStage_, merged, evaluator)) {
      throw std::runtime_error(
          "Intermediate polynomial did not match black box");
    }

    LAPP2_ << "Polynomial matched black box ... adding to the pool";
    polPool.insert(merged);
  }
  PartialPolynomialPtr pol = *(polPool.begin());
  LAPP2_ << "Verifying final " << verifyFinal_ << " time(s)...";
  if (!detail::validatePolynomial<F,E>(verifyFinal_, pol, evaluator)) {
    throw std::runtime_error("Final polynomial did not match black box");
  }
  return pol->getPolynomial();
}

template <typename F> template <typename E>
boost::shared_ptr<algebra::PartialPolynomial<F> >
Interpolator<F>::sparseInterpolateStage(
    E& evaluator,
    const algebra::PartialPolynomial<F>& ppolA,
    const algebra::PartialPolynomial<F>& ppolB) {
  // Extract important parameters
  const algebra::Polynomial<F>& polA = *(ppolA.getPolynomial());
  const algebra::Polynomial<F>& polB = *(ppolB.getPolynomial());

  int nVars = evaluator.getNumberOfVariables();
  int nTermsA = polA.getNumberOfTerms();
  int nTermsB = polB.getNumberOfTerms();

  int nVarsA = polA.getNumberOfVariables();
  int nVarsB = polB.getNumberOfVariables();

  std::ostringstream polAIdxStr;
  for (int i = 0; i < nVarsA; i++) {
    if (i) { polAIdxStr <<","; }
    polAIdxStr << ppolA.getVarIdx(i);
  }
  std::ostringstream polBIdxStr;
  for (int i = 0; i < nVarsB; i++) {
    if (i) { polBIdxStr <<",";  }
    polBIdxStr << ppolB.getVarIdx(i);
  }
  boost::timer tm;

  std::string firstAlgName = vf_->getAlgName(nTermsA,nTermsB-1);
  double firstAlgTime = complexity::Perf::predicted(
      firstAlgName, nTermsA, nTermsB - 1);
  LAPP1_ << "  Will be solving " << std::right << (nTermsB - 1)
         << " vand systems of size "
         << nTermsA << " x " << nTermsA << " with alg " << firstAlgName;
  LAPP1_ << "    predicted time " << firstAlgTime;
  const complexity::Equation* eqn = complexity::Perf::getEquation(firstAlgName);
  if (eqn) {
    LAPP1_ << "    equation " << *eqn;
  }
  std::string secondAlgName = vf_->getAlgName(nTermsB,nTermsA);
  double secondAlgTime = complexity::Perf::predicted(
      secondAlgName, nTermsB, nTermsA);
  LAPP1_ << "  and " << nTermsA << " of size " << nTermsB << " x " << nTermsB
         << " vand systems, with alg " << secondAlgName;
  LAPP1_ << "    predicted time " << secondAlgTime;
  eqn = complexity::Perf::getEquation(secondAlgName);
  if (eqn) {
    LAPP1_ << "    equation " << *eqn;
  }

  double evalTime = complexity::Perf::predicted(
      complexity::AlgEnum::VAL_OF_EVAL, nTermsA, 1);

  LAPP1_ << "  Will be evaluating black box "
         << nTermsA << " times, predicted time " << evalTime;
  eqn = complexity::Perf::getEquation(complexity::AlgEnum::VAL_OF_EVAL);
  if (eqn) {
    LAPP1_ << "    equation " << *eqn;
  }


  LAPP1_ << "Total predicted time for the stage "
         <<  (firstAlgTime + secondAlgTime +  evalTime);

  LAPP1_ << "  polA terms: " << nTermsA
         << " variables: (" << nVarsA << "): [" << polA.getShortName() << "]";
  LAPP2_ << "     indexes: [" << polAIdxStr.str() << "]";
  LAPP1_ << "  polB terms: " << nTermsB
         << " variables: (" << nVarsB << "): [" << polB.getShortName() << "]";
  LAPP2_ << "     indexes: [" << polBIdxStr.str() << "]";

  // Alocate a 2D grid for storing coefficients of polA at various choices for
  // variables of polB to do more efficient indexing allocate array
  // whose size is power of 2.
  unsigned int nTermsAPw2 = math::intlog(nTermsA); // 2^nTermsAPw2 >= nTermsA
  unsigned int nTermsBPw2 = math::intlog(nTermsB);

  LDBG_ << "Allocating memory " << (1 << (nTermsAPw2 + nTermsBPw2))
		    << " element coefficient grid";
  // Allocate a power of two for easier 2D indexing
  boost::scoped_array<F> coeffAGrid(new F[1 << (nTermsAPw2 + nTermsBPw2)]);
  // (i,j)th entry i<<nTermsBPw2+j has coefficient of jth term in polB
  // when polB variables are set to (i+1)st power

  // Setup array where polA is evaluated at variables of B
  unsigned int nVarsBPw2 = math::intlog(nVarsB);
  LDBG_ << "Allocating memory for " << (1 << (nVarsBPw2 + nTermsBPw2))
		    << " field elements";
  boost::scoped_array<F> valsB(new F[1 << (nVarsBPw2 + nTermsBPw2)]);

  // Fill in initial values since first coefficients already computed
  // (part of the input)
  //LAPP2_ << "polB base var values:";
  for (int j = 0; j < nVarsB; ++j) {
    valsB[j] = ppolA.getFixedVal(ppolB.getVarIdx(j));
    //LAPP2_ << "  " << j << ". " << polB.getVarName(j) << " " << valsB[j];
  }

  // First set of coeffs is already evaluated (part of the input)
  for (int tA = 0; tA < nTermsA; ++tA) {
    coeffAGrid[tA << nTermsBPw2] = polA.getCoeff(tA);
  }

  // Compute remaining by solving vandermonde systems

  // Allocate in one shot
  LDBG_ << "Allocating memory for " << (nTermsB << 1)
        << " Vandermode Matrix Entries ";
  // Vandermode Matrix Entries
  boost::scoped_array<F> vanEntrySetB(new F[nTermsB << 1]);
  //boost::scoped_array<F> vanEntryInvSetB(new F[nTermsB]);
  F* vanEntryInvSetB = vanEntrySetB.get() + nTermsB;

  // Precompute all values where polA will be evaluated at, later used
  // to interpolate copies of polB. valsB[i] = orig^i,
  // where orig was fixed val in polA
  for (int tB = 1; tB < nTermsB; ++tB) {
    for (int j = 0; j < nVarsB; ++j) {
      valsB[tB << nVarsBPw2 | j] = valsB[(tB - 1) << nVarsBPw2 | j] * valsB[j];
    }
  }
  for (int tB = 0; tB < nTermsB; ++tB) {
    vanEntrySetB[tB] = polB.monValue(tB, valsB.get());
    // Precompute inverses, as solution will need to be divided
    vanEntryInvSetB[tB] = vanEntrySetB[tB].getInverse();
    // Because starting evaluation points are not 1 (to improve randomization)
  }

  // Compute Vandermonde entries for polA
  boost::scoped_array<F> valsA(new F[nVarsA]);

  // Initialize eval points v to be primes.
  // evaluator will be evaluated at v^k for increasing values of k
  for (int i = 0; i < nVarsA; i++) {
    valsA[i] = F(math::primes[nVars+i]);
  }
  LDBG_ << "Allocating memory for " << (nTermsA << 1)
        << " vandermonde matrix entries";
  // Vandermode Matrix Entries
  boost::scoped_array<F> vanEntrySetA(new F[nTermsA << 1]);
  //boost::scoped_array<F> vanEntryInvSetA(new F[nTermsA]);
  F* vanEntryInvSetA = vanEntrySetA.get() + nTermsA;

  // Evaluate skeleton of first polynomial on chosen primes
  for (int tA = 0; tA < nTermsA; ++tA) {
    vanEntrySetA[tA] = polA.monValue(tA, valsA.get());
    // Precompute inverses, as solution will need to be divided,
    // Because starting evaluation points are not 1 (to improve randomization).
    vanEntryInvSetA[tA] = vanEntrySetA[tA].getInverse();
  }
  // Build Vandermonde solver to be used on Vandermode matrices of this
  // size with these entries.
  LAPP1_ << "Building Vandermonde Solver of size " << nTermsA;
  boost::shared_ptr<math::vandermonde::Solver<F> > vSolverA =
      vf_->newSolver(nTermsA, vanEntrySetA.get());

  // Remaining variables are held constant
  boost::scoped_array<F> pointSet(new F[nVars<<1]);
  //boost::scoped_array<F> fixedVals(new F[nVars]);
  F* fixedVals = pointSet.get()+nVars;
  for (int i = 0; i < nVars; i++) {
    fixedVals[i] = pointSet[i] = ppolA.getFixedVal(i);
  }

  // While holding remaining variables fixed, interpolate varIndex
  // variable at each coefficient of previous polynomial skeleton
  // Black box resulting values
  boost::scoped_array<F> vanValueSetA(new F[nTermsA << 1]);
  // Solution values for coefficients
  //boost::scoped_array<F> vanCoeffSetA(new F[nTermsA]);
  F* vanCoeffSetA = vanValueSetA.get()+nTermsA;

  //F* vals = varValues[varIndex];
  // For each term for this variable solve a for coefficients
  LAPP1_ << "  solving " << std::setw(5) << std::right << (nTermsB-1)
         << " vandermonde systems of size " << nTermsA << "x" << nTermsA;
//  LAPP1_ << "   predicted time "
//         << util::formatSecs(this->vf_->getPredictedTime(nTermsA,nTermsB-1));
  for (int tB = 1; tB < nTermsB; ++tB) {
    //LAPP2_ << "  setting up system " << tB << " of " << nTermsB;
    //LAPP2_ << "    Evaluating BB " << nTermsA << " times";
    ////LAPP2_ << "    evaluating ...";
    // Select next value for variables in B
    for (int j = 0; j < nVarsB; ++j) {
      pointSet[ppolB.getVarIdx(j)] = valsB[tB<<nVarsBPw2|j];
    }
    // Setup initial values for variables in A
    for (int i = 0; i < nVarsA; ++i) {
			pointSet[ppolA.getVarIdx(i)] = valsA[i];
    }
    //std::ostringstream sStr;
    //for (int i = 0; i < nVars; i++) {
    //  if (i) { sStr << ","; }
    //  sStr << pointSet[i];
    //}
    vanValueSetA[0] = evaluator(pointSet.get());  // Evaluate at first location
    //sStr << "=" << vanValueSetA[0];
    //LAPP2_ << "      " << sStr.str();
    // For remaining terms of prev polynomial,
    for (int tA = 1; tA < nTermsA; ++tA) {
      // Fill in previous variable values and evaluate hold this var
      // and subsequent constant.
      for (int i = 0; i < nVarsA; ++i) {
			  pointSet[ppolA.getVarIdx(i)] *= valsA[i];
      }
      //std::ostringstream sStr;
      //for (int i = 0; i < nVars; i++) {
      //  if (i) { sStr << ","; }
      //  sStr << pointSet[i];
      //}
      vanValueSetA[tA] = evaluator(pointSet.get());
      //sStr << "=" << vanValueSetA[tA];
      //LAPP2_ << "      " << sStr.str();
    }
    //LAPP2_ << "    solving " << nTermsA << "x"
    // << nTermsA << " vandermonde system...";
    vSolverA->solveTranspose(vanValueSetA.get(), vanCoeffSetA);
    // Copy solutions taking out extra factor
    //LAPP2_ << "    copying " << nTermsA << " coefficients...";
    for (int tA = 0; tA < nTermsA; ++tA) {
      // Divide out extra term, since initial value was not 1 but prime numbers.
			coeffAGrid[tA<<nTermsBPw2|tB] = vanCoeffSetA[tA] * vanEntryInvSetA[tA];
		}
  }
  /*
  LAPP2_ << "Solution grid ...";

  for (int i = 0; i < nTermsB; i++) {
    std::ostringstream ss;
    for (int j = 0; j < nVarsB; j++) {
      if (j) { ss << ","; }
      ss << valsB[i<<nVarsBPw2|j];
    }
    ss << "=";
    for (int j = 0; j < nTermsA; j++) {
      if (j) { ss << ","; }
       ss << coeffAGrid[j<<nTermsBPw2|i];
    }
    LAPP2_ << ss.str();
  }
  */
  LDBG_ << "  Done soving vandermonde systems for A";
  LDBG_ << "  Proceeding to get final coefficents of each polB";
  // After this solves contains the values polB take on

  //std::ostringstream ss;
  //for (int i = 0; i < nTermsB; i++) {
  //  if (i) { ss << ","; }
  //  ss << vanEntrySetB[i];
  //}
  //LAPP2_ << "vanEtrySetB = " << ss.str();

  // Build final polynomial
  std::vector<std::string> varNames(nVarsA+nVarsB);
  std::vector<int> varIdx(nVarsA+nVarsB);
  int i;
  for (i = 0; i < nVarsA; ++i) {
    varNames[i] = polA.getVarName(i);
    varIdx[i] = ppolA.getVarIdx(i);
  }
  for (int j = 0; j < nVarsB; ++j, ++i) {
    varNames[i] = polB.getVarName(j);
    varIdx[i] = ppolB.getVarIdx(j);
  }

  boost::shared_ptr<algebra::Polynomial<F> > pol(
      new algebra::Polynomial<F>(varNames, nTermsA*nTermsB));

  // Build Vandermonde solver to be used on Vandermode matrices
  // of this size with these entries
  LAPP1_ << "Building Vandermonde Solver of size " << nTermsB;
  boost::shared_ptr<math::vandermonde::Solver<F> > vSolverB
      = vf_->newSolver(nTermsB, vanEntrySetB.get());
  boost::scoped_array<F> vanCoeffSetB(new F[nTermsB]);

  int t = 0;
  LAPP1_ << "  solving " << std::setw(5) << std::right << nTermsA << " "
         << std::setw(4) << std::right << nTermsB
         << "x" << std::setw(4) << std::left << nTermsB << " vand systems";
//  LAPP1_ << "  predicted time "
//         << util::formatSecs(this->vf_->getPredictedTime(nTermsB,nTermsA));
  for (int tA = 0; tA < nTermsA; ++tA) {
    vSolverB->solveTranspose(&coeffAGrid[tA << nTermsBPw2], vanCoeffSetB.get());
		for (int tB = 0; tB < nTermsB; ++tB, ++t) {
      pol->setCoeff(t, vanCoeffSetB[tB] * vanEntryInvSetB[tB]);
      for (i = 0; i < nVarsA; ++i) {
        pol->setDegree(t,i,polA.getDegree(tA, i));
      }
      for (int j = 0; j < nVarsB; ++j, ++i) {
        pol->setDegree(t,i,polB.getDegree(tB, j));
      }
    }
	}
  pol->removeZeroTerms();

  LAPP1_ << "  Merged polynomial ... "
         << " Tot Deg = " << pol->getTotDegree()
         << " nTerms  = " << pol->getNumberOfTerms()
         << " in " << tm.elapsed() << " secs";

  return boost::shared_ptr<algebra::PartialPolynomial<F> > (
      new algebra::PartialPolynomial<F>(nVars, fixedVals, varIdx, pol));
}

}  // namespace interp
#undef LOG_MODULE
