#ifndef SHADER_HXX
#define SHADER_HXX
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
class Shader
{
public:
    //程序id
    GLuint ID;
    Shader(const GLchar* vertexPath,const GLchar* fragmentPath,const char* geometryPath=nullptr);
    //激活程序
    void use();
    //uniform工具函数
    void setBool(const std::string &name,bool value) const;
    void setInt(const std::string &name,int value) const;
    void setFloat(const std::string &name,float value) const;
    // ------------------------------------------------------------------------
    void setVec2(const string &name,const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w);
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
private:
    void checkCompileErrors(GLuint shader, string type);
private:
    //从文件路径中获取顶点/片段着色器
    string vertexCode;
    string fragmentCode;
    string geometryCode;

    ifstream vShaderFile;
    ifstream fShaderFile;
    ifstream gShaderFile;
};
#endif // SHADER_HXX
