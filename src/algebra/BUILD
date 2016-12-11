cc_library(
    name = "polynomial",
    hdrs = [
        "Polynomial.h",
        "Polynomial-impl.h",
    ],
    srcs = [
        "Polynomial.cpp",
    ],
    deps = [
    ],
    visibility = ["//interp:__pkg__",],
)

cc_library(
    name = "partial_polynomial",
    hdrs = [
        "PartialPolynomial.h",
    ],
    srcs = [
        "PartialPolynomial.cpp",
    ],
    deps = [
        ":polynomial",
        "//util:smart_assert",
        "//external:boost-function",
        "//external:boost-smart_ptr",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
)

cc_test(
    name = "partial_polynomial_test",
    srcs = ["PartialPolynomial.t.cpp"],
    deps = [
        ":partial_polynomial",
        "//external:boost-test",
    ],
)