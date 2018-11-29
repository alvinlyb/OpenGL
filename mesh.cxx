#include "mesh.hxx"

Mesh::Mesh(vector<Vertex> vertices,vector<GLuint> indices,vector<Texture> texture)
{
    this->vertices=vertices;
    this->indices=indices;
    this->textures=texture;
    this->setupMesh();
}
//绘制
void Mesh::Draw(const Shader &shader) const
{
    GLuint diffuseCnt=0,specularCnt=0,texUnitCnt=0;
    for(vector<Texture>::const_iterator it=this->textures.begin();this->textures.end()!=it;++it)
    {
        switch(it->type)
        {
        case aiTextureType_DIFFUSE:
        {
            glActiveTexture(GL_TEXTURE0+texUnitCnt);
            glBindTexture(GL_TEXTURE_2D,it->id);
            stringstream ss;
            ss<<"texture_diffuse"<<diffuseCnt++;
            shader.setFloat(ss.str().c_str(),diffuseCnt++);   //设置相对应的纹理uniform
            break;
        }
        case aiTextureType_SPECULAR:
        {
            glActiveTexture(GL_TEXTURE0+texUnitCnt);
            glBindTexture(GL_TEXTURE_2D,it->id);
            stringstream ss;
            ss<<"texture_specular"<<specularCnt++;
            shader.setFloat(ss.str().c_str(),specularCnt++);   //设置相对应的纹理uniform
            break;
        }
        default:
            break;
        }
    }

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES,this->indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}
//清除顶缓冲区
void Mesh::final() const
{
    glDeleteVertexArrays(1,&this->VAO);
    glDeleteBuffers(1,&this->VBO);
    glDeleteBuffers(1,&this->EBO);
}

//初始化模型数据
void Mesh::setupMesh()
{
    glGenVertexArrays(1,&this->VAO);
    glGenBuffers(1,&this->VBO);
    glGenBuffers(1,&this->EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
    glBufferData(GL_ARRAY_BUFFER,this->vertices.size()*sizeof(Vertex),&this->vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->indices.size()*sizeof(GLuint),&this->indices[0],GL_STATIC_DRAW);

    //设置顶点坐标指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)0);
    //设置法线指针
    glEnableVertexAttribArray(1);
    //预处理指令offset(s,m)第一个参数传入结构体，第二个参数是这个结构体名字的变量,函数返回这个变量从结构体开始的字节偏移量（offset）
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,Normal));
    //设置顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,TexCoords));

    glBindVertexArray(0);
}
