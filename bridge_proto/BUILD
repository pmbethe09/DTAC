package(default_visibility = ["//visibility:public"])

load("@io_bazel_rules_go//proto:def.bzl", "go_proto_library")

cc_proto_library(
    name = "bridge_cc_proto",
    deps = [":bridge_proto"],
)

proto_library(
    name = "bridge_proto",
    srcs = ["bridge.proto"],
)

java_proto_library(
    name = "bridge_java_proto",
    deps = [":bridge_proto"],
)

go_proto_library(
    name = "bridge_go_proto",
    importpath = "github.com/pmbethe09/dtac/bridge_proto",
    proto = ":bridge_proto",
)
