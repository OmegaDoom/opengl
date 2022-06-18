cc_binary(
    name = "parallax_occlusion_mapping",
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
