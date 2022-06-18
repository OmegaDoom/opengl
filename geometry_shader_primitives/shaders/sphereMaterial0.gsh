#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 80) out;
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

const float radius = 2.f;

void EmitPoint(vec4 point, mat4 normal, mat4 viewModel, mat4 projViewModel)
{
    vec4 new_point = vec4(normalize(point.xyz) * radius, 1.f);
    gl_Position = projViewModel * new_point;
    gs_out.normalCoords = mat3(normal) * normalize(point.xyz);
    gs_out.fragPos = vec3(viewModel * new_point);
    EmitVertex();
}

void getTessellation(vec4 point0, vec4 point1, vec4 point2, out vec4 result_point0, out vec4 result_point1, out vec4 result_point2)
{
    result_point0 = vec4(normalize((point0.xyz + point1.xyz) * 0.5f), 1.f);
    result_point1 = vec4(normalize((point1.xyz + point2.xyz) * 0.5f), 1.f);
    result_point2 = vec4(normalize((point2.xyz + point0.xyz) * 0.5f), 1.f);
}

void tessellate(vec4 point0, vec4 point1, vec4 point2, mat4 normal, mat4 viewModel, mat4 projViewModel)
{
    vec4 points[66];
    points[0] = point0;
    points[1] = point1;
    points[2] = point2;

    getTessellation(points[0], points[1], points[2], points[3], points[4], points[5]);

    getTessellation(points[0], points[3], points[5], points[6], points[7], points[8]);
    getTessellation(points[3], points[4], points[5], points[9], points[10], points[11]);
    getTessellation(points[3], points[1], points[4], points[12], points[13], points[14]);
    getTessellation(points[5], points[4], points[2], points[15], points[16], points[17]);

    getTessellation(points[0], points[6], points[8], points[18], points[19], points[20]);
    getTessellation(points[6], points[7], points[8], points[21], points[22], points[23]);
    getTessellation(points[6], points[3], points[7], points[24], points[25], points[26]);
    getTessellation(points[3], points[9], points[11], points[27], points[28], points[29]);
    getTessellation(points[3], points[12], points[14], points[30], points[31], points[32]);
    getTessellation(points[12], points[13], points[14], points[33], points[34], points[35]);
    getTessellation(points[12], points[1], points[13], points[36], points[37], points[38]);

    getTessellation(points[8], points[7], points[5], points[39], points[40], points[41]);
    getTessellation(points[7], points[10], points[5], points[42], points[43], points[44]);
    getTessellation(points[11], points[9], points[10], points[45], points[46], points[47]);
    getTessellation(points[9], points[4], points[10], points[48], points[49], points[50]);
    getTessellation(points[14], points[13], points[4], points[51], points[52], points[53]);

    getTessellation(points[5], points[15], points[17], points[54], points[55], points[56]);
    getTessellation(points[15], points[16], points[17], points[57], points[58], points[59]);
    getTessellation(points[10], points[4], points[16], points[60], points[61], points[62]);

    getTessellation(points[17], points[16], points[2], points[63], points[64], points[65]);

    // first batch from top to down right
    EmitPoint(points[0], normal, viewModel, projViewModel);
    EmitPoint(points[18], normal, viewModel, projViewModel);
    EmitPoint(points[20], normal, viewModel, projViewModel);
    EmitPoint(points[19], normal, viewModel, projViewModel);
    EmitPoint(points[8], normal, viewModel, projViewModel);
    EmitPoint(points[22], normal, viewModel, projViewModel);
    EmitPoint(points[41], normal, viewModel, projViewModel);
    EmitPoint(points[40], normal, viewModel, projViewModel);
    EmitPoint(points[5], normal, viewModel, projViewModel);
    EmitPoint(points[43], normal, viewModel, projViewModel);
    EmitPoint(points[56], normal, viewModel, projViewModel);
    EmitPoint(points[55], normal, viewModel, projViewModel);
    EmitPoint(points[17], normal, viewModel, projViewModel);
    EmitPoint(points[58], normal, viewModel, projViewModel);
    EmitPoint(points[65], normal, viewModel, projViewModel);
    EmitPoint(points[64], normal, viewModel, projViewModel);
    EmitPoint(points[2], normal, viewModel, projViewModel);
    EndPrimitive();

    // second batch
    EmitPoint(points[18], normal, viewModel, projViewModel);
    EmitPoint(points[6], normal, viewModel, projViewModel);
    EmitPoint(points[19], normal, viewModel, projViewModel);
    EmitPoint(points[21], normal, viewModel, projViewModel);
    EmitPoint(points[22], normal, viewModel, projViewModel);
    EmitPoint(points[7], normal, viewModel, projViewModel);
    EmitPoint(points[40], normal, viewModel, projViewModel);
    EmitPoint(points[42], normal, viewModel, projViewModel);
    EmitPoint(points[43], normal, viewModel, projViewModel);
    EmitPoint(points[10], normal, viewModel, projViewModel);
    EmitPoint(points[55], normal, viewModel, projViewModel);
    EmitPoint(points[57], normal, viewModel, projViewModel);
    EmitPoint(points[58], normal, viewModel, projViewModel);
    EmitPoint(points[16], normal, viewModel, projViewModel);
    EmitPoint(points[64], normal, viewModel, projViewModel);
    EndPrimitive();

    // third batch
    EmitPoint(points[6], normal, viewModel, projViewModel);
    EmitPoint(points[24], normal, viewModel, projViewModel);
    EmitPoint(points[21], normal, viewModel, projViewModel);
    EmitPoint(points[25], normal, viewModel, projViewModel);
    EmitPoint(points[7], normal, viewModel, projViewModel);
    EmitPoint(points[28], normal, viewModel, projViewModel);
    EmitPoint(points[42], normal, viewModel, projViewModel);
    EmitPoint(points[46], normal, viewModel, projViewModel);
    EmitPoint(points[10], normal, viewModel, projViewModel);
    EmitPoint(points[49], normal, viewModel, projViewModel);
    EmitPoint(points[57], normal, viewModel, projViewModel);
    EmitPoint(points[61], normal, viewModel, projViewModel);
    EmitPoint(points[16], normal, viewModel, projViewModel);
    EndPrimitive();

    // fourth batch
    EmitPoint(points[24], normal, viewModel, projViewModel);
    EmitPoint(points[3], normal, viewModel, projViewModel);
    EmitPoint(points[25], normal, viewModel, projViewModel);
    EmitPoint(points[27], normal, viewModel, projViewModel);
    EmitPoint(points[28], normal, viewModel, projViewModel);
    EmitPoint(points[9], normal, viewModel, projViewModel);
    EmitPoint(points[46], normal, viewModel, projViewModel);
    EmitPoint(points[48], normal, viewModel, projViewModel);
    EmitPoint(points[49], normal, viewModel, projViewModel);
    EmitPoint(points[4], normal, viewModel, projViewModel);
    EmitPoint(points[61], normal, viewModel, projViewModel);
    EndPrimitive();

    // fifth batch
    EmitPoint(points[3], normal, viewModel, projViewModel);
    EmitPoint(points[30], normal, viewModel, projViewModel);
    EmitPoint(points[27], normal, viewModel, projViewModel);
    EmitPoint(points[31], normal, viewModel, projViewModel);
    EmitPoint(points[14], normal, viewModel, projViewModel);
    EmitPoint(points[34], normal, viewModel, projViewModel);
    EmitPoint(points[48], normal, viewModel, projViewModel);
    EmitPoint(points[52], normal, viewModel, projViewModel);
    EmitPoint(points[4], normal, viewModel, projViewModel);
    EndPrimitive();

    // sixth batch
    EmitPoint(points[30], normal, viewModel, projViewModel);
    EmitPoint(points[12], normal, viewModel, projViewModel);
    EmitPoint(points[31], normal, viewModel, projViewModel);
    EmitPoint(points[33], normal, viewModel, projViewModel);
    EmitPoint(points[34], normal, viewModel, projViewModel);
    EmitPoint(points[13], normal, viewModel, projViewModel);
    EmitPoint(points[52], normal, viewModel, projViewModel);
    EndPrimitive();

    // seventh batch
    EmitPoint(points[12], normal, viewModel, projViewModel);
    EmitPoint(points[36], normal, viewModel, projViewModel);
    EmitPoint(points[33], normal, viewModel, projViewModel);
    EmitPoint(points[37], normal, viewModel, projViewModel);
    EmitPoint(points[13], normal, viewModel, projViewModel);
    EndPrimitive();

    // eighth batch
    EmitPoint(points[36], normal, viewModel, projViewModel);
    EmitPoint(points[1], normal, viewModel, projViewModel);
    EmitPoint(points[37], normal, viewModel, projViewModel);
    EndPrimitive();
}

void main()
{
    vec4 center = gl_in[0].gl_Position;
    float local_radius = 1.f;
    vec4 point0 = vec4(vec3(center.x - local_radius, center.y - local_radius, center.z - local_radius), 1.f);
    vec4 point1 = vec4(vec3(center.x + local_radius, center.y - local_radius, center.z - local_radius), 1.f);
    vec4 point2 = vec4(vec3(center.x + local_radius, center.y - local_radius, center.z + local_radius), 1.f);
    vec4 point3 = vec4(vec3(center.x - local_radius, center.y - local_radius, center.z + local_radius), 1.f);
    vec4 point4 = vec4(vec3(center.x - local_radius, center.y + local_radius, center.z - local_radius), 1.f);
    vec4 point5 = vec4(vec3(center.x + local_radius, center.y + local_radius, center.z - local_radius), 1.f);
    vec4 point6 = vec4(vec3(center.x + local_radius, center.y + local_radius, center.z + local_radius), 1.f);
    vec4 point7 = vec4(vec3(center.x - local_radius, center.y + local_radius, center.z + local_radius), 1.f);

    mat4 projViewModel = projection * view * model;
    mat4 viewModel = view * model;

    if (0 == gl_InvocationID)
    {
        tessellate(point0, point1, point2, normal, viewModel, projViewModel);
    }

    if (1 == gl_InvocationID)
    {
        tessellate(point2, point3, point0, normal, viewModel, projViewModel);
    }

    if (2 == gl_InvocationID)
    {
        tessellate(point4, point7, point6, normal, viewModel, projViewModel);
    }

    if (3 == gl_InvocationID)
    {
        tessellate(point6, point5, point4, normal, viewModel, projViewModel);
    }

    if (4 == gl_InvocationID)
    {
        tessellate(point0, point3, point7, normal, viewModel, projViewModel);
    }

    if (5 == gl_InvocationID)
    {
        tessellate(point7, point4, point0, normal, viewModel, projViewModel);
    }

    if (6 == gl_InvocationID)
    {
        tessellate(point2, point1, point5, normal, viewModel, projViewModel);
    }

    if (7 == gl_InvocationID)
    {
        tessellate(point5, point6, point2, normal, viewModel, projViewModel);
    }

    if (8 == gl_InvocationID)
    {
        tessellate(point3, point2, point6, normal, viewModel, projViewModel);
    }

    if (9 == gl_InvocationID)
    {
        tessellate(point6, point7, point3, normal, viewModel, projViewModel);
    }

    if (10 == gl_InvocationID)
    {
        tessellate(point1, point0, point4, normal, viewModel, projViewModel);
    }

    if (11 == gl_InvocationID)
    {
        tessellate(point4, point5, point1, normal, viewModel, projViewModel);
    }
}