# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "_freqt",
    srcs = ["_freqt.cc"],
    hdrs = ["freqt.hpp"],
    deps = [
        "//lib:common",
    ],
)

cc_binary(
    name = "freqt",
    srcs = ["freqt.cc"],
    deps = [
        "//lib:common",
        ":_freqt",
    ],
)

cc_library(
    name = "_freqtGO",
    srcs = ["_freqtGO.cc"],
    hdrs = ["freqtGO.hpp"],
    copts = ["-O3"],
    deps = [
        "//lib:common",
        "@openblas_lib//:dylib",
        "@openblas_hdrs//:hdrs",
    ],
)

cc_binary(
    name = "freqtGO",
    srcs = ["freqtGO.cc"],
    copts = ["-O3"],
    deps = [
        "//lib:common",
        ":_freqtGO",
    ],
)
