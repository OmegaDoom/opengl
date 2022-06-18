cc_library(
    name = "glm",
    deps = select(
	{
            "@platforms//os:linux": [],
            "@platforms//os:windows": ["@glm_windows"],
	},
    ),
    visibility = ["//visibility:public"],
)
