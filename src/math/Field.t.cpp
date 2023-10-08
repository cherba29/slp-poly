#include <sstream>
#include <iomanip>

#include "operation/field/setup.h"

#include <boost/timer.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(field)

BOOST_AUTO_TEST_CASE_TEMPLATE(test_field_constructors, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE( "  Testing " << F::getName() );
  // Default constructor
  // F xx;
  // BOOST_WARN_MESSAGE(!xx.template is<0>(),
  //                    "Element should not be set on default constructor" );
  F x(0);
  BOOST_CHECK(x.template is<0>() );
  BOOST_CHECK(x.isPositive() );
  BOOST_CHECK(!x.hasInverse() );

  for (int i = -10; i < 0; ++i) {
    F y(i);
    BOOST_CHECK(y.isNegative());
    BOOST_CHECK(y.is(i));
  }
  for (int i = 0; i < 10; ++i) {
    F y(i);
    BOOST_CHECK(y.isPositive());
    BOOST_CHECK(y.is(i));
  }

  // Initialize from integer
  F y(1);
  //  BOOST_CHECK(y == F(1));
  BOOST_CHECK(y.template is<1>());
  BOOST_CHECK(y.isPositive());
  BOOST_CHECK(y.hasInverse());

  F z(-1);
  BOOST_CHECK(z.isNegative());
  BOOST_CHECK(z.template is<-1>());
  BOOST_CHECK(z.hasInverse());

  // Check copy constructor
  F q = y+z;
  BOOST_CHECK(q.template is<0>());
}

//=============================================================================
BOOST_AUTO_TEST_CASE_TEMPLATE(setTo_is_operations, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());

  F x;

  x.template setTo<1>();
  BOOST_CHECK_EQUAL(x, F(1));
  BOOST_CHECK_MESSAGE(x.template is<1>()," for field " << F::getName());
  BOOST_CHECK_MESSAGE(!x.template is<0>()," for field " << F::getName());
  BOOST_CHECK(x.is(1));

  x.template setTo<-1>();
  BOOST_CHECK_EQUAL(x, F(-1));
  BOOST_CHECK_MESSAGE(!x.template is<0>(), " for field " << F::getName());
  BOOST_CHECK_MESSAGE(x.template is<-1>(), " for field " << F::getName());
  BOOST_CHECK_MESSAGE(x.is(-1), " for field " << F::getName());

  x.template setTo<0>();
  BOOST_CHECK_EQUAL(x, F(0));
  BOOST_CHECK_MESSAGE(x.template is<0>(),
      "x.template is<0>() failed, for x==" << x
      << "for field " << F::getName());
  BOOST_CHECK_MESSAGE(x.is(0),
      "x.is(0) failed, for x==" << x << " for field " << F::getName());

  x.template setTo<-101>();
  BOOST_CHECK_EQUAL( x, -F(101) );
  BOOST_CHECK_MESSAGE( x.template is<-101>(),
      "x.template is<-101>() failed, x==" << x
      << " for field " << F::getName());
  BOOST_CHECK_MESSAGE( x.is(-101),
      "x.is(-101) failed, x==" << x << " for field " << F::getName());

  x.setTo(-202);
  BOOST_CHECK_EQUAL(x, -F(202));
  BOOST_CHECK_MESSAGE(x.template is<-202>(),
      "x.template is<-202>() failed, x==" << x
      << " for field " << F::getName());
  BOOST_CHECK_MESSAGE(x.is(-202),
      "x.is(-202) failed,  x==" << x << " for field " << F::getName());
}

//=============================================================================
BOOST_AUTO_TEST_CASE_TEMPLATE(random, F, math::FieldTypes) {
  // @todo, make a better test case for random
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  F x;
  F y;
  const int MAX_TESTS = 10;
  for (int i = 0; i < MAX_TESTS; ++i) {
    x.invertableRandom();  // Check that random does not return anything higher
                           // than supported MOD.
    y.template setTo<-1>();
    F z = x+y;
    // Stream to string
    char strBuf[100];
    z.toString(strBuf);
    // Read from string
    std::istringstream iss(strBuf);
    typename F::sbase_type q;
    iss >> q;
    if (iss) {
      BOOST_CHECK_MESSAGE(z.is(q), i << " z==" << z << " and q==" << q
          << " buf==" << strBuf << " for " << F::getName());
      BOOST_CHECK_MESSAGE(x==(z+F(1))," x==" << x << " and z==" << z
          << " for " << F::getName());
      BOOST_CHECK_MESSAGE(x.is(q+1), " x=="<< x <<" and q==" << q
          << " for " << F::getName());
      BOOST_CHECK_MESSAGE(iss.eof(), "Input stream is not fully read in");
    } else {
      BOOST_WARN_MESSAGE(false, "Failed to read in '" << strBuf
          << "' into integer of " << sizeof(q)
          << " bytes for " << F::getName());
    }
  }
}

//=============================================================================
BOOST_AUTO_TEST_CASE_TEMPLATE(field_size, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  BOOST_TEST_MESSAGE("    sizeof(F)==" << sizeof(F));
  BOOST_TEST_MESSAGE("    sizeof(void*)==" << sizeof(void*));
  // Test that field size is at least of pointer size.
  BOOST_WARN_MESSAGE(sizeof(F) >= sizeof(void*),F::getName() << " of size "
      << sizeof(F) << " is less than sizeof(void*)" << sizeof(void*));
  // Warn if it is larger than a pointer size.
  BOOST_WARN_MESSAGE(sizeof(F) <= sizeof(void*),F::getName() << " of size "
      << sizeof(F) << "is greater than sizeof(void*)" << sizeof(void*));
  BOOST_CHECK(F::getSize() > (1 << 16));
}

//=============================================================================
BOOST_AUTO_TEST_CASE_TEMPLATE(field_name, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  const char* name1 = F::getName();
  std::string name1Str = name1;
  const char* name2 = F::getName();
  std::string name2Str = name2;
  //BOOST_CHECK_EQUAL(name1, name2); // Returned same address.
  BOOST_CHECK_EQUAL(name1Str, name2Str); // Returned same value.
  BOOST_CHECK(name1Str.size() > 0); // Is not empty string.
}

//=============================================================================
BOOST_AUTO_TEST_CASE_TEMPLATE(swap, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  F x;
  F y;
  x.template setTo<15>();
  y.template setTo<-1>();
  BOOST_CHECK(x.template is<15>());
  BOOST_CHECK(y.template is<-1>());
  x.swap(y);
  BOOST_CHECK(x.template is<-1>());
  BOOST_CHECK(y.template is<15>());
}
//=============================================================================
BOOST_AUTO_TEST_CASE_TEMPLATE(invert, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.

  BOOST_TEST_MESSAGE("  Testing " << F::getName());

  F x(-1);
  F y = x.getInverse();
  BOOST_CHECK_MESSAGE(x==y, "is x==y with x=" << x << " y=" << y);
  y = F(2);
  x = y;
  y.invert();
  BOOST_CHECK_MESSAGE(x * y == F(1), "is x*y==1 with x=" << x << " y=" << y);
}
//=============================================================================
BOOST_AUTO_TEST_CASE_TEMPLATE(sqr, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());

  F x(1);
  BOOST_REQUIRE_EQUAL(x, F(1));
  x.sqr();
  BOOST_CHECK_EQUAL(x, F(1));
  x.template setTo<-1>();
  BOOST_CHECK_EQUAL(x, F(-1));
  x.sqr();
  BOOST_CHECK(x.template is<1>());

  // sqr
  F s1(-2);
  s1.sqr();
  BOOST_CHECK(s1.template is<4>());
  s1.sqr();
  BOOST_CHECK_EQUAL(s1, F(16));
  s1.sqr();
  BOOST_CHECK_EQUAL(s1, F(256));
  s1.sqr();
  BOOST_CHECK_EQUAL(s1, F(256)*F(256));
}

BOOST_AUTO_TEST_CASE_TEMPLATE( powerBy, F, math::FieldTypes ) {
  math::FieldFixture<F> f;
  BOOST_CHECK( f ); // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  F s1;

  // x.powerBy(2)
  s1 = F(2);
  s1.powerBy(3);
  BOOST_CHECK_EQUAL(s1, F(8));
  s1.powerBy(2);
  BOOST_CHECK_EQUAL(s1, F(64));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(square_root, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  F x;
  x.template setTo<4>();
  BOOST_CHECK_EQUAL(x, F(4));
  F y(2);
  BOOST_CHECK_EQUAL(y, F(2));

  F z = x.getSqrt();
  BOOST_CHECK_EQUAL(z, F(2));

  BOOST_CHECK_EQUAL(x.getSqrt(), y);

  x.template setTo<9>();
  BOOST_CHECK_EQUAL(x.getSqrt(), F(3));

  x.template setTo<1>();
  BOOST_CHECK_EQUAL(x.getSqrt(), F(1));

  x.template setTo<-1>();
  z = x.getSqrt();
  BOOST_CHECK_EQUAL(z * z, x);

  // F sqrt() const
  {
    F v;
    v.template setTo<9>();
    F sq = v.getSqrt();
    BOOST_CHECK((sq.template is<3>()) || (sq.template is<-3>()));

    v.template setTo<1>();
    sq = v.getSqrt();
    BOOST_CHECK_EQUAL(sq, F(1));
  }

}

template <typename F>
void powerBy2p(int d, F* val) {
  // Max power we can compute is 2^32-1,
  // if depth is > 31 we need to power in a loop.
  // Using a^(2^i) = (a^(2^j))^(2^k) where i=j+k.
  int n31 = d / 31;
  int r31 = d % 31;
  val->powerBy(1 << r31);
  for (int i = 0; i < n31; i++) {
    val->powerBy(1UL << 31);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE( primitive_roots, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.

  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  // Unlikely to encounter fields which have larger order.
  const unsigned int limit = 1024;
  F primRoots[limit];
  F primRootsInv[limit];

  unsigned int order = math::field::computePrimitiveRoots<F>(
      F(-1), primRoots, primRootsInv, limit);

  BOOST_CHECK_EQUAL(order, math::field::getFourierOrder<F>());

  for (unsigned int i = 0; i < order; ++i) {
    F omega2 = primRoots[i];
    powerBy2p<F>(i + 1, &omega2);  // Compute omega2^(2^depth).
    BOOST_CHECK_EQUAL(omega2, F(1));
    BOOST_CHECK_EQUAL(primRoots[i].getInverse(), primRootsInv[i]);
  }
  BOOST_CHECK_EQUAL(primRoots[0],F(-1));
  for (unsigned int i = 1; i < order; ++i) {
    BOOST_CHECK_EQUAL(primRoots[i] * primRoots[i], primRoots[i-1]);
  }

  const F* roots = F::getPrimRoots();
  for (unsigned int i = 1; i < order; ++i) {
    BOOST_CHECK_EQUAL(roots[i], primRoots[i]);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fourier_order, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.

  BOOST_TEST_MESSAGE("  Testing " << F::getName());

  F omega;
  omega.template setTo<1>();
  omega.negate();
  BOOST_CHECK_EQUAL(omega, F(-1));
  BOOST_CHECK(omega.template is<-1>());
  F old_omega;
  F omega2;
  F twoPow(2);

  int depth;
  // Maximum depth is is limited by max i where 2^i < Field size.
  for (depth = 1; omega.hasInverse(); ++depth) {
    old_omega = omega;
    omega2 = omega;

    // See p215 Modern Computer Algebra, Gathen/Gerhald.
    // Check that omega is n = 2^depth root of unity in F
    powerBy2p<F>(depth, &omega2);  // Compute omega2^(2^depth).
    BOOST_CHECK_EQUAL(omega2, F(1));

    // check that omega^(n/t)-1 has inverse of t=2
    omega2 = omega;
    powerBy2p<F>(depth-1, &omega2); // omega^(n/2)
    BOOST_CHECK(omega2 != F(1));
    BOOST_CHECK((omega2-F(1)).hasInverse());

    omega = omega.getSqrt();
    if (omega.hasInverse()) {
      BOOST_CHECK_EQUAL(omega * omega, old_omega);
      BOOST_CHECK((!omega.template is<0>()));
    }
  }
  BOOST_CHECK(!omega.hasInverse());

  powerBy2p<F>(depth, &old_omega);
  BOOST_CHECK_EQUAL(old_omega, F(1));
  depth--;
  BOOST_CHECK_EQUAL(depth, math::field::getFourierOrder<F>());

  // static long double size()
  long double size = F::getSize();
  BOOST_TEST_MESSAGE("Field size "
      << std::fixed << std::setprecision(30) << size);
  BOOST_CHECK(size >= (static_cast<uint64_t>(1) << depth));
  // Check that size-1 is divisible by 2^depth.
  long double rem = std::fmod(size - 1,
      static_cast<long double>(static_cast<uint64_t>(1) << depth));
  BOOST_CHECK_EQUAL(rem, 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fromString, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK( f ); // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  F s;
  BOOST_CHECK(math::field::util::fromString("-101", &s));
  // Check constructor from string.
  BOOST_CHECK_MESSAGE(s.isNegative(), "with s=" << s );
  //BOOST_CHECK_EQUAL( s , F(-101) );
  //BOOST_CHECK( s == F(-101) );
  BOOST_CHECK_MESSAGE(s.template is<-101>(), "with s=" << s);
  BOOST_CHECK_MESSAGE(s.is(-101), "with s=" << s);
  BOOST_CHECK_MESSAGE(s.hasInverse(), "with s=" << s);

  BOOST_CHECK(math::field::util::fromString("-0", &s));
  char buffer[100];
  s.toDebugString(buffer);
  BOOST_TEST_MESSAGE("  debug str for -0: " << buffer);
  F x(-1);
  x.toDebugString(buffer);
  BOOST_TEST_MESSAGE("  debug str for -1: " << buffer);
  BOOST_CHECK_EQUAL( x, F(-1));
  BOOST_CHECK(x.template is<-1>());

  BOOST_CHECK(!math::field::util::fromString("", &s));
  BOOST_CHECK(!math::field::util::fromString(" 1", &s));
  BOOST_CHECK(!math::field::util::fromString("-", &s));
  BOOST_CHECK(!math::field::util::fromString("+", &s));
  BOOST_CHECK(!math::field::util::fromString("A", &s));

  BOOST_CHECK(math::field::util::fromString("+0", &s));
  BOOST_CHECK(s.template is<0>());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(toString, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.

  // Convert negative number to string.
  F x(-101);
  char* buf = new char[F::getMaxStringRepLength()];
  x.toString(buf);
  BOOST_CHECK_EQUAL(buf[0], '-');
  BOOST_CHECK_EQUAL(buf[1], '1');
  BOOST_CHECK_EQUAL(buf[2], '0');
  BOOST_CHECK_EQUAL(buf[3], '1');
  BOOST_CHECK_EQUAL(buf[4], '\0');

  // Convert 0 to string
  x.template setTo<0>();
  x.toString(buf);
  BOOST_CHECK_EQUAL(buf[0], '0');
  BOOST_CHECK_EQUAL(buf[1], '\0');

  // Convert -1/2 to string and back
  F z;
  z.template setTo<-1>();
  z /= F(2);
  z.toString(buf);

  if (math::field::util::fromString<F>(buf, &x)) {
    BOOST_CHECK_MESSAGE(z == x, "z == x with z ==" << z << " and x == " << x
        << " and buf == " << buf << " for " << F::getName());
  } else {
    BOOST_ERROR("Failed to convert back to field: " << buf);
  }
/*
  x.template setTo<0>();
  for (int i = (buf[0]=='-')?1:0; buf[i]; ++i) {
    x *= F(10);
    x += F(buf[i]-'0');
  }
  if (buf[0]=='-') {
    x.negate();
  }
  BOOST_CHECK_MESSAGE(z==x, "z==x with z=="<<z<<" and x==" << x
    << " and buf==" << buf << " for " << F::getName());
*/
  delete[] buf;
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mult_eq, F, math::FieldTypes) {
  math::FieldFixture<F> f; BOOST_CHECK( f ); // avoid unreferenced local variable warning
  BOOST_TEST_MESSAGE( "  Testing " << F::getName() );
  //F& F::operator*=(const F& m);
  F x(-1);
  F y(-1);
  x *= y;
  BOOST_CHECK_EQUAL(x, F(1));

  // Multiply by one gives same number back
  y.template setTo<1>();
  x *= y;
  BOOST_CHECK_EQUAL(x, F(1));

  // Multiply by -1 negates
  x.template setTo<101>();
  y.template setTo<-1>();
  x *= y;
  BOOST_CHECK_EQUAL(x, F(-101));

  // Multiply by zero gives zero
  y.template setTo<0>();
  x *= y;
  BOOST_CHECK_EQUAL(x, F(0));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(negate, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());

  BOOST_CHECK(F(0).isPositive());

  F x(1);
  BOOST_CHECK_MESSAGE(x.template is<1>(), "Is x==1 wit x=" << x);

  F y = -x;  // operator-
  BOOST_CHECK_MESSAGE(y.isNegative(),
      "Is y negative with y=-x=" << y << " and x=" << x);

  x.negate();

  BOOST_CHECK_EQUAL(x,y);

  BOOST_CHECK_MESSAGE(x.template is<-1>(), "Is x == -1 with x=" << x);
  BOOST_CHECK_MESSAGE((-x) == (-(-(-x))),
      "Is -x == -(-(-x)) with x=" << x); // operator-
  BOOST_CHECK_MESSAGE((-x).template is<1>(), "Is -x==1 with x=" << x);
  x.negate();
  BOOST_CHECK_MESSAGE(x.isPositive(), " with x=" << x);

  BOOST_CHECK_EQUAL(x, -y);

  y = F(1);
  x = F(-1);
  BOOST_CHECK_MESSAGE(x.isNegative(), "isNegative with x=-1==" << x);
  BOOST_CHECK_MESSAGE(y.isPositive(), "isPositive with y=1==" << y);
  x /= F(2);
  y /= F(2);
  BOOST_CHECK_MESSAGE(x.isPositive(), "isPositive with x=-1/2==" << x);
  BOOST_CHECK_MESSAGE((-x).isNegative(),
      "(-x).isNegative() with x=(-1/2)==" << x << " and -x==" << -x);
  BOOST_CHECK_MESSAGE(y.isNegative(), "isNegative with y=1/2==" << y);
  x.negate();
  y.negate();
  BOOST_CHECK_MESSAGE(x.isNegative(), "isNegative with x=1/2==" << x);
  BOOST_CHECK_MESSAGE(y.isPositive(), "isPositive with y=-1/2==" << y);
  x -= F(1);
  y += F(1);
  BOOST_CHECK_MESSAGE(x.isPositive(), "isPositive with x=1/2-1==" << x);
  BOOST_CHECK_MESSAGE(y.isNegative(), "isNegative with y=-1/2+1==" << y);
  BOOST_CHECK_MESSAGE(x + y == F(0),"x+y==0 with x=="<<x<< " y==" << y);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_eq, F, math::FieldTypes) {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  // Test F& F::operator+=(const F& m);

  F y;
  y.template setTo<0>();
  y += F(1);
  BOOST_CHECK(y.template is<1>());

  F x(-1);
  x += y;
  BOOST_CHECK_EQUAL(x, F(0));

  // Add zero gives same number back.
  x.template setTo<-3>();
  y.template setTo<0>();
  x += y;
  BOOST_CHECK_EQUAL(x, F(-3));

  // Add -1 is oposite of +1.
  x.template setTo<101>();
  y.template setTo<-1>();
  x += y;
  BOOST_CHECK_EQUAL(x + F(1), F(101));

  F z;
  x.template setTo<-441849981>();
  z = x + y;
  BOOST_CHECK_EQUAL(x - F(1), z);

}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_field_operations, F, math::FieldTypes)  {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  F x(1);
  BOOST_CHECK(x.template is<1>());

  // -=
  F y2;
  y2.template setTo<0>();
  BOOST_CHECK(y2.template is<0>());
  y2-=x;
  BOOST_CHECK(y2.template is<-1>());
  // /=
  // x.inverse()
  // y = x.inv()
  // x.setToOne();
  // x.setToZero();
  // F::setFill(dest, val, count)
  // F::setFill2P(dest, val, count)
  // x.setTo(int)
  // int toInt() const
  // std::string toString() const
  // std::string toStringRepresentation()
  // F operator+(Field32 right) const
  // F operator-(Field32 right) const
  // F operator*(Field32 right) const
  // F operator /(Field32 right) const
  // F pow(unsigned int p) const
  F p1(2);
  F p2 = p1;
  p2.sqr();
  std::stringstream ss;
  BOOST_CHECK_EQUAL(p1.getPow(0), F(1));
  BOOST_CHECK_EQUAL(p1.getPow(1), p1);
  BOOST_CHECK_EQUAL(p1.getPow(2), p2);
  BOOST_CHECK_EQUAL(p1.getPow(4), p2*p2);
  BOOST_CHECK_EQUAL(p1.getPow(4), F(16));

  // void random()
  // void nonZeroRandom()
  // static F getRandom()
  // static F one()
  // static F negOne()
  // static F zero()
  // static void randomInit(unsigned int seed)
  // static void swap(F& A, Field32& B)
  // static void add(F& dest, const F& src1, const F& src2)
  // static void subtract(F& dest, const F& src1, const F& src2)
  // static void neg(F& dest, const F& src)
  // static void multiply(F& dest, const F& src1, const F& src2)
  // static void divide(F& dest, const F& src1, const F& src2)
  // static void pow(F& dest, F& src, unsigned int p)

  // std::istream& operator>>(std::istream& is, F& f);
  // std::ostream& operator<<(std::ostream& os, const F& f);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_field_det,F,math::FieldTypes)  {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  BOOST_TEST_MESSAGE("  Testing " << F::getName());
  // 3x3 identity matrix.
  F mtx[9];
  mtx[0].template setTo<1>();
  mtx[1].template setTo<0>(); 
  mtx[2].template setTo<0>();
  mtx[3].template setTo<0>();
  mtx[4].template setTo<1>();
  mtx[5].template setTo<0>();
  mtx[6].template setTo<0>();
  mtx[7].template setTo<0>();
  mtx[8].template setTo<1>();
  // test static void det(F& dest, F* first, int size)

  BOOST_TEST_MESSAGE("mtx");
  BOOST_TEST_MESSAGE(mtx[0] << " " << mtx[1] << " " << mtx[2]);
  BOOST_TEST_MESSAGE(mtx[3] << " " << mtx[4] << " " << mtx[5]);
  BOOST_TEST_MESSAGE(mtx[6] << " " << mtx[7] << " " << mtx[8]);

  F dest;
  math::field::util::det<F>(&dest, mtx, 3);
  BOOST_CHECK(dest.template is<1>());
  BOOST_TEST_MESSAGE("Det = " << dest);

  boost::timer t;
  const int sz = 11;
  boost::shared_ptr<F> mtxptr(F::allocSq(sz),F::dealloc);
  F* mtx3 = mtxptr.get();
  for (int i = 0; i < sz; i++) {
    for (int j = 0; j < sz; j++) {
      mtx3[i*sz+j].template setTo<0>();
    }
  }
  BOOST_TEST_MESSAGE("Constructed " << sz << "x" << sz << " on heap in " << t.elapsed() << "s");
  t.restart();
  for (int i = 0; i < sz; i++) {
    mtx3[(sz-1)*(i+1)].template setTo<1>();
  }
  BOOST_TEST_MESSAGE("Filled diagonal in " << t.elapsed() << "s");
  for (int i = 0; i < sz; i++) {
    for (int j = 0; j < sz; j++) {
      if ((sz-i-1) == j) BOOST_CHECK(mtx3[i*sz+j].template is<1>());
      else BOOST_CHECK(mtx3[i*sz+j].template is<0>());
    }
  }

  t.restart();
  math::field::util::det<F>(&dest, mtx3, sz);
  BOOST_TEST_MESSAGE("Computed Determinant " << dest << " in " << t.elapsed() << "s");
  BOOST_CHECK( (sz>>1)&1?dest.template is<-1>():dest.template is<1>() );
}

//=============================================================================
/*
BOOST_AUTO_TEST_CASE_TEMPLATE(test_field_det2p, F, FieldTypes)  {
  math::FieldFixture<F> f;
  BOOST_CHECK(f);  // Avoid unreferenced local variable warning.
  // static void det2p(F& dest, F* first, int size)
  const int sz2 = 10;
  unsigned int p2w = math::intlog(sz2);
  BOOST_CHECK_EQUAL( p2w, 4 );
  F* mtx2 = F::allocSq2p(sz2);
  for (int i = 0; i < sz2; ++i) {
    for (int j = 0; j < sz2; ++j) {
      mtx2[(i << p2w) + j].setTo<0>();
    }
  }
  for (int i = 0; i < sz2; ++i) {
    mtx2[(i<<p2w)+sz2-i-1].setTo<1>();
  }
  for (int i = 0; i < sz2; ++i) {
    for (int j = 0; j < sz2; ++j) {
      if (i == sz2-j-1) {
        BOOST_CHECK_EQUAL(mtx2[(i << p2w) + j] , 1);
      } else {
        BOOST_CHECK_EQUAL(mtx2[(i << p2w) + j] , 0);
      }
    }
  }
  F dest2;
  dest2.setTo<0>();
  BOOST_CHECK_EQUAL(dest2, 0);
  math::util::det2p<F>(&dest2, mtx2, sz2);
  delete[] mtx2;
  BOOST_CHECK_EQUAL(dest2,(sz2 >> 1) & 1 ? -1 : 1);
  //BOOST_CHECK_EQUAL( dest2, F(1) );
  BOOST_TEST_MESSAGE("Computed Determinant " << dest2);
}
*/
BOOST_AUTO_TEST_SUITE_END()
