package(default_visibility = ["//visibility:public"])

java_library(
    name = "scoring",
    srcs = glob(["**/*.java"]),
    deps = [
        "//bridge_proto:bridge_proto_java",
        "//cards_proto:cards_proto_java",
        "//java/edu/nyu/bridge/util",
        "//java/edu/nyu/cards",
        "//third_party/auto_value",
        "@guava_repo//jar",
        "@jsr305_repo//jar",
    ],
)
