cc_library(
    name = "code_annotation",
    hdrs = [
        "CodeAnnotation.h"
    ],
    deps = [
        "//util:log",
    ],
    visibility = [
        "//eval:__pkg__",
    ],
)

cc_library(
    name = "data_allocator",
    hdrs = [
        "DataAllocator.h"
    ],
    deps = [
    ],
    visibility = [
        "//eval:__pkg__",
    ],
)

cc_library(
    name = "instruction_counter",
    hdrs = [
        "InstructionCounter.h"
    ],
    deps = [
        "//eval:InstructionEnum",
        "//eval:instruction",
    ],
    visibility = [
        "//run:__pkg__",
    ],
)

cc_library(
    name = "instruction_max_degree",
    hdrs = [
        "InstructionMaxDegree.h"
    ],
    deps = [
        "//eval:instruction",
        "//util:smart_assert",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
)

cc_library(
    name = "instruction_printer",
    hdrs = [
        "InstructionPrinter.h"
    ],
    deps = [
        "//math/field/util",
    ],
    visibility = [
        "//eval:__pkg__",
    ],
)
