#ifndef VERTEX_HPP
#define VERTEX_HPP

struct vertex_coords
{
    float x;
    float y;
};

struct vertex_color
{
    float r;
    float g;
    float b;
};

struct vertex
{
    vertex_coords coords;
    vertex_color color;
};

#endif //VERTEX_HPP
