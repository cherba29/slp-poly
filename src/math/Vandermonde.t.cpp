/**
 * @file Vandermonde.h
 *
 */

#include "math/vandermonde/QuadSolver.h"
#include "math/vandermonde/FFTSolver.h"

#include "operation/field/setup.h"
//#include <boost/timer.hpp>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE( Vandermonde );

//******************************************************************************
BOOST_AUTO_TEST_CASE_TEMPLATE( vand_quad_simple, F, math::FieldTypes ) {
  math::FieldFixture<F> f; BOOST_CHECK( f ); // avoid unreferenced local variable warning

  BOOST_TEST_MESSAGE( "  Testing " << F::getName() );

  unsigned int size = 3;
  boost::scoped_array<F> entries(new F[size+1]);
  entries[0].template setTo<-1>();
  entries[1].template setTo<0>();
  entries[2].template setTo<1>();

  BOOST_CHECK(entries[0].template is<-1>());

  math::vandermonde::QuadSolver<F> qSolver(size, entries.get());

  BOOST_CHECK_EQUAL(qSolver.getMatrixSize(),size);

  BOOST_CHECK_EQUAL(qSolver.getEntry(0),F(-1));
  BOOST_CHECK_EQUAL(qSolver.getEntry(1),F(0));
  BOOST_CHECK_EQUAL(qSolver.getEntry(2),F(1));

  boost::scoped_array<F> values(new F[size]);

  values[0].template setTo<1>();
  values[1].template setTo<0>();
  values[2].template setTo<1>();

  boost::scoped_array<F> result(new F[size]);

  qSolver.solveTranspose(values.get(), result.get());

  BOOST_CHECK_EQUAL(result[0],F(1)/F(2));
  BOOST_CHECK_EQUAL(result[1],F(0));
  BOOST_CHECK_EQUAL(result[2],F(1)/F(2));

}

//******************************************************************************
BOOST_AUTO_TEST_CASE_TEMPLATE( vand_fft_simple, F, math::FieldTypes ) {
  math::FieldFixture<F> f; BOOST_CHECK( f ); // avoid unreferenced local variable warning

  BOOST_TEST_MESSAGE( "  Testing " << F::getName() );

  unsigned int size = 3;
  boost::scoped_array<F> entries(new F[size]);
  entries[0].template setTo<-1>();
  entries[1].template setTo<0>();
  entries[2].template setTo<1>();

  BOOST_CHECK(entries[0].template is<-1>());
  BOOST_CHECK(entries[1].template is<0>());
  BOOST_CHECK(entries[2].template is<1>());

  math::vandermonde::FFTSolver<F> fftSolver(size, entries.get());

  BOOST_CHECK_EQUAL(fftSolver.getMatrixSize(), size);

  BOOST_CHECK_EQUAL(fftSolver.getEntry(0), F(-1));
  BOOST_CHECK_EQUAL(fftSolver.getEntry(1), F(0));
  BOOST_CHECK_EQUAL(fftSolver.getEntry(2), F(1));

  boost::scoped_array<F> coeffs(new F[size]);
  coeffs[0] = F(1)/F(2);
  coeffs[1] = F(0);
  coeffs[2] = F(1)/F(2);

  boost::scoped_array<F> values(new F[size]);

  values[0].template setTo<1>();
  values[1].template setTo<0>();
  values[2].template setTo<1>();

  boost::scoped_array<F> result(new F[size]);

  fftSolver.evaluate(coeffs.get(),result.get());

  BOOST_CHECK_EQUAL(result[0], F(1));
  BOOST_CHECK_EQUAL(result[1], F(1)/F(2));
  BOOST_CHECK_EQUAL(result[2], F(1));


  fftSolver.solveTranspose(values.get(), result.get());

  BOOST_CHECK_EQUAL(result[0], F(1)/F(2));
  BOOST_CHECK_EQUAL(result[1], F(0));
  BOOST_CHECK_EQUAL(result[2], F(1)/F(2));

}


BOOST_AUTO_TEST_SUITE_END();


bool init_unit_test() {
  return true;
}

