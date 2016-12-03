#ifndef NTRP_RUN_FIELD_BENCHMARKS_H_
#define NTRP_RUN_FIELD_BENCHMARKS_H_

/**
 * @file FieldBenchmarks.h Test performance of given field on various algorithms
 */

#include "complexity/Equation.h"
#include "complexity/Estimator.h"
#include "complexity/AlgEnum.h"
#include "complexity/Perf.h"
#include "operation/field/benchmark.h"
#include "operation/vandermonde/benchmark.h"
#include "util/log.h"
#include "util/MultiIndexMap.h"

#include <iomanip>
#include <sstream>

#define LOG_MODULE ::logging::LogModuleEnum::RUN

namespace run {

class FieldBenchmarks {
public:
  template <typename BENCHMARKER>
  void runFieldOperation(complexity::AlgEnum alg, util::MultiIndexMap* mmap) {
    util::MultiIndexMap& m = *mmap; // Reference is more convenient

    const double MAX_FIELD_TEST_TIME = 5.0; // seconds
    //typedef typename complexity::Estimator<T> FieldOperationEstimator;
    //FieldOperationEstimator estField;
    //estField.run(MAX_FIELD_TEST_TIME);
    {
      using namespace complexity;
      Perf::registerAlg<LinearEquation>(alg.toString());

      Perf::exercise(alg.toString(),BENCHMARKER(),MAX_FIELD_TEST_TIME);
      const Equation* eq = Perf::getEquation(alg.toString());

      if (eq) {
        LAPP1_ << alg.toString() << ": " << std::fixed << std::setprecision(4)
               << eq->solveFor(1.0) << " ops/sec";
        LAPP1_ << "  equation " << std::scientific << *eq;
        m["equation"] = *eq;
        m["evals_sec"] =  eq->solveFor(1.0);
      } else {
        LWRN_ << "equation " << alg.toString() << " is NULL";
      }
    }
  }

#ifdef FIELD_DYNAMIC32
  int runDynamic32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_A32
  int runA32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_B32
  int runB32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_C32
  int runC32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_AB32
  int runAB32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_BC32
  int runBC32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_AC32
  int runAC32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_CRT32
  int runCRT32(util::MultiIndexMap* m);
#endif
#ifdef FIELD_DYNAMIC64
  int runDynamic64(util::MultiIndexMap* m);
#endif
#ifdef FIELD_A64
  int runA64(util::MultiIndexMap* m);
#endif
#ifdef FIELD_B64
  int runB64(util::MultiIndexMap* m);
#endif
#ifdef FIELD_C64
  int runC64(util::MultiIndexMap* m);
#endif
#ifdef FIELD_AB64
  int runAB64(util::MultiIndexMap* m);
#endif
#ifdef FIELD_BC64
  int runBC64(util::MultiIndexMap* m);
#endif
#ifdef FIELD_AC64
  int runAC64(util::MultiIndexMap* m);
#endif
#ifdef FIELD_CRT64
  int runCRT64(util::MultiIndexMap* m);
#endif


  int run(util::MultiIndexMap* mmap);

  template <typename F>
  int runWithField(util::MultiIndexMap* mmap) {
//    const double MAX_FIELD_TEST_TIME = 2.0; // seconds

    util::MultiIndexMap& m = (*mmap)[F::getId()];
    m["sizeof"] = sizeof(F);
    unsigned int order = math::field::getFourierOrder<F>();
    LAPP_ << "Field Fourier order " << order;
    m["fourier_order"] = order;
    m["size"] = F::getSize();

    runFieldOperation<operation::field::Add<F> >(complexity::AlgEnum::FIELD_ADD, &(m["operation"]["add"]));
    runFieldOperation<operation::field::Subtract<F> >(complexity::AlgEnum::FIELD_SUB, &(m["operation"]["sub"]));
    runFieldOperation<operation::field::Multiply<F> >(complexity::AlgEnum::FIELD_MUL, &(m["operation"]["mul"]));
    runFieldOperation<operation::field::Divide<F> >(complexity::AlgEnum::FIELD_DIV, &(m["operation"]["div"]));
    runFieldOperation<operation::field::Inverse<F> >(complexity::AlgEnum::FIELD_INV, &(m["operation"]["inv"]));

    const double BENCHMARK_TIME_LIMIT = 2.0;
    {
      using namespace complexity;
      LAPP1_ << "Benchmarking " << AlgEnum::VAND_QUAD_SETUP << " for " << BENCHMARK_TIME_LIMIT << " sec";
      Perf::exercise(AlgEnum::VAL_OF_VAND_QUAD_SETUP,operation::vandermonde::QuadSetup<F>(),BENCHMARK_TIME_LIMIT);
      const Equation* eq = Perf::getEquation(AlgEnum::VAL_OF_VAND_QUAD_SETUP);
      if (eq) {
        double timeToEval = eq->evaluateAt(1000000);
        LAPP1_ << "Vandermonde Quad setup for size 1000000 in " << std::fixed << std::setprecision(4)
               << timeToEval << " secs equation " << std::scientific << *eq;
        m["vandermonde"]["setup"]["quad"]["equation"] = *eq;
      } else {
        LWRN_ << "  equation NULL";
        m["vandermonde"]["setup"]["quad"]["equation"] = "NULL";
      }
    }

    //typedef complexity::Estimator<operation::vandermonde::QuadSolve<F> > QuadSolveEstimator;
    //QuadSolveEstimator estQuadSolve;
    //estQuadSolve.run(2*MAX_FIELD_TEST_TIME);

    //timeToEval = QuadSolveEstimator::getComplexityEquation().evaluateAt(1000000);
    //LAPP1_ << "Vandermonde Quad solve for size 1000000 in " << std::fixed << std::setprecision(4)
    //       << timeToEval << " secs equation " << std::scientific << QuadSolveEstimator::getComplexityEquation();

    //m["vandermonde"]["solve"]["quad"]["equation"] = QuadSolveEstimator::getComplexityEquation();

    //typedef complexity::Estimator<operation::vandermonde::FFTSetup<F> > FFTSetupEstimator;
    //FFTSetupEstimator estFFTSetup;
    //estFFTSetup.run(2*MAX_FIELD_TEST_TIME);

    //timeToEval = FFTSetupEstimator::getComplexityEquation().evaluateAt(1000000);
    //LAPP1_ << "Vandermonde FFT setup for size 1000000 in " << std::fixed << std::setprecision(4)
    //       << timeToEval << " secs equation " << std::scientific << FFTSetupEstimator::getComplexityEquation();

    //m["vandermonde"]["setup"]["fft"]["equation"] = FFTSetupEstimator::getComplexityEquation();

    //typedef complexity::Estimator<operation::vandermonde::FFTSolve<F> > FFTSolveEstimator;
    //FFTSolveEstimator estFFTSolve;
    //estFFTSolve.run(2*MAX_FIELD_TEST_TIME);

    //timeToEval = FFTSolveEstimator::getComplexityEquation().evaluateAt(1000000);
    //LAPP1_ << "Vandermonde FFT solve for size 1000000 in " << std::fixed << std::setprecision(4)
    //       << timeToEval << " secs equation " << std::scientific << FFTSolveEstimator::getComplexityEquation();

    //m["vandermonde"]["solve"]["fft"]["equation"] = FFTSolveEstimator::getComplexityEquation();

    //long double intersectionPoint
    //  = complexity::Equation::intersectsAt(FFTSetupEstimator::getComplexityEquation(),
    //                         QuadSetupEstimator::getComplexityEquation());
    //LAPP1_ << "FFT Setup intersects Quad at " << intersectionPoint;

    //m["vandermonde"]["setup"]["fft_quad_intersection"] = intersectionPoint;
    //LAPP1_ << "FFT setup time "
    //       << FFTSetupEstimator::getComplexityEquation().evaluateAt(intersectionPoint)
    //       << " Quad setup time "
    //       << QuadSetupEstimator::getComplexityEquation().evaluateAt(intersectionPoint);

    //intersectionPoint
    //  = complexity::Equation::intersectsAt(FFTSolveEstimator::getComplexityEquation(),
    //                         QuadSolveEstimator::getComplexityEquation());
    //LAPP1_ << "FFT solve intersects Quad at " << intersectionPoint;
    //m["vandermonde"]["solve"]["fft_quad_intersection"] = intersectionPoint;
    //LAPP1_ << "FFT solve time "
    //       << FFTSolveEstimator::getComplexityEquation().evaluateAt(intersectionPoint)
    //       << " Quad solve time "
    //       << QuadSolveEstimator::getComplexityEquation().evaluateAt(intersectionPoint);
    return 0;
  }

}; // class FieldBenchmarks
} // namespace run

#undef LOG_MODULE

#endif  // NTRP_RUN_FIELD_BENCHMARKS_H_
