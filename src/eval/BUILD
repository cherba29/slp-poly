load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file = "InstructionEnum.yaml",
    visibility = [
        "//eval/util:__pkg__",
    ],
)

cc_library(
    name = "code_generator",
    hdrs = [
        "CodeGenerator.h",
        "CodeGenerator-Impl.h",
    ],
    visibility = [
        "//interp:__pkg__",
        "//run:__pkg__",
    ],
    deps = [
        "//context:interp_context",
        "//context:printer",
        "//eval/util:data_allocator",
        "//eval/util:instruction_printer",
    ],
)

cc_library(
    name = "evaluator",
    hdrs = [
        "Evaluator.h",
    ],
    visibility = [
        "//operation/eval:__pkg__",
    ],
    deps = [
        ":instruction",
        "//eval/util:code_annotation",
        "//math/field/util",
    ],
)

cc_library(
    name = "instruction",
    hdrs = [
        "Instruction.h",
    ],
    visibility = [
        "//eval/util:__pkg__",
    ],
    deps = [
        ":InstructionEnum",
    ],
)

cc_library(
    name = "univ_evaluator",
    hdrs = [
        "UnivEvaluator.h",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
    deps = [
        ":evaluator",
    ],
)
