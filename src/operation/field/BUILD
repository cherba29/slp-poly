cc_library(
    name = "benchmark",
    hdrs = [
        "benchmark.h",
        "FieldOperations.h",
        "Operation.h",
    ],
    deps = [
        "//complexity:equation",
        "//complexity:tracker",
        "//external:boost-timer",
        "//math:base",
        "//util:log",
    ],
    visibility = [
        "//run:__pkg__",
    ]
)

cc_library(
    name = "setup",
    hdrs = [
        "setup.h"
    ],
    deps = [
        "//math/field",
        "//math/field:field_uint",
        "//math/field:field_uint2x",
        "//math/field:crt",
        "//math/field:crt2",
        "//math/field/util",
        "//external:boost-mpl",
    ],
    visibility = [
        "//interp:__pkg__",
        "//math:__pkg__",
    ],
)