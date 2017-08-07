package(default_visibility = ["//visibility:public"])

#load("@com_google_protobuf//:protobuf.bzl", "cc_proto_library")

cc_proto_library(
    name = "cards_proto_cc",
    deps = [":cards_proto"],
)

proto_library(
    name = "cards_proto",
    srcs = ["cards.proto"],
)

java_proto_library(
    name = "cards_proto_java",
    deps = [":cards_proto"],
)
