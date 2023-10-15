#ifndef NTRP_RUN_INTERPOLATION_H_
#define NTRP_RUN_INTERPOLATION_H_

/**
 * @file Interpolation.h Given interpolation profile instantiates
 *       and runs interpolator
 */

#include "complexity/AlgEnum.h"
#include "complexity/Perf.h"
#include "eval/CodeGenerator.h"
#include "eval/util/InstructionCounter.h"
#include "interp/Interpolator.h"
#include "interp/VandSolveTypeEnum.h"
#include "math/util/random.h"
#include "operation/eval/benchmark.h"
#include "operation/vandermonde/benchmark.h"
#include "runprofile/InterpProfile.h"
#include "util/MultiIndexMap.h"

#include <boost/shared_ptr.hpp>
#include <ctime>  // std::time
#include <fstream>

#define LOG_MODULE ::logging::LogModuleEnum::RUN

namespace run {

/**
 * @brief Instantiate templeted interpolation structures for particualar field
 *        specified by interpolation profile, and start interpolation process,
 *        for a given black box specified as syntax tree (InterpContext)
 *        During execution InterpContext is compiled into bytecode.
 *        Instantiation over different field is split in separate compilation
 * units, this somewhat reduces compilation error messages since they do not
 * repeat for each instantiation
 */
class Interpolation {
  boost::shared_ptr<runprofile::InterpProfile const> interpProfile_;

 public:
  Interpolation(
      const boost::shared_ptr<runprofile::InterpProfile const>& interpProfile)
      : interpProfile_(interpProfile){};
#ifdef FIELD_DYNAMIC32
  int runDynamic32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
                   util::MultiIndexMap* m);
#endif
#ifdef FIELD_A32
  int runA32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
             util::MultiIndexMap* m);
#endif
#ifdef FIELD_B32
  int runB32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
             util::MultiIndexMap* m);
#endif
#ifdef FIELD_C32
  int runC32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
             util::MultiIndexMap* m);
#endif
#ifdef FIELD_AB32
  int runAB32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
              util::MultiIndexMap* m);
#endif
#ifdef FIELD_BC32
  int runBC32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
              util::MultiIndexMap* m);
#endif
#ifdef FIELD_AC32
  int runAC32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
              util::MultiIndexMap* m);
#endif
#ifdef FIELD_CRT32
  int runCRT32(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
               util::MultiIndexMap* m);
#endif
#ifdef FIELD_DYNAMIC64
  int runDynamic64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
                   util::MultiIndexMap* m);
#endif
#ifdef FIELD_A64
  int runA64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
             util::MultiIndexMap* m);
#endif
#ifdef FIELD_B64
  int runB64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
             util::MultiIndexMap* m);
#endif
#ifdef FIELD_C64
  int runC64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
             util::MultiIndexMap* m);
#endif
#ifdef FIELD_AB64
  int runAB64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
              util::MultiIndexMap* m);
#endif
#ifdef FIELD_BC64
  int runBC64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
              util::MultiIndexMap* m);
#endif
#ifdef FIELD_AC64
  int runAC64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
              util::MultiIndexMap* m);
#endif
#ifdef FIELD_CRT64
  int runCRT64(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
               util::MultiIndexMap* m);
#endif

  /**
   * @brief Entry point to interpolation engine. This function instantiates
   * the rest of templeted code dependend on field.
   */
  template <typename F>
  int runWithField(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
                   util::MultiIndexMap* mmap) {
    util::MultiIndexMap& m = *mmap;  // Reference is more convenient

    // == Collect statistics about this field =============
    {
      LAPP1_ << "Field element stack memory footprint is " << sizeof(F)
             << " bytes.";

      m["field"]["name"] = F::getName();
      m["field"]["sizeof"] = sizeof(F);
      LAPP2_ << "Field has " << std::fixed << std::setprecision(2)
             << F::getSize() << " or ~2^"
             << log(static_cast<long double>(F::getSize())) / log(2.0L)
             << " number of elements";
      m["field"]["size"] = F::getSize();

      unsigned int order = math::field::getFourierOrder<F>();

      LAPP_ << "Field Fourier order " << order;
      m["field"]["fourier_order"] = order;
    }

    // == Set random Seed =================================
    {
      typename F::ubase_type seed;
      if (interpProfile_->getRandomSeed()) {
        seed = *(interpProfile_->getRandomSeed());
      } else {
        seed = static_cast<typename F::ubase_type>(std::time(0));
      }
      math::random<typename F::ubase_type>::setSeed(seed);
      LAPP1_ << "Set random seed to " << seed;
      m["seed"] = seed;
    }

    // == Compile input into black box evaluator ==========
    LAPP1_ << "Compiling statements into bytecode...";

    eval::CodeGenerator<F> cg(ctxtPtr);
    boost::shared_ptr<eval::Evaluator<F> > evalPtr = cg.getEvaluator();
    if (!evalPtr) {
      LERR_ << "Not able to build evaluator from parsed in context";
      return 1;
    }
    eval::Evaluator<F>& bbeval = *evalPtr;

    // -- Write out black box to a file -------------------
    const boost::optional<std::string>& asmFileName =
        interpProfile_->getAsmFileName();
    if (asmFileName) {
      LAPP1_ << "Writing bytecode assembly file '" << *(asmFileName) << "'";
      std::ofstream ofile(asmFileName->c_str(), std::ios::out);
      if (ofile.fail()) {
        LERR_ << "Error opening file '" << *(asmFileName) << "' for writing.";
        return 1;
      }
      // Trace evaluator writing output to a file
      eval::InstructionPrinter ip(ofile, bbeval.getAnnotation());
      bbeval.traceWith(ip);
      ofile.close();
    } else {
      LAPP2_ << "No assembly file specified, skipping bytecode dump.";
    }

    // -- Collect some statistics about black box ---------
    eval::InstructionCounter<F> counter;
    bbeval.traceWith(counter);

    util::MultiIndexMap& vm = m["evaluator"]["instructions"];
    int totNumInstr = bbeval.getCodeSize();
    LAPP2_ << "Instructions used by the evaluator (total " << totNumInstr
           << "):";
    for (int i = 0; i < eval::InstructionEnum::NUM_ENUMS; ++i) {
      eval::InstructionEnum instrType(i);
      int count = counter.getCount(instrType);
      if (count > 0) {
        util::MultiIndexMap minstr;
        minstr["name"] = instrType;
        minstr["occurs"] = count;
        vm += minstr;

        LAPP2_ << "  " << std::setw(7) << instrType << " - " << std::setw(5)
               << std::right << count << " times (" << std::right
               << std::setw(5) << std::fixed << std::setprecision(2)
               << count * 100.0 / totNumInstr << "%)";
      }
    }

    int nVars = bbeval.getNumberOfVariables();
    m["evaluator"]["variable"]["unbounded"] = nVars;
    LAPP1_ << "Number of unbound variables is " << nVars;
    LAPP2_ << "Data block size is " << bbeval.getDataSize();
    m["evaluator"]["datasize"] = bbeval.getDataSize();
    LAPP2_ << "Code block size is " << bbeval.getCodeSize();
    m["evaluator"]["codesize"] = bbeval.getCodeSize();

    LAPP1_ << "Checking if interpolant is identically zero ...";
    {  // -- check if evaluator is identically zero --------
      const int NUM_CHECK_FOR_ZERO = 100;
      boost::shared_ptr<F> pointSet(F::alloc(nVars), F::dealloc);
      int check;
      boost::timer tm;
      for (check = 0; check < NUM_CHECK_FOR_ZERO; ++check) {
        for (int i = 0; i < nVars; ++i) {
          pointSet.get()[i].setTo(math::primes[check * nVars + i]);
        }
        F result = bbeval(pointSet.get());
        if (!result.template is<0>()) break;
      }
      // double t =  tm.elapsed();
      if (check >= NUM_CHECK_FOR_ZERO) {
        std::ostringstream oss;
        oss << "The input evaluates to zero (" << NUM_CHECK_FOR_ZERO
            << " times check). Exiting...";
        LERR_ << oss.str();
        throw std::runtime_error(oss.str());
      } else {
        LAPP1_ << "  no. Found non zero point on try " << (check + 1);
      }
    }
    // == Benchmark main used algorithms ==================
    {
      using namespace complexity;
      Perf::registerAlg<BigOQuadEquation>(AlgEnum::VAL_OF_VAND_QUAD_SETUP);
      Perf::registerAlg<BigOQuadEquation>(AlgEnum::VAL_OF_VAND_QUAD_SOLVE);

      Perf::registerAlg<BigONLogN2Equation>(AlgEnum::VAL_OF_VAND_FFT_SETUP);
      Perf::registerAlg<BigONLogN2Equation>(AlgEnum::VAL_OF_VAND_FFT_SOLVE);

      Perf::registerAlg<BigOLinearEquation>(AlgEnum::VAL_OF_EVAL);

      const double BENCHMARK_TIME_LIMIT = 2.0;
      {
        LAPP1_ << "Benchmarking " << AlgEnum::VAND_QUAD_SETUP << " for "
               << BENCHMARK_TIME_LIMIT << " sec";
        Perf::exercise(AlgEnum::VAL_OF_VAND_QUAD_SETUP,
                       operation::vandermonde::QuadSetup<F>(),
                       BENCHMARK_TIME_LIMIT);
        const complexity::Equation* eq =
            Perf::getEquation(AlgEnum::VAL_OF_VAND_QUAD_SETUP);
        if (eq) {
          LAPP1_ << "  equation " << *eq;
          m["vandermonde"]["quad"]["setup"]["speed"]["equation"] = *eq;
        } else {
          LWRN_ << "  equation NULL";
          m["vandermonde"]["quad"]["setup"]["speed"]["equation"] = "NULL";
        }
      }
      {
        LAPP1_ << "Benchmarking " << AlgEnum::VAND_QUAD_SOLVE << " for "
               << BENCHMARK_TIME_LIMIT << " sec";
        Perf::exercise(AlgEnum::VAL_OF_VAND_QUAD_SOLVE,
                       operation::vandermonde::QuadSolve<F>(),
                       BENCHMARK_TIME_LIMIT);
        const complexity::Equation* eq =
            Perf::getEquation(AlgEnum::VAL_OF_VAND_QUAD_SOLVE);
        if (eq) {
          LAPP1_ << "  equation " << *eq;
          m["vandermonde"]["quad"]["solve"]["speed"]["equation"] = *eq;
        } else {
          LWRN_ << "  equation NULL";
          m["vandermonde"]["quad"]["solve"]["speed"]["equation"] = "NULL";
        }
      }
      {
        LAPP1_ << "Benchmarking " << AlgEnum::VAND_FFT_SETUP << " for "
               << BENCHMARK_TIME_LIMIT << " sec";
        Perf::exercise(AlgEnum::VAL_OF_VAND_FFT_SETUP,
                       operation::vandermonde::FFTSetup<F>(),
                       BENCHMARK_TIME_LIMIT);
        const complexity::Equation* eq =
            Perf::getEquation(AlgEnum::VAL_OF_VAND_FFT_SETUP);
        if (eq) {
          LAPP1_ << "  equation " << *eq;
          m["vandermonde"]["fft"]["setup"]["speed"]["equation"] = *eq;
        } else {
          LWRN_ << "  equation NULL";
          m["vandermonde"]["fft"]["setup"]["speed"]["equation"] = "NULL";
        }
      }
      {
        LAPP1_ << "Benchmarking " << AlgEnum::VAND_FFT_SOLVE << " for "
               << BENCHMARK_TIME_LIMIT << " sec";
        Perf::exercise(AlgEnum::VAL_OF_VAND_FFT_SOLVE,
                       operation::vandermonde::FFTSolve<F>(),
                       BENCHMARK_TIME_LIMIT);
        const complexity::Equation* eq =
            Perf::getEquation(AlgEnum::VAL_OF_VAND_FFT_SOLVE);
        if (eq) {
          LAPP1_ << "  equation " << *eq;
          m["vandermonde"]["fft"]["solve"]["speed"]["equation"] = *eq;
        } else {
          LWRN_ << "  equation NULL";
          m["vandermonde"]["fft"]["solve"]["speed"]["equation"] = "NULL";
        }
      }
      {
        LAPP1_ << "Benchmarking " << AlgEnum::EVAL << " for "
               << BENCHMARK_TIME_LIMIT << " sec";
        Perf::exercise(AlgEnum::VAL_OF_EVAL,
                       operation::eval::Evaluator<F>(bbeval), 2.0);
        const complexity::Equation* eq =
            Perf::getEquation(AlgEnum::VAL_OF_EVAL);
        if (eq) {
          LAPP1_ << "  equation " << *eq;
          m["evaluator"]["speed"]["equation"] = *eq;
        } else {
          LWRN_ << "  equation NULL";
          m["evaluator"]["speed"]["equation"] = "NULL";
        }
      }
    }

    // == Create and run interpolator on black box ========
    interp::Interpolator<F> interp(
        getVandermondeFactory<F>(), getInterpPolynomialOrder<F>(),
        interpProfile_->getPrunningChoice(), interpProfile_->getVerifyStage(),
        interpProfile_->getVerifyFinal());

    LAPP1_ << "--- Interpolating ---";
    // Interpolate the evaluator
    boost::shared_ptr<algebra::Polynomial<F> const> pol =
        interp.template interpolate<eval::Evaluator<F> >(bbeval);

    LAPP_ << "Final polynomial has " << pol->getNumberOfTerms() << " term(s)";
    m["result"]["nterms"] = pol->getNumberOfTerms();
    m["result"]["polynomial"] = *pol;
    return 0;
  }

  int run(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
          util::MultiIndexMap* m);

  /**
   * @brief Return polynomial ordering functor
   */
  template <typename F>
  typename algebra::PartialPolynomial<F>::OrderByFunctor
  getInterpPolynomialOrder() const {
    switch (interpProfile_->getVarOrder().getId()) {
      case interp::VarOrderEnum::MAX_DEGREE:
        return typename algebra::PartialPolynomial<F>::MaxDegOrder();
      case interp::VarOrderEnum::MIN_DEGREE:
        return typename algebra::PartialPolynomial<F>::MinDegOrder();
      case interp::VarOrderEnum::MIN_TERMS:
        return typename algebra::PartialPolynomial<F>::MinTermsOrder();
      case interp::VarOrderEnum::ORIGINAL:
        return typename algebra::PartialPolynomial<F>::OriginalOrder();
      case interp::VarOrderEnum::PREDICT_NUM_EVAL_POINTS:
      case interp::VarOrderEnum::MAX_TERMS:
      default:
        break;
    }
    return typename algebra::PartialPolynomial<F>::MaxTermsOrder();
  }

  /**
   * @brief Return vandermonde factory based on interpolation profile
   */
  template <typename F>
  boost::shared_ptr<interp::VandSolverFactory<F> > getVandermondeFactory() {
    switch (interpProfile_->getVandermondeSolver().getId()) {
      case interp::VandSolveTypeEnum::QUAD:
        return boost::shared_ptr<interp::VandSolverFactory<F> >(
            new interp::VandSolverFactory<F>(interp::VandSolveTypeEnum::QUAD));
      case interp::VandSolveTypeEnum::FFT:
        return boost::shared_ptr<interp::VandSolverFactory<F> >(
            new interp::VandSolverFactory<F>(interp::VandSolveTypeEnum::FFT));
      case interp::VandSolveTypeEnum::BEST:
      default:
        break;
    }
    return boost::shared_ptr<interp::VandSolverFactory<F> >(
        new interp::VandSolverFactory<F>(interp::VandSolveTypeEnum::BEST));
  }
};  // class Interpolation

}  // namespace run

#undef LOG_MODULE

#endif  // NTRP_RUN_INTERPOLATION_H_
