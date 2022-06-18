#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 84) out;
layout(invocations = 12) in;

out GS_OUT
{
    smooth vec3 normalCoords;
    smooth vec3 fragPos;
} gs_out;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 view;
    uniform mat4 camera;
    uniform mat4 projection;
};

uniform mat4 model;
uniform mat4 normal;

const float radius = 3.f;

mat4 translationMatrix(vec3 delta)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(delta, 1.0));
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
                oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
                oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

void EmitPoint(vec4 point, mat4 normal, mat4 viewModel, mat4 projViewModel)
{
    vec4 new_point = vec4(normalize(point.xyz) * radius, 1.f);
    gl_Position = projViewModel * new_point;
    gs_out.normalCoords = mat3(normal) * normalize(point.xyz);
    gs_out.fragPos = vec3(viewModel * new_point);
    EmitVertex();
}

void draw_cube_side(mat4 localTransform, mat4 normal, mat4 viewModel, mat4 projViewModel)
{
    const int n = 8;
    const int m = 4;
    const float step = 1.f / n;
    for (int j = 0; j < m; ++j)
    {
        float x = 0.5f;
        float y0 = j * step;
        float y1 = y0 + step;
        vec4 point0 = localTransform * vec4(x, y0, 0.f, 1.f);
        vec4 point1 = localTransform * vec4(x, y1, 0.f, 1.f);
        EmitPoint(point0, normal, viewModel, projViewModel);
        EmitPoint(point1, normal, viewModel, projViewModel);
        for (int i = 0; i < n; ++i)
        {
            float x = 0.5f - i * step - step;
            float y0 = j * step;
            float y1 = y0 + step;
            vec4 point0 = localTransform * vec4(x, y0, 0.f, 1.f);
            vec4 point1 = localTransform * vec4(x, y1, 0.f, 1.f);
            EmitPoint(point0, normal, viewModel, projViewModel);
            EmitPoint(point1, normal, viewModel, projViewModel);
        }
        EndPrimitive();
    }
}

void main()
{
    vec4 center = gl_in[0].gl_Position;
    float local_radius = 1.f;
    mat4 projViewModel = projection * view * model;
    mat4 viewModel = view * model;
    const float pi = 3.1416;

    if (0 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, 0.f, 0.5f)), normal, viewModel, projViewModel);
    }

    if (1 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, -0.5f, 0.5f)), normal, viewModel, projViewModel);
    }

    if (2 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, 0.f, -0.5f)) * rotationMatrix(vec3(-1.f, 0.f, 0.f), pi), normal, viewModel, projViewModel);
    }

    if (3 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, 0.5f, -0.5f)) * rotationMatrix(vec3(-1.f, 0.f, 0.f), pi), normal, viewModel, projViewModel);
    }

    if (4 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(-0.5f, 0.0f, 0.f)) * rotationMatrix(vec3(0.f, 1.f, 0.f), pi * 0.5f) , normal, viewModel, projViewModel);
    }

    if (5 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(-0.5f, -0.5f, 0.f)) * rotationMatrix(vec3(0.f, 1.f, 0.f), pi * 0.5f) , normal, viewModel, projViewModel);
    }

    if (6 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.5f, 0.0f, 0.f)) * rotationMatrix(vec3(0.f, 1.f, 0.f), -pi * 0.5f) , normal, viewModel, projViewModel);
    }

    if (7 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.5f, -0.5f, 0.f)) * rotationMatrix(vec3(0.f, 1.f, 0.f), -pi * 0.5f) , normal, viewModel, projViewModel);
    }

    if (8 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, -0.5f, 0.f)) * rotationMatrix(vec3(1.f, 0.f, 0.f), -pi * 0.5f) , normal, viewModel, projViewModel);
    }

    if (9 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, -0.5f, -0.5f)) * rotationMatrix(vec3(1.f, 0.f, 0.f), -pi * 0.5f) , normal, viewModel, projViewModel);
    }

    if (10 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, 0.5f, 0.f)) * rotationMatrix(vec3(1.f, 0.f, 0.f), pi * 0.5f) , normal, viewModel, projViewModel);
    }

    if (11 == gl_InvocationID)
    {
        draw_cube_side(translationMatrix(vec3(0.f, 0.5f, 0.5f)) * rotationMatrix(vec3(1.f, 0.f, 0.f), pi * 0.5f) , normal, viewModel, projViewModel);
    }
}
