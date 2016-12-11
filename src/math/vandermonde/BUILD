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
    deps = [
        ":solver",
        "//math/fft:algorithm",
        "//util:smart_assert",
    ],
    visibility = [
        "//interp:__pkg__",
        "//operation/vandermonde:__pkg__",
    ],
)

cc_library(
    name = "quad_solver",
    hdrs = [
        "QuadSolver.h",
        "QuadSolver-Impl.h",
    ],
    deps = [
        ":solver",
    ],
    visibility = [
        "//interp:__pkg__",
        "//operation/vandermonde:__pkg__",
    ],
)

