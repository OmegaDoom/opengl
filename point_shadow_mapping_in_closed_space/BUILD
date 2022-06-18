cc_binary(
    name = "point_shadow_mapping_in_closed_space",
    srcs = glob([
        "src/*.cpp",
        "src/*.hpp",
        "src/*.h",
    ]),
    data = glob([
        "resources/**/*",
        "shaders/*",
    ]),
    deps = [
        "//assimp",
        "//gl",
        "//glew",
        "//glfw",
        "//glm",
    ],
)
