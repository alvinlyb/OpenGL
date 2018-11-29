#include "model.hxx"
#include "qdebug.h"

Model::Model()
{
    //this->loadModel(path);
}

Model::~Model()
{
    for(vector<Mesh>::const_iterator it=this->meshes.begin();this->meshes.end()!=it;++it)
        it->final();
}

void Model::Draw(const Shader &shader) const
{
    for(vector<Mesh>::const_iterator it=this->meshes.begin();this->meshes.end()!=it;++it)
        it->Draw(shader);
}

bool Model::loadModel(const string &path)
{
    Assimp::Importer importer;
    //路径为空时返回错误
    if(path.empty())
    {
        cerr<<"Error:Model::loadModel,empty model file path."<<endl;
        return false;
    }
    //在path中读取文件,aiProcess_Triangulate如果不是全部由三角形组成转换所有的模型大海捞针始几何形状为三角形
    //aiProcess_FlipUVs基于y轴翻转纹理坐标,在处理时候是必须的。
    const aiScene* scene=importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);
    if(!scene||scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode)
    {
        cout<<"ERROR::ASSIMP::"<<importer.GetErrorString()<<endl;
        return false;
    }
    this->directory=path.substr(0,path.find_last_of('/'));  //保存一下当前目录路径
    if(!this->processNode(scene->mRootNode,scene))  //传递根节点和场景
    {
        cerr<<"Error:Model::loadModel,process node failed."<<endl;
        return false;
    }
    return true;
}

bool Model::processNode(aiNode *node, const aiScene *scene)
{
    if(!node||!scene)
        return false;
    //添加当前节点中的所有mesh
    for(GLuint i=0;i<node->mNumMeshes;i++)
    {
        aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];
        if(mesh)
        {
            this->meshes.push_back(this->processMesh(mesh,scene));  //把每个mesh压入meshes中
        }

    }
    //递归处理该节点的子节点
    for(GLuint i=0;i<node->mNumChildren;i++)
    {
        this->processNode(node->mChildren[i],scene);    //递归读取子节点
    }
    return true;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    //vertices
    for(GLuint i=0;i<mesh->mNumVertices;i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        //position
        if(mesh->HasPositions())
        {
            vector.x=mesh->mVertices[i].x;
            vector.y=mesh->mVertices[i].y;
            vector.z=mesh->mVertices[i].z;
            vertex.Position=vector; //把每个顶点的x,y,z数据存储在vectex结构中的Position中。
        }
        //normals
        if(mesh->HasNormals())
        {
            vector.x=mesh->mNormals[i].x;
            vector.y=mesh->mNormals[i].y;
            vector.z=mesh->mNormals[i].z;
            vertex.Normal=vector;   //重新定义vector变为法线数据存储在vectex结构中的Normal中。
        }

        if(mesh->HasTextureCoords(0))
        {
            glm::vec2 vec;
            vec.x=mesh->mTextureCoords[0][i].x;
            vec.y=mesh->mTextureCoords[0][i].y;
            vertex.TexCoords=vec;   //存储纹理到vertex结构中的TexCoords中.
        }else{
            vertex.TexCoords=glm::vec2(0.0f,0.0f);  //如果没有纹理坐标信息存为0.0f，保证数据结构的完整。
        }

        vertices.push_back(vertex); //把顶点数据压入vertices
    }
    //indices索引数据
    for(GLuint i=0;i<mesh->mNumFaces;i++)
    {
        aiFace face=mesh->mFaces[i];    //定义面
        if(face.mNumIndices!=3)
        {
            cerr<<"Error:Model::processMesh,mesh not transformed to triangle mesh."<<endl;
        }
        for(GLuint j=0;j<face.mNumIndices;j++)
            indices.push_back(face.mIndices[j]);   //面的绘制索引压入indices中，最后作为EBO使用
    }
    //textures获取纹理数据
    if(mesh->mMaterialIndex>=0)
    {
        aiMaterial* material=scene->mMaterials[mesh->mMaterialIndex];   //获取材质节点
        //1.diffuse maps
        vector<Texture> diffuseMaps=this->loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
        textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
        //2.specular maps
        vector<Texture> specularMaps=this->loadMaterialTextures(material,aiTextureType_SPECULAR,"texture_specular");
        textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
    }
    return Mesh(vertices,indices,textures); //使用Mesh类来处理网格
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat,aiTextureType type,string typeName)
{
    vector<Texture> textures;
    //通过mat中储存的纹理数量
    for(GLuint i=0;i<mat->GetTextureCount(type);i++)
    {
        aiString str;
        mat->GetTexture(type,i,&str);   //找到每个纹理的文件位置
        GLboolean skip=false;
        for(GLuint j=0;j<textures_loaded.size();j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(),str.C_Str())==0)
            {
                textures.push_back(textures_loaded[j]);
                skip=true;
                break;
            }
        }
        if(!skip)   //如果纹理没有被加载，就加载纹理
        {
            Texture texture;
            texture.id=TextureFromFile(str.C_Str(),this->directory);
            texture.type=type;  //名称
            texture.path=str.C_Str();   //纹理路径
            textures.push_back(texture);    //把纹理结构压入textures中
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

GLuint Model::TextureFromFile(const char* path,const string &directory)
{
    string filename=string(path);
    filename=directory+'/'+filename;
    GLuint textureID;
    glGenTextures(1,&textureID);
    int width,height,nrComponents;
    //unsigned char* image=SOIL_load_image(filename.c_str(),&width,&height,0,SOIL_LOAD_RGB);
    unsigned char* image=stbi_load(filename.c_str(),&width,&height,&nrComponents,0);
    if(image)
    {
        GLenum format;
        if(nrComponents==1)
            format=GL_RED;
        else if(nrComponents==3)
            format=GL_RGB;
        else if(nrComponents==4)
            format=GL_RGBA;

        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        stbi_image_free(image); //释放图片内存
    }else{
        cout<<"Texture failed to load at path:"<<path<<endl;
        stbi_image_free(image);
    }

    return textureID;
}
