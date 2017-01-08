cc_library(
    name = "code_annotation",
    hdrs = [
        "CodeAnnotation.h",
    ],
    visibility = [
        "//eval:__pkg__",
    ],
    deps = [
        "//util:log",
    ],
)

cc_library(
    name = "data_allocator",
    hdrs = [
        "DataAllocator.h",
    ],
    visibility = [
        "//eval:__pkg__",
    ],
    deps = [
    ],
)

cc_library(
    name = "instruction_counter",
    hdrs = [
        "InstructionCounter.h",
    ],
    visibility = [
        "//run:__pkg__",
    ],
    deps = [
        "//eval:InstructionEnum",
        "//eval:instruction",
    ],
)

cc_library(
    name = "instruction_max_degree",
    hdrs = [
        "InstructionMaxDegree.h",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
    deps = [
        "//eval:instruction",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "instruction_printer",
    hdrs = [
        "InstructionPrinter.h",
    ],
    visibility = [
        "//eval:__pkg__",
    ],
    deps = [
        "//math/field/util",
    ],
)
