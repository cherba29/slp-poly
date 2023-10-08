cc_library(
    name = "polynomial",
    srcs = [
        "Polynomial.cpp",
    ],
    hdrs = [
        "Polynomial.h",
        "Polynomial-impl.h",
    ],
    visibility = ["//interp:__pkg__"],
    deps = [
    ],
)

cc_library(
    name = "partial_polynomial",
    srcs = [
        "PartialPolynomial.cpp",
    ],
    hdrs = [
        "PartialPolynomial.h",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
    deps = [
        ":polynomial",
        "@boost//:function",
        "@boost//:smart_ptr",
        "//util:smart_assert",
    ],
)

cc_test(
    name = "partial_polynomial_test",
    srcs = ["PartialPolynomial.t.cpp"],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        ":partial_polynomial",
        "@boost//:test.so",
    ],
)
