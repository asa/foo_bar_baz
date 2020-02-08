load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

# Change master to the git tag you want.
git_repository(
    name = "com_grail_bazel_toolchain",
    commit = "2200d5315ddd3071985a2e82e574da525617867e",
    remote = "https://github.com/grailbio/bazel-toolchain.git",
)

load("@com_grail_bazel_toolchain//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    llvm_version = "8.0.0",
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

git_repository(
    name = "com_github_nelhage_rules_boost",
    #    commit = "417642961150e987bc1ac78c7814c617566ffdaa",
    commit = "9eff8676ce431232ae625ee93c4123f7c03477eb",  # 1.71
    remote = "https://github.com/nelhage/rules_boost",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

###############################################################
git_repository(
    name = "immer",
    remote = "https://github.com/arximboldi/immer.git",
    tag = "v0.6.1",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

http_file(
    name = "doctest",
    downloaded_file_path = "doctest.h",
    sha256 = "09ebd80d94758824cc615910cf686ebe5a43b8bd5a1c167359ef89fa6e13f13b",
    urls = ["https://raw.githubusercontent.com/onqtam/doctest/2.3.6/doctest/doctest.h"],
)

http_file(
    name = "catch2",
    downloaded_file_path = "catch.hpp",
    urls = ["https://github.com/catchorg/Catch2/releases/download/v2.11.0/catch.hpp"],
)

http_archive(
    name = "clara",
    build_file = "@//:third_party/BUILD.clara",
    #url = "https://github.com/catchorg/Clara/releases/download/v1.1.5/clara.hpp"
    strip_prefix = "Clara-1.1.5",
    url = "https://github.com/catchorg/Clara/archive/v1.1.5.tar.gz",
)

##############################################################

git_repository(
    name = "zug",
    commit = "be20cae36e7e5876bf5bfb08b2a0562e1db3b546",
    remote = "https://github.com/arximboldi/zug",
    shallow_since = "1571094853 +0200",
)

git_repository(
    name = "lager",
    commit = "dbc1fde8b323537154e351a24da34a485e2ccfa0",
    remote = "https://github.com/arximboldi/lager.git",
)

new_git_repository(
    name = "cereal",
    build_file_content = """
cc_library(name="cereal",
        srcs=glob(["include/**/*.hpp"]),
        visibility = ["//visibility:public"],
        includes=["include"],
)
        """,
    remote = "https://github.com/USCiLab/cereal.git",
    tag = "v1.3.0",
)

new_git_repository(
    name = "scelta",
    build_file_content = """
cc_library(name="scelta",
        srcs=glob(["include/**/*.hpp"]),
        visibility = ["//visibility:public"],
        includes=["include"],
)
        """,
    commit = "a0f4f701a33d03d6a9127da5c77e81d598bc7931",
    remote = "https://github.com/SuperV1234/scelta.git",
    shallow_since = "1557561051 +0100",
)
