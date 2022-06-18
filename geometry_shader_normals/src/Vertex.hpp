#ifndef VERTEX_HPP
#define VERTEX_HPP

struct vertex_coords
{
    float x;
    float y;
    float z;
};

struct vertex_color 
{
    float r;
    float g;
    float b;
};

struct vertex_tex_coords
{
    float s;
    float t;
};

struct vertex_normal_coords
{
    float nx;
    float ny;
    float nz;
};

struct vertex_bones_info
{
    int ids[4];
    float weights[4];
};

struct vertex
{
    vertex_coords coords;
    vertex_color color;
    vertex_tex_coords tex_coords;
    vertex_normal_coords normal_coords;
    vertex_bones_info bones_info;
};

#endif //VERTEX_HPP
