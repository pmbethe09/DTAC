package(default_visibility = ["//visibility:public"])

java_library(
    name = "util",
    srcs = glob(["**/*.java"]),
    deps = [
        "//bridge_proto:bridge_java_proto",
        "//cards_proto:cards_java_proto",
        "//java/edu/nyu/cards",
        "//third_party/auto_value",
        "@guava_repo//jar",
        "@jsr305_repo//jar",
    ],
)
