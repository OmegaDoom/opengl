cc_import(
    name = "assimp_import",
    shared_library = "bin/assimp-vc142-mt.dll",
    interface_library = "lib/assimp-vc142-mt.lib",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "assimp_windows",
    hdrs = glob(["include/assimp/**/*.h", "include/assimp/**/*.hpp", "include/assimp/**/*.inl"]),
    deps = ["assimp_import"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
