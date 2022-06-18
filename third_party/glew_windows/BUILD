cc_import(
    name = "glew_import",
    shared_library = "bin/Release/x64/glew32.dll",
    interface_library = "lib/Release/x64/glew32.lib",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "glew_windows",
    hdrs = glob(["include/GL/*.h"]),
    deps = ["glew_import"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)