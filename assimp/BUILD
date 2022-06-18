cc_library(
    name = "assimp",
    deps = select(
	{
            "@platforms//os:linux": ["@assimp_linux"],
            "@platforms//os:windows": ["@assimp_windows"],
	},
    ),
    visibility = ["//visibility:public"],
)
