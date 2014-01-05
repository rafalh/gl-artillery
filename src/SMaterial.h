#ifndef SMATERIAL_H_INCLUDED
#define SMATERIAL_H_INCLUDED

struct SMaterial
{
    SMaterial():
        Texture(0),
        AmbientColor(0.0f, 0.0f, 0.0f),
        DiffuseColor(1.0f, 1.0f, 1.0f),
        SpecularColor(0.0f, 0.0f, 0.0f),
        Shininess(1.0f) {}
    
    GLuint Texture;
    glm::vec3 AmbientColor;
    glm::vec3 DiffuseColor;
    glm::vec3 SpecularColor;
    float Shininess; // Never set to 0!
};

#endif // SMATERIAL_H_INCLUDED
