cc_library(
    name = "solver",
    hdrs = [
        "Solver.h",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
)

cc_library(
    name = "fft_solver",
    hdrs = [
        "FFTSolver.h",
        "FFTSolver-Impl.h",
    ],
    visibility = [
        "//interp:__pkg__",
        "//math:__pkg__",
        "//operation/vandermonde:__pkg__",
    ],
    deps = [
        ":solver",
        "//math/fft:algorithm",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "quad_solver",
    hdrs = [
        "QuadSolver.h",
        "QuadSolver-Impl.h",
    ],
    visibility = [
        "//interp:__pkg__",
        "//math:__pkg__",
        "//operation/vandermonde:__pkg__",
    ],
    deps = [
        ":solver",
    ],
)
