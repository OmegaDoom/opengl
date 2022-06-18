#ifndef VERTEX_WEIGHT_HPP
#define VERTEX_WEIGHT_HPP

class VertexWeight
{
public:
    VertexWeight(unsigned vertexId, float weight);
    unsigned VertexId() const;
    float Weight() const;
private:
    unsigned m_vertexId;
    float m_weight;
}; 

#endif //VERTEX_WEIGHT_HPP
