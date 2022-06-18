#include "VertexWeight.hpp"

VertexWeight::VertexWeight(unsigned vertexId, float weight)
    : m_vertexId{ vertexId }
    , m_weight{ weight }
{
}

unsigned VertexWeight::VertexId() const
{
    return m_vertexId;
}

float VertexWeight::Weight() const
{
    return m_weight;
}
