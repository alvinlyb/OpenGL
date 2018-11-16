#include "shader.hxx"
#include "qdebug.h"
// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position, 1.0);\n"
    "ourColor = color;\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "uniform vec4 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = ourColor;\n"
    "}\n\0";

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const char* geometryPath)
{
    //保证ifstream对象可以抛出异常
    vShaderFile.exceptions(ifstream::failbit|ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit|ifstream::badbit);
    gShaderFile.exceptions(ifstream::failbit|ifstream::badbit);
    try{
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream,fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream<<vShaderFile.rdbuf();
        fShaderStream<<fShaderFile.rdbuf();
        //关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode=vShaderStream.str();
        fragmentCode=fShaderStream.str();

        if(geometryPath!=nullptr)
        {
            gShaderFile.open(geometryPath); //打开geometry文件
            stringstream gShaderStream;
            gShaderStream<<gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode=gShaderStream.str();
        }
    }
    catch(ifstream::failure e){
        //读取shader文件错误
        cout<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"<<endl;
    }
    const GLchar* vShaderCode=vertexCode.c_str();
    const GLchar* fShaderCode=fragmentCode.c_str();
    //测试shaderSource
//    const char* vShaderCode=vertexShaderSource;
//    const char* fShaderCode=fragmentShaderSource;
    //2.编译着色器
    GLuint vertex,fragment;
    //vertex shader
    vertex=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vShaderCode,NULL); //源化vertex
    glCompileShader(vertex);    //编译shader
    //检查编译shader是否错误
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    GLuint geometry;
    if(geometryPath!=nullptr)
    {
        const char* gShaderCode=geometryCode.c_str();
        geometry=glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry,1,&gShaderCode,NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry,"GEOMETRY");
    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);     //添加vertext到ID
    glAttachShader(ID, fragment);   //添加fragment到ID
    if(geometryPath!=nullptr)
        glAttachShader(ID,geometry);
    glLinkProgram(ID);  //链接程序
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geometryPath!=nullptr)
        glDeleteShader(geometry);
}
//激活shader
void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}

void Shader::setInt(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setFloat(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, string type)
{
    int success;
    char infoLog[512];

    if (type != "PROGRAM")
    {
        //检查glsl是否编译成功
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
