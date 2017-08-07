package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "bridge_proto_cc",
    deps = [":bridge_proto"],
)

proto_library(
    name = "bridge_proto",
    srcs = ["bridge.proto"],
)

java_proto_library(
    name = "bridge_proto_java",
    deps = [":bridge_proto"],
)
