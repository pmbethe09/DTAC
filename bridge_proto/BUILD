package(default_visibility = ["//visibility:public"])

load("@com_github_google_protobuf//:protobuf.bzl", "cc_proto_library")
load("//proto:java_proto_library.bzl", "java_proto_library")

cc_proto_library(
    name = "bridge_proto_cc",
    srcs = ["bridge.proto"],
    default_runtime = "@com_github_google_protobuf//:protobuf",
    protoc = "@com_github_google_protobuf//:protoc",
)

java_proto_library(
    name = "bridge_proto_java",
    srcs = ["bridge.proto"],
)
