package(default_visibility = ["//visibility:public"])

load("@com_google_protobuf//:protobuf.bzl", "cc_proto_library")

cc_proto_library(
    name = "bridge_proto_cc",
    srcs = ["bridge.proto"],
    default_runtime = "@com_google_protobuf//:protobuf",
    protoc = "@com_google_protobuf//:protoc",
)

proto_library(
    name = "bridge_proto",
    srcs = ["bridge.proto"],
)

java_proto_library(
    name = "bridge_proto_java",
    deps = [":bridge_proto"],
)
