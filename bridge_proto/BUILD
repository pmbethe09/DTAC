load("@rules_go//proto:def.bzl", "go_proto_library")
load("@rules_python//python:proto.bzl", "py_proto_library")

package(default_visibility = ["//visibility:public"])

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

py_proto_library(
    name = "bridge_py_proto",
    deps = [":bridge_proto"],
)
