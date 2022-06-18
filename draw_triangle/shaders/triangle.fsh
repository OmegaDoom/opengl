#version 330 core
in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D diffuse;

uniform int screen_width;
uniform int screen_height;
uniform vec2 point0;
uniform vec2 point1;
uniform vec2 point2;
uniform vec2 tex0;
uniform vec2 tex1;
uniform vec2 tex2;

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
vec3 Barycentric(vec3 a, vec3 b, vec3 c, vec3 p)
{
    vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    return vec3(u, v, w);
}

void main()
{
    float frag_x = 2.f * (gl_FragCoord.x / screen_width) - 1.f;
    float frag_y = 2.f * (gl_FragCoord.y / screen_height) - 1.f;

    vec3 barycentric = Barycentric(vec3(point0, 0), vec3(point1, 0), vec3(point2, 0), vec3(frag_x, frag_y, 0));

    bool point_is_inside = barycentric.x >= 0.0f && barycentric.z >= 0.0f && (barycentric.x + barycentric.z) <= 1.0f;

    if (point_is_inside)
    {
        vec2 tex_coords = tex0 * barycentric.x + tex1 * barycentric.y + tex2 * barycentric.z;
        fragColor = texture(diffuse, tex_coords);
    }
    else
    {
        discard;
    }
}