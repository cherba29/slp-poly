# Straight Line Program (SLP) Polynomial interpolator.

UNFUNCTIONAL: under development.

----

A _straight-line program_ is a sequence of arithmetic assignments to new
variables, the operands of which are either constants, indeterminates, or
previously assigned variables. The operators allowed are addition, subtraction,
multiplication, and division. 

SLP representation can define in polynomial-space families of polynomials with
exponentially many individual terms, such as
[determinants](https://en.wikipedia.org/wiki/Determinant) by
Gaussian elimination.

When converting SLP into its polynomial form it is often the case that one gets
exponential explosion of intermediate expressions while final result might not.

This program avoids this issue by interpolating final sparse polynomial which
respresents given SLP. Its complexity is tied to size of the result.

## Requirements

* C++ compiler
  * g++ version 5.4
* [cmake](https://cmake.org/download/)
  * version 2.8
* [boost](http://www.boost.org/)
  * version 1.58.0
* flex/bison ([windows](https://sourceforge.net/projects/winflexbison/))

## Build & Run

    $ git clone ...
    $ cd slp-poly
    $ mkdir build
    $ cd build
    $ BOOST_ROOT=<Path to Boost> cmake .
    $ make
    $ src/slp-poly

### Windows

CMake based build produces single executable `slp-poly`.

1. `mkdir build`
1. `cd build`
1. `cmake -G "Visual Studio 12 2013" -Wdev -DCMAKE_BUILD_TYPE=Debug ..`

### Linux

1. `mkdir build`
1. `cd build`
1. `cmake -G "Unix Makefiles" -Wdev -DCMAKE_BUILD_TYPE=Debug ..`
1. `make`
1. `./src/slp-poly`


## Developing

TBD
