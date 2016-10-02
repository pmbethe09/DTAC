package(default_visibility = ["//visibility:public"])

TESTS = glob(["Test*.cpp"])

TEST_LIB = ["BridgeTestBase.cpp"]

TEST_LIB_HDRS = ["BridgeTestBase.hpp"]

MAINS = [
    "ClaimStats.cpp",
    "PbnRunner.cpp",
    "runRandom.cpp",
]

HDRS = [
    "Asserts.hpp",
    "Constants.hpp",
    "Exceptions.hpp",
]

cc_library(
    name = "hdrs",
    hdrs = HDRS,
)

HAND_HDRS = ["Hand.hpp"]

HAND_SRCS = ["Hand.cpp"]

cc_library(
    name = "hand",
    srcs = HAND_SRCS,
    hdrs = HAND_HDRS,
    copts = ["-Wno-sign-compare"],  # TODO -- fix
    deps = [":hdrs"],
)

cc_library(
    name = "dtac",
    srcs = glob(
        ["*.cpp"],
        exclude = MAINS + TESTS + HAND_SRCS + TEST_LIB,
    ),
    hdrs = glob(
        ["*.hpp"],
        exclude = HDRS + HAND_HDRS + TEST_LIB_HDRS,
    ),
    copts = ["-Wno-sign-compare"],  # TODO -- fix
    deps = [
        ":hand",
        ":hdrs",
    ],
    linkopts = ["-lm"],
)

[
    cc_binary(
        name = m[:-4],
        srcs = [m],
        copts = ["-Wno-sign-compare"],  # TODO -- fix
        deps = [":dtac"],
    )
    for m in MAINS
]

cc_library(
    name = "testlib",
    testonly = 1,
    srcs = TEST_LIB,
    hdrs = TEST_LIB_HDRS,
    deps = [
        "//external:gtest",
        ":dtac",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = TESTS,
    deps = [
        "//external:gtest",
        ":dtac",
        ":testlib",
    ],
    copts = ["-Wno-sign-compare"],  # TODO -- fix
    size = "small",
)

genrule(
    name = "gentest",
    testonly = 1,
    outs = ["runtest.sh"],
    cmd = "echo '$$@' > $@",
)

test_suite(
    name = "all_tests",
    tags = ["-flaky"],
)

sh_test(
    name = "test_pbn",
    size = "small",
    srcs = [":gentest"],
    args = ["$(location :PbnRunner) $(location //dtac/testdata)"],
    data = [
        ":PbnRunner",
        "//dtac/testdata",
    ],
)

sh_test(
    name = "test_stats",
    size = "small",
    srcs = [":gentest"],
    args = ["$(location :ClaimStats) $(location //dtac/testdata)"],
    data = [
        ":ClaimStats",
        "//dtac/testdata",
    ],
)
