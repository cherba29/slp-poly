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
        "//external:boost-function",
        "//external:boost-smart_ptr",
        "//util:smart_assert",
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
