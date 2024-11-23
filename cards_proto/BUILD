load("@rules_go//proto:def.bzl", "go_proto_library")
load("@rules_python//python:proto.bzl", "py_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "cards_cc_proto",
    deps = [":cards_proto"],
)

proto_library(
    name = "cards_proto",
    srcs = ["cards.proto"],
)

java_proto_library(
    name = "cards_java_proto",
    deps = [":cards_proto"],
)

go_proto_library(
    name = "cards_go_proto",
    importpath = "github.com/pmbethe09/dtac/cards_proto",
    proto = ":cards_proto",
)

py_proto_library(
    name = "cards_py_proto",
    deps = [":cards_proto"],
)
