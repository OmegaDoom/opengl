#version 420 core

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

out GS_OUT
{
    vec2 texCoords;
    flat vec3 normalCoords;
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

void EmitTriangle(vec4 point0, vec4 point1, vec4 point2, mat4 normal, mat4 viewModel, mat4 projViewModel)
{
    vec3 norm = mat3(normal) * (cross(point1.xyz - point0.xyz, point2.xyz - point0.xyz));
    gl_Position = projViewModel * point0;
    gs_out.normalCoords = norm;
    gs_out.fragPos = vec3(viewModel * point0);
    EmitVertex();

    gl_Position = projViewModel * point1;
    gs_out.fragPos = vec3(viewModel * point1);
    EmitVertex();

    gl_Position = projViewModel * point2;
    gs_out.fragPos = vec3(viewModel * point2);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    const float radius = 4.f;
    vec4 center = gl_in[0].gl_Position;
    vec4 first = vec4(center.x, center.y + radius, center.z, 1.f);
    vec4 second = vec4(center.x, center.y - (0.5 * radius), center.z + (0.866 * radius), 1.f);
    vec4 third = vec4(center.x - (0.866 * radius), center.y - (0.5 * radius), center.z - (0.433 * radius), 1.f);
    vec4 fourth = vec4(center.x + (0.866 * radius), center.y - (0.5 * radius), center.z - (0.433 * radius), 1.f);

    mat4 projViewModel = projection * view * model;
    mat4 viewModel = view * model;

    EmitTriangle(second, fourth, first, normal, viewModel, projViewModel);
    EmitTriangle(fourth, third, first, normal, viewModel, projViewModel);
    EmitTriangle(third, second, first, normal, viewModel, projViewModel);
    EmitTriangle(fourth, second, third, normal, viewModel, projViewModel);
}
