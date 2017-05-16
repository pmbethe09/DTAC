load("//rules:build_test.bzl", "build_test")

build_test(
    name = "binaries",
    targets = [
        "//dtac:runRandom",
        "//dtac:PbnRunner",
        "//dtac:ClaimStats",
    ],
)

test_suite(
    name = "tests",
    tests = [
        ":binaries",
        "//dtac:test_pbn",
        "//dtac:test_stats",
        "//dtac:unit_tests",
        "//javatests/edu/nyu/bridge/util",
        "//javatests/edu/nyu/cards",
        "//javatests/edu/nyu/cards/dealer",
    ],
)
