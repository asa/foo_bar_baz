cc_library(
    name = "api",
    srcs = [
        "api.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//common",
    ],
)

cc_library(
    name = "codec",
    srcs = [
        "codec.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//common",
        "//net/api",
        "@cereal",
    ],
)
