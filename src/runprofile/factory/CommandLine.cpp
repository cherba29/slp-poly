/**
 * @file CommandLine.cpp Implementation of RunProfile factory from command line
  */

#include "runprofile/InterpProfile.h"
#include "runprofile/factory/CommandLine.h"
#include "util/SmartAssert.h"

#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/static_assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/type_traits/has_right_shift.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

namespace po = boost::program_options;

namespace {
/* Auxiliary functions for checking input for validity. */

/**
 * Function used to check that 'opt1' and 'opt2' are not specified
 * at the same time.
 */
inline bool both_options(const po::variables_map& vm,
                         const char* opt1, const char* opt2) {
 return (vm.count(opt1) && !vm[opt1].defaulted()
        && vm.count(opt2) && !vm[opt2].defaulted());
}

/**
 * Check that at least one of the optioins is specified
 */
bool atLeastOne(const po::variables_map& vm,
    const char* options[], unsigned int nOptions) {
  for (unsigned int i = 0; i < nOptions; ++i) {
    if (vm.count(options[i]) && !vm[options[i]].defaulted()) {
      return true;
    }
  }
  return false;
}

/**
 * Check if any two simultaneous options are specified.
 */
bool exclusiveOptions(
  const po::variables_map& vm,
  const char* options[], unsigned int nOptions) {
  for (unsigned int i = 0; i < nOptions; i++) {
    for (unsigned int j = i+1; j < nOptions; j++) {
      if (both_options(vm, options[i], options[j]))
        throw std::logic_error(std::string("Conflicting options '")
                          + options[i] + "' and '" + options[j] + "'.");
    }
  }
  return false;
}

/**
 * Check if given option and one of the other specified options are set.
 */
bool conflicting_options(
    const po::variables_map& vm,
    const char opt1[], const char* options[], unsigned int nOptions) {
  if (vm.count(opt1) && !vm[opt1].defaulted()) {
    for (unsigned int i = 0; i < nOptions; i++) {
      if (vm.count(options[i]) && !vm[options[i]].defaulted())
        return true;
    }
  }
  return false;
}

/**
 * Function used to check that of 'for_what' is specified, then
 * 'required_option' is specified too.
 */
inline bool option_dependency(const po::variables_map& vm,
                        const char* for_what, const char* required_option) {
  return (vm.count(for_what) && !vm[for_what].defaulted())
    && (vm.count(required_option) == 0 || vm[required_option].defaulted());
}

/**
 * Extra parser to allow @ for response file specification
 * @param s - parameter/command line option string
 * @return option/value pair
 */
std::pair<std::string, std::string> at_option_parser(const std::string& s) {
  if ('@' == s[0]) {
    return std::make_pair(std::string("response-file"), s.substr(1));
  }
  else {
    return std::pair<std::string, std::string>();
  }
}

}  // anonymous namespace

/*
void validate(boost::any& v,
              const std::vector<std::string>& values,
              std::pair< ::logging::Module, ::logging::Level>* target_type,
              int)
{
  const std::string& s = po::validators::get_single_string(values);

  size_t pos = s.find_first_of('=');
  if ( pos != std::string::npos) {
    std::string moduleStr = s.substr(0,pos);
    std::string logLevelStr = s.substr(pos);
    std::pair< ::logging::Module,::logging::Level> modLogLevel(moduleStr, logLevelStr);
    if (modLogLevel.first == ::logging::Module::UNKNOWN) {
      throw std::logic_error(std::string("Unknown module: ")+moduleStr);
    }
    if (modLogLevel.second == ::logging::Level::UNKNOWN) {
      throw std::logic_error(std::string("Unknown logging level: ")+logLevelStr);
    }
    v = boost::any(modLogLevel);
  } else {
    throw po::validation_error("invalid log value");
  }
}
*/

// validate above does not work, because of boost::program_options assumption
// that one can specialize/override a function template
// Instead defining this function to make boost::lexical_cast to work
//typedef std::pair< ::logging::LogModuleEnum, ::logging::LogLevelEnum> ModLogLevel;

class ModLogLevel {
public:
	::logging::LogModuleEnum log_module;
	::logging::LogLevelEnum log_level;
};

std::istream& operator>>(std::istream& is, ModLogLevel& modLogLevel) {
  std::string s;
  is >> s;
  size_t pos = s.find_first_of('=');
  if (pos != std::string::npos) {
    ::logging::LogModuleEnum module;
    module.setToValue(s.substr(0,pos).c_str());
    if (!module || module == ::logging::LogModuleEnum::UNKNOWN) {
      throw std::logic_error(std::string("Unknown module: ") + s.substr(0,pos));
    }
    ::logging::LogLevelEnum level;
    level.setToValue(s.substr(pos+1).c_str());

    if (!level || level == ::logging::LogLevelEnum::UNKNOWN) {
      throw std::logic_error(std::string("Unknown logging level: ")
          + s.substr(pos+1));
    }
	//modLogLevel = ModLogLevel(module, level);
	modLogLevel.log_module = module;
	modLogLevel.log_level = level;
  } else {
    throw po::validation_error(po::validation_error::invalid_option_value,
        s, "module logging level, expecting module=level");
  }
  return is;
};


namespace runprofile {
namespace factory {

std::unique_ptr<RunProfile> CommandLine::getRunProfile(
    int argc,  char *argv[]) {
  // Boolean flags
  bool h_given = false;
  bool v_given = false;
  bool w_given = false;
  bool m_given = false;
  bool b_given = false;
  bool t_given = false;

  // Respective choices
  std::string orderStr;
  std::vector<ModLogLevel> logLevels;
  std::string univarStr;
  std::string vandStr;
  std::string pruneStr;
  std::string fieldOptionStr;
  std::string logTags;

  po::options_description mainOptions("Main (mutually exclusive) Options");
  mainOptions.add_options()
    ("help,h", po::bool_switch(&h_given),
        "produce this help message. "
        "Use in conjunction with other options to get more help,"
        " e.g. -h -t will show interpolation options")
    ("version,v", po::bool_switch(&v_given),
        "print version string")
    ("machine,m", po::bool_switch(&m_given),
        "Output machine information into given filename.")
    ("benchmark,b", po::bool_switch(&b_given),
        "Only run benchmarks and save into given filename.")
    ("interpolate,t", po::bool_switch(&t_given),
        "Interpolate problem given in the input file, "
        "saving in the given output file. run -ht option to see more info.")
  ;

  po::options_description genOptions("General Options");
  genOptions.add_options()
    ("level,l",
        po::value<std::vector<ModLogLevel> >(&logLevels)->composing(),
        "<module name>=<log level>. Log levels:\n"
        "  trace - \tVery detailed program execution trace. Includes all other level.\n"
        "  debug - \tLess detailed than trace, used by key complex areas of code.\n"
        "  info  - \t(default) Application level/domain specific messages.\n"
        "  inf1  - \tMore detailed application messages, shows all info as well.\n"
        "  inf2  - \tEven more detailed application messages, shows all info and info1.\n"
        "  warning - \tWarnings.\n"
        "  error - \t Only show severe problems.\n"
        "  fatal - \t Fatal errors.\n"
        "  none - \tNo tracing not even errors.")
    ("log-tags", po::value<std::string>(&logTags)->default_value("itml"),
        "Show various tags in the log\n"
        "i -\t Incrementing index (first default)\n"
        "t -\t Running timer (second default)\n"
        "m -\t Module name (third default)\n"
        "l -\t Log Level name (see level option)\n"
        "n -\t File name and line number\n"
        "f -\t C++ Function name (too verbose)\n"
        "h -\t Thread id")
    ("response-file", po::value<std::string>(),
        "Config file with same syntax as command line. "
        "Can be specified with '@name', too.")
    ("output-file,o", po::value<std::string>(),
        "Output file to save results in yaml format.\n"
        "If not specified the output will be dumped to"
        "the terminal at the end of execution")
    ("overwrite,w", po::bool_switch(&w_given),
        "overwrite output file if it exists")
//    ("trace", po::value<std::string>(),
//        "Save trace, debug and error level messages into specified file.")
//    ("runtime", po::value<std::string>(),
//        "Save application level messages into specified file. Will enable info log level")
  ;

  // Declare interpolation supported options.
  int verifyStage;
  int verifyFinal;
  std::ostringstream usageMsg;
  usageMsg << "Interpolation\n  usage: " << argv[0]
           << " -t [options] <input file> <output file>\n\n"
              "where [options] are a combination of any of the general options "
              "and of the following";

  po::options_description interpOptions(usageMsg.str());
  interpOptions.add_options()
    ("asmfile,a", po::value<std::string>(),
        "Save compiled input into assembly file.")
    ("field,f", po::value<std::string>(&fieldOptionStr)
        ->default_value(FieldOptionEnum(1).getName()),
        "Specify field to use for interpolation\n"
#ifdef __LP64__
        "  <"
#ifdef FIELD_A64
            "A"
#if defined(FIELD_B64) || defined(FIELD_C64)
            "|"
#endif // B64 or C64
#endif // FIELD_A64
#ifdef FIELD_B64
            "B"
#if defined(FIELD_C64)
            "|"
#endif // FIELD_C64
#endif // FIELD_B64
#ifdef FIELD_C64
            "C"
#endif // FIELD_B64
            "<32|64> - \tUse 32 or 64 bit hardcoded primes, eg. B64\n"
        "  <AB|AC|BC><32|64>       - \tAB,AC,BC use two hardcoded primes. eg. AC32 \n"
#ifdef FIELD_DYNAMIC64
        "  <integer 32/64-bit>     - \tSpecify any prime to use (slower than hardcoded)\n"
#endif // FIELD_DYNAMIC64
#ifdef FIELD_CRT64
        "  <list of integers 32/64-bit>  - \tSpecify a list of primes to use"
#endif // FIELD_CRT64
        )
#else // __LP64__
        "  <A|B|C|AB|AC|BC>32 - \tUse 32 bit hardcoded primes, eg. B32\n"
        "                     - \tAB,AC,BC use two hardcoded primes. eg. AC32 \n"
#ifdef FIELD_DYNAMIC32
        "  <integer>          - \tSpecify any 32 bit prime to use (slower than hardcoded)\n"
#endif // FIELD_DYNAMIC32
#ifdef FIELD_CRT32
        "  <list>             - \tSpecify a comma separated list of primes to use"
#endif // FIELD_CRT32
        )
#endif // __LP64__
    ("order,r", po::value<std::string>(&orderStr)->default_value("evalpts"),
        "Interpolation variable order. Possible values:\n"
        "  declared - \tas in the input file,\n"
        "  evalpts  - \tDescending order of predicted evaluation points,\n"
        "  maxdeg   - \tmaximum degree first,\n"
        "  mindeg   - \tminimum degree first,\n"
        "  maxterms - \tmaximum number of terms first,\n"
        "  minterms - \tminimum number of terms first")
    ("prune,p", po::value<std::string>(&pruneStr)->default_value("none"),
        "Use extra homogenizing variable to prunne fully interpolated terms"
        " from stage polynomials. Possible options:\n"
        "  none     - \tdo not use prunning\n"
        "  declared - \tdeclared variable order\n"
        "  max_last - \tdeclared order but max in number of terms last (it comes for free)")
    ("random-seed", po::value<int>(),
        "Seed to use to initialize random number generator. "
        "Default is time.")
    ("savefile,s", po::value<std::string>(),
        "Save intermediate state into specified file.\n"
        "if [filename] already exists, resume from the state saved in that file.")
    ("univar,u", po::value<std::string>(&univarStr)->default_value("newton"),
        "Specify algorithm for univariate interpolation\n"
        "  newton      - \tClassical O(n^2) algorithm\n"
        "  vandermonde - \tsee --vandermonde option\n"
        "  adapt       - \tpick fastest for given size")
    ("vand,v", po::value<std::string>(&vandStr)->default_value("adapt"),
        "Vandermonde matrix solver:\n"
        "  quad  - \tclassical O(n^2) algorithm,\n"
        "  fft   - \tO(n log^2 n) algorithm using Fast Fourier Transform,\n"
        "  adapt - \tChoose fastest algorithm quad or fft for each matrix (default)")
    ("verify-stage", po::value<int>(&verifyStage)->default_value(10),
        "Compare at each stage evaluated black box"
        " and interpolated polynomial at random points. "
        "Program will abort if not matched. Choose different"
        " seed or field in that case.")
    ("verify-final", po::value<int>(&verifyFinal)->default_value(10),
        "Compare evaluated black box and final "
        " interpolated polynomial at random points.")
  ;
  // Declare a group of options that will be
  // allowed both on command line and in config file
  // @todo - define configuration file
  po::options_description config("Configuration");
  config.add_options()
    ("include-path,I", po::value< std::vector<std::string> >()->composing(),
     "include path")
  ;

  // Hidden options, will be allowed both on command line and
  // in config file, but will not be shown to the user.
  po::options_description hidden("Hidden options");
  hidden.add_options()
    ("input-file,i",  po::value<std::string>(), "maple input file")
//    ("output-file,o", po::value<std::string>(), "maple output file")
  ;
  // First and second arguments are treated as input and output files respectively
  po::positional_options_description pd;
  pd.add("input-file", 1); //.add("output-file", 1);

  po::options_description cmdline_options;
  cmdline_options
    .add(mainOptions)
    .add(genOptions)
    .add(interpOptions)
    .add(config)
    .add(hidden);

  po::options_description config_file_options;
  config_file_options
    .add(interpOptions)
    .add(config)
    .add(hidden);

  po::options_description visible("Allowed options");
  visible
    .add(mainOptions)
    .add(genOptions);

  po::variables_map vm;
  po::store(
    po::command_line_parser(argc, argv)
      .options(cmdline_options)
      .positional(pd)
      .extra_parser(at_option_parser)
      .run(), vm);
  po::notify(vm);

  if (vm.count("response-file")) {
    // @todo: allow more flexible response file format with comments
    // Load the file and tokenize it
    std::string filename = vm["response-file"].as<std::string>();
    std::ifstream ifs(filename.c_str());
    if (!ifs) {
      throw std::logic_error(
        std::string("Could no open the response file ") + filename);
    }
    // Read the whole file into a string
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string fileContent = ss.str();

    std::vector<std::string> origArgs;
    boost::split( origArgs, fileContent, boost::is_any_of(" \n\r\t") );
    std::vector<std::string> args;
    // Only use non-white space tokens
    for (unsigned int i = 0; i < origArgs.size(); i++) {
      if (origArgs[i].find_first_not_of(" \n\r\t") != std::string::npos) {
        args.push_back(origArgs[i]);
      }
    }
    //std::cerr << fileContent << std::endl;
    po::store(po::command_line_parser(args)
      .options(cmdline_options)
      .positional(pd)
      .run(), vm);

    po::notify(vm);
  }

  //---------------------------------------------------------------
  // Verify options
  const char* mainOptionNames[] = {
    "version",
    "benchmark",
    "machine",
    "interpolate"
  };
  const unsigned int nOptions =
      sizeof(mainOptionNames) / sizeof(mainOptionNames[0]);

  // throw exception if some exclusive options are specified at the same time
  exclusiveOptions(vm, mainOptionNames, nOptions);

  std::unique_ptr<RunProfile> prof(new RunProfile());

  if (!h_given) {  // If no help option was given
    if (!atLeastOne(vm, mainOptionNames, nOptions)) {
      std::ostringstream oss;
      oss << "At least one of the options" << std::endl
          << "  " << mainOptionNames[0];
      for (unsigned int i = 1; i < nOptions; ++i) {
        oss << ", " << std::endl << "  " << mainOptionNames[i] ;
      }
      oss << std::endl << "must be specified on a command line. "
          "Use -h for help on possible options.";
      throw std::logic_error(oss.str());
    }
  } else {
    prof->setAction(ActionEnum::HELP);
    std::ostringstream oss;
    if (b_given) {
      oss << "\nUsage: " << argv[0] << " -b [-o <output file>]\n\n"
             "Measure speed of key algorithms on this machine,\n"
             "saving output in YAML format output file";
    } else if (v_given) {
      oss << "\nUsage: " << argv[0] << " -v\n\n"
             "shows version,license and other information.";
    } else if (m_given) {
      //oss << machineOptions;
      oss << "\nUsage: " << argv[0] << " -m <output file>\n\n"
             "Measures speed and gathers relevant machine specifications\n"
             "saving output in YAML format output file."
             "This information can be used to compare relative speed of \n"
             "machines and predict how long a problem might take to solve\n"
             "based on how much time the solution took on a different machine.";
    } else if (t_given) {
      oss << interpOptions;
    } else {
      oss << visible;
    }
    prof->setOptionDescription(oss.str());
    return prof;
  }

  for (unsigned int i = 0; i < logTags.size(); ++i) {
    switch(logTags[i]) {
      case 'i': prof->addLogTag(logging::LogTagEnum::INDEX);  break;
      case 't': prof->addLogTag(logging::LogTagEnum::TIME);   break;
      case 'm': prof->addLogTag(logging::LogTagEnum::MODULE); break;
      case 'l': prof->addLogTag(logging::LogTagEnum::LEVEL);  break;
      case 'n': prof->addLogTag(logging::LogTagEnum::FILELINE); break;
      case 'f': prof->addLogTag(logging::LogTagEnum::FUNCTION); break;
      case 'h': prof->addLogTag(logging::LogTagEnum::THREAD_ID); break;
      default:
        throw std::logic_error(
            std::string("Unknown log tag option: ") + logTags[i]);
    }
  }

  //conflicting_options(vm, "help", help_conflicts, sizeof(help_conflicts)/sizeof(char*));
  //conflicting_options(vm, "benchmark", "version");
  //conflicting_options(vm, "benchmark", "interpolate");
  //conflicting_options(vm, "benchmark", "interpolate");
  //conflicting_options(vm, "help", "benchmark");
  for (unsigned int i = 0; i < logLevels.size(); i++) {
    prof->setLogLevel(logLevels[i].log_module, logLevels[i].log_level);
  }

  if (vm.count("trace")) {
    prof->setTraceFileName(vm["trace"].as<std::string>());
  }
  if (vm.count("runtime")) {
    prof->setInfoFileName(vm["runtime"].as<std::string>());
  }

  if (vm.count("output-file")) {
    prof->setOutputFileName(vm["output-file"].as<std::string>());
  } else {
    prof->setOutputFileName("");
  }
  prof->setOverwrite(w_given);

  if (v_given) {
    prof->setAction(ActionEnum::VERSION);
  } else if (b_given) {
    prof->setAction(ActionEnum::BENCHMARK);
    if (vm.count("output-file")) {
      prof->setOutputFileName(vm["output-file"].as<std::string>());
    } else {
      prof->setOutputFileName("");
    }

  } else if (m_given) {
    if (vm.count("output-file")) {
      prof->setOutputFileName(vm["output-file"].as<std::string>());
    } else {
      throw std::logic_error("No output file given for machine info.'");
    }
    prof->setAction(ActionEnum::MACHINE_INFO);
  } else if (t_given) {
    prof->setAction(ActionEnum::INTERPOLATE);
    boost::shared_ptr<InterpProfile> iprof(new InterpProfile());
    prof->setInterpProfile(iprof);

    iprof->setVerifyStage(verifyStage);
    iprof->setVerifyFinal(verifyFinal);

    interp::VarOrderEnum varOrder;
    if (!varOrder.setToValue(orderStr.c_str())
        || varOrder == interp::VarOrderEnum::UNKNOWN) {
      throw std::logic_error(
          std::string("Unknown variable interpolation order: ") + orderStr);
    }
    iprof->setVarOrder(varOrder);
    iprof->setFieldOption(fieldOptionStr);
    if (iprof->getFieldOption() == FieldOptionEnum::UNKNOWN) {
      throw std::logic_error(
          std::string("Unknown field option: ") + fieldOptionStr);
    }
    interp::UnivInterpEnum univInterp;
    if (!univInterp.setToValue(univarStr.c_str())
        || univInterp == interp::UnivInterpEnum::UNKNOWN) {
      throw std::logic_error(
          std::string("Unknown univariate interpolation algorithm: ")
          + univarStr);
    }
    iprof->setUnivInterpAlg(univInterp);
    interp::VandSolveTypeEnum vandType;
    if (!vandType.setToValue(vandStr.c_str())
        || vandType == interp::VandSolveTypeEnum::UNKNOWN) {
      throw std::logic_error(
          std::string("Unknown vandermonde solver algorithm: ") + vandStr);
    }
    iprof->setVandermondeSolver(vandType);
    interp::PrunningEnum prunning;
    if (!prunning.setToValue(pruneStr.c_str())
        || prunning == interp::PrunningEnum::UNKNOWN) {
      throw std::logic_error(
          std::string("Unknown pruning using homogenizing variable: ")
          + vandStr);
    }
    iprof->setPrunningChoice(prunning);
    if (vm.count("input-file")) {
      prof->setInputFileName(vm["input-file"].as<std::string>());
    } else {
      throw std::logic_error("No input file given for interpolation.'");
    }
    if (vm.count("asmfile")) {
      iprof->setAsmFileName(vm["asmfile"].as<std::string>());
    }
    if (vm.count("savefile")) {
      iprof->setSaveFileName(vm["savefile"].as<std::string>());
    }
    if (vm.count("random-seed")) {
      iprof->setRandomSeed(vm["random-seed"].as<int>());
    }
  } else {
    std::cerr << "Unknown option ... ?";
    prof->setAction(ActionEnum::HELP);
    std::ostringstream oss;
    oss << visible;
    prof->setOptionDescription(oss.str());
  }
  return prof;
}

}  // namespace factory
}  // namespace runprofile
