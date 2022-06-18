cc_binary(
    name = "draw_triangle",
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
        "//gl",
        "//glew",
        "//glfw",
        "//glm",
    ],
)
