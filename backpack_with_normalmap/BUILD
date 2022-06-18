cc_binary(
    name = "backpack_with_normalmap",
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
