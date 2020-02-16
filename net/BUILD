cc_library(
    name = "client",
    srcs = [
        "client.cc",
        "client.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":api",
        "//foo",
        "@lager",
        "@scelta",
        "@zug",
    ],
)

cc_library(
    name = "api",
    srcs = [
        "api.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//common",
        "@cereal",
    ],
)

# the remote data service
cc_library(
    name = "svc",
    srcs = [
        "svc.cc",
        "svc.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":api",
        "@lager",
        "@scelta",
        "@zug",
    ],
)

cc_library(
    name = "ws",
    srcs = [
        "ws.cc",
        "ws.hh",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":api",
        "@lager",
        "@scelta",
        "@zug",
    ],
)
