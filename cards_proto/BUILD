package(default_visibility = ["//visibility:public"])

load("//proto:protobuf.bzl", "cc_proto_library")
load("//proto:java_proto_library.bzl", "java_proto_library")

cc_proto_library(
    name = "cards_proto_cc",
    srcs = ["cards.proto"],
    protoc = "@com_github_google_protobuf//:protoc",
    default_runtime="@com_github_google_protobuf//:protobuf",
)

java_proto_library(
    name = "cards_proto_java",
    srcs = ["cards.proto"],
)
