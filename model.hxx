#ifndef MODEL_HXX
#define MODEL_HXX
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <map>

#include "stb_image.h"
#include "mesh.hxx"
#include "shader.hxx"
using namespace std;

class Model
{
public:
    Model();
    ~Model();
    void Draw(const Shader& shader) const;
    bool loadModel(string const &path);    //加载文件
private:
    bool processNode(aiNode* node,const aiScene* scene);    //处理递归节点
    Mesh processMesh(aiMesh* mesh,const aiScene* scene);    //遍历mesh
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    GLuint TextureFromFile(const char* path,const string &directory);
private:
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;    //所有的mesh数据
    string directory;   //保存加载文件的对应目录
};

#endif // MODEL_HXX
