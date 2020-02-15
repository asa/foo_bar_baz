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
        "//net:client",
        "//net:svc",
        "//net:ws",
        "@boost//:asio",
        "@boost//:thread",
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
