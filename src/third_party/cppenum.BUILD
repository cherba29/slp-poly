
py_binary(
    name = "gen_cppenum",
    srcs = [
        "cppenum/gen.py",
    ],
    main = "cppenum/gen.py",
    data = [
        "templates/cpp.mako",
        "templates/h.mako",
        "templates/shared.mako",
        "templates/t.cpp.mako",
    ],
    visibility = ["//visibility:public"],
)
