package(default_visibility = ["//visibility:public"])

java_library(
    name = "local",
    srcs = glob(["**/*.java"]),
    deps = [
        "//bridge_proto:bridge_java_proto",
        "//cards_proto:cards_java_proto",
        "//java/edu/nyu/bridge/player",
        "//java/edu/nyu/bridge/scoring",
        "//java/edu/nyu/bridge/util",
        "//java/edu/nyu/cards",
        "//third_party/auto_value",
        "@guava_repo//jar",
        "@guice_repo//jar",
        "@jsr305_repo//jar",
        "@jsr330_repo//jar",
    ],
)
