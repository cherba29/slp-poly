cc_library(
    name = "benchmark",
    hdrs = [
        "benchmark.h",
    ],
    visibility = [
        "//run:__pkg__",
    ],
    deps = [
        "//complexity:equation",
        "//complexity:tracker",
        "@boost//:smart_ptr",
        "@boost//:timer",
        "//math:base",
        "//math/vandermonde:fft_solver",
        "//math/vandermonde:quad_solver",
    ],
)
