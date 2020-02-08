cc_library(
    name = "core",
    srcs = [
        "core.cc",
        "core.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//bar",
        "//baz",
        "//common",
        "//foo",
        "@boost//:asio",
        "@lager",
        "@scelta",
        "@zug",
    ],
)

cc_binary(
    name = "app",
    srcs = [
        "test.cc",
        "@doctest//file",
    ],
    deps = [
        ":core",
    ],
)
