#ifndef MESH_HXX
#define MESH_HXX
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "shader.hxx"

using namespace std;
//每个顶点存储的结构
struct Vertex
{
    glm::vec3 Position; //位置
    glm::vec3 Normal;   //法线
    glm::vec2 TexCoords;    //uv
};
//纹理结构
struct Texture
{
    GLuint id;
    aiTextureType type;    //类型
    string path;  //存储加载过的纹理路径
};
//纹理类型
enum ETextureType
{
    ETextureTypeDiffuse=1,  //漫反射
    ETextureTypeSpecular=2 //高光反射
};

class Mesh
{
public:
    Mesh(vector<Vertex> vertices,vector<GLuint> indices,vector<Texture> texture);
    void Draw(const Shader& shader) const;
    void final() const;
private:
    GLuint VAO,VBO,EBO;
    void setupMesh();

    vector<Vertex> vertices;    //顶点法线uv数据
    vector<GLuint> indices; //顶点索引数据
    vector<Texture> textures;   //纹理数据
};

#endif // MESH_HXX
