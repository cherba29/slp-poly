#ifndef NTRP_MATH_RANDOM_H
#define NTRP_MATH_RANDOM_H

#include <boost/random.hpp>

#ifdef max
#undef max
#endif

namespace math {

template <typename UIntType>
class random {
  
  static boost::mt19937& getRgen() {
    static boost::mt19937 rng;  
    return rng;
  }

  static boost::variate_generator<boost::mt19937&, boost::uniform_int<UIntType> >& 
  getGenerator() {
    //static typename boost::uniform_int<UIntType> distribution;
    static boost::variate_generator<boost::mt19937&, boost::uniform_int<UIntType> > 
      // by default range is 0..9, set to maximum range 
      gen(getRgen(), boost::uniform_int<UIntType>(0, std::numeric_limits<UIntType>::max()));
    return gen;
  }

public:

  static void setSeed(unsigned int seed) { 
    getRgen().seed(static_cast<boost::uint32_t>(seed));
    //random<UIntType>::getGenerator().seed(seed); 
  }
  
  static UIntType get() { 
    return random<UIntType>::getGenerator()();
  }
  static UIntType get(UIntType minVal, UIntType maxVal) {
    //static boost::mt19937 rng;
    //typename boost::uniform_int<UIntType> distribution(minVal, maxVal);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<UIntType> > 
      gen(getRgen(), boost::uniform_int<UIntType>(minVal, maxVal));

    return gen();
  }
  
};
}  // namespace math

#endif  //NTRP_MATH_RANDOM_H
