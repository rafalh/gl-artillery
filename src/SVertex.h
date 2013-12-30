#ifndef SVERTEX_H_INCLUDED
#define SVERTEX_H_INCLUDED

struct SVertex
{
    SVertex() {}
    SVertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv, uint32_t c = 0xFFFFFF):
        Pos(p), Normal(n), UV(uv), Clr(c) {}
    
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 UV;
    uint32_t Clr;
};

#endif // SVERTEX_H_INCLUDED
