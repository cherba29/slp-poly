CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(slp-poly)

# The version number.
SET(PROJECT_DESCRIPTION "Polynomial Interpolator (${PROJECT_NAME})")
SET(MAJOR_VERSION 0 )
SET(MINOR_VERSION 1 )
SET(PATCH_LEVEL 0 )
SET(BUG_REPORT_URL, "https://github.com/cherba29/slp-poly/issues")
SET(SITE "https://github.com/cherba29/slp-poly")

# By default release with debug info
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "RelWithDebInfo")
ENDIF()

IF(CMAKE_BUILD_TYPE STREQUAL "Debug" OR
 CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
 ADD_DEFINITIONS("-D_DEBUG")
ENDIF()

# Set compiler flags variable to be included as info in the build
IF(CMAKE_BUILD_TYPE MATCHES None)
  SET(COMPILER_FLAGS ${CMAKE_CXX_FLAGS})
ELSEIF(CMAKE_BUILD_TYPE MATCHES Debug)
  SET(COMPILER_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
  SET(COMPILER_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
ELSEIF(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  SET(COMPILER_FLAGS ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
ELSEIF(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
  SET(COMPILER_FLAGS ${CMAKE_CXX_FLAGS_MINSIZEREL})
ENDIF()

exec_program(
    "git"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS "rev-parse -q HEAD"
    OUTPUT_VARIABLE GIT_SHA1 )

#-- Generate configuration file from cmake -------------
configure_file (
  "${PROJECT_SOURCE_DIR}/platform/Platform.cpp.in"
  "${PROJECT_SOURCE_DIR}/platform/Platform.cpp"
)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

FIND_PACKAGE(Boost COMPONENTS system thread log REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

MACRO(GENERATE_ENUM _enum_name)
  ADD_CUSTOM_COMMAND(
      OUTPUT ${_enum_name}.cpp ${_enum_name}.h ${_enum_name}.t.cpp 
      COMMAND python ${CMAKE_SOURCE_DIR}/third_party/cppenum/cppenum/gen.py --config ${CMAKE_CURRENT_SOURCE_DIR}/${_enum_name}.yaml --output_dir ${CMAKE_CURRENT_BINARY_DIR}
      DEPENDS ${_enum_name}.yaml
      COMMENT "Generating c++ enum ${_enum_name}")
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ENDMACRO()

GENERATE_ENUM(ReturnValue)

ADD_EXECUTABLE(slp-poly
    main.cpp
    ReturnValue.h
    ReturnValue.cpp
    platform/Platform.h
    platform/Platform.cpp
    util/log.h
    util/log.cpp)

TARGET_LINK_LIBRARIES(slp-poly ${Boost_LIBRARIES} )

MESSAGE(STATUS)
MESSAGE(STATUS "Boost Version = ${Boost_VERSION}")
MESSAGE(STATUS "Boost Include = ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "Boost Libs = ${Boost_LIBRARIES}")