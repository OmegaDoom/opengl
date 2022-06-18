cc_library(
    name = "glew",
    deps = select(
	{
            "@platforms//os:linux": ["@glew_linux"],
            "@platforms//os:windows": ["@glew_windows"],
	},
    ),
    visibility = ["//visibility:public"],
)
