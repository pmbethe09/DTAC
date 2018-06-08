package(default_visibility = ["//visibility:public"])

load("@io_bazel_rules_go//proto:def.bzl", "go_proto_library")

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
