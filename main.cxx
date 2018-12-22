//#include <GLFW/glfw3.h>
//#include <SDL.h>

#include <iostream>
#include <QApplication>
#include <string>
#include "qdebug.h"

#include "shader.hxx"
#include "camera.hxx"
#include "model.hxx"
#include "SDLProcessEvent.hxx"
#include "testdata.hxx"

#define IMPORT_OPENGL_LOADER_GLAD
#define IMPORT_LOADER_GLFW
#include "import_opengl3.hxx"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include <common/common.hpp>
//#include <SOIL2/SOIL2.h>
//#pragma comment(lib, "D:\\cxxlib\\soil2\\lib\\soil2d.lib")
//#include <boost/format.hpp>

//数组长度的获取
#define getArrayLen(array,len) {len = sizeof(array)/sizeof(array[0]);}
template<class T>
int length(T& arr)
{
    return sizeof(arr)/sizeof(arr[0]);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//灯光
glm::vec3 objectColor(1.0f,0.5f,0.31f);
//camera实例
Camera camera(glm::vec3(0.0f,0.0f,3.0f));
bool firstMouse = true;
bool keys[1024];
//得到窗口的中心位置
float lastX =  (float)SCR_WIDTH / 2.0;
float lastY =  (float)SCR_HEIGHT / 2.0;
float fov   =  45.0f;

// 时间差变量
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //sdl：initialize and configure
//    SDL_Init(SDL_INIT_EVERYTHING);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

//    SDL_Window *sdlWindow=nullptr;
//    SDL_GLContext gl_Context;
//    sdlWindow=SDL_CreateWindow("LernOpenGL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCR_WIDTH,SCR_HEIGHT,SDL_WINDOW_OPENGL);
//    if(sdlWindow == nullptr)
//    {
//        std::cout<<SDL_GetError()<<std::endl;
//        return -1;
//    }else{
//        gl_Context=SDL_GL_CreateContext(sdlWindow);
//        gladLoadGL();
//    }
//    SDL_GL_SetSwapInterval(1);

     //glfw: initialize and configure
     //------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window,key_callback);        //设置键盘回调函数
    glfwSetCursorPosCallback(window,mouse_callback);    //设置鼠标回调函数
    glfwSetScrollCallback(window,scroll_callback);  //设置滚动回调函数
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);  //捕捉窗口鼠标

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);        //打开opengl的深度测试
    glDepthFunc(GL_LESS);       //在片段深度值 小于缓冲区的深度时通过测试
    //加载模型
    Model objModel;
    if(!objModel.loadModel("resources/objects/nanosuit/nanosuit.obj"))
    {
        //glfwTerminate();
        system("pause");
        return -1;
    }
    //shader的实例化glsl
    Shader ourShader("../vertex.vert", "../fragment.frag");
    //灯光
    Shader lightShader("../LightVertex.vert", "../light.frag");
    // ------------------------------------------------------------------

    GLuint VBO, VAO,EBO,lightVAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);  //生成顶点缓冲管理对象
    //glGenBuffers(2,&EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //把新创建的VBO绑定到GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO); //绑定VAO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);  //绑定到EBO
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);   //以线框显示
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //法线属性
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //纹理属性
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // 解除绑定的VAO

    //灯光lightVAO
    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
//    // 位置属性
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    // 颜色属性
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);


    // load and create a texture
    GLuint diffuseMap,specularMap;
    glGenTextures(1,&diffuseMap);
    glGenTextures(1,&specularMap);
    int width,height,channels;
    unsigned char* image;
    //diffuseMap texture
    glBindTexture(GL_TEXTURE_2D,diffuseMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    //加载纹理图片
    image=stbi_load("../container2.jpg",&width,&height,&channels,0);
    //image=SOIL_load_image("F:/script/QtProject/OpenGLTest001/container2.png",&width,&height,0,SOIL_LOAD_RGBA);
    if(image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout<<"Failed to load image"<<std::endl;
    }
    stbi_image_free(image);    //释放图像
    //SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D,0);
    //specularMap texture
    glBindTexture(GL_TEXTURE_2D,specularMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    image=stbi_load("../container2_specular.jpg",&width,&height,&channels,0);
    //image=SOIL_load_image("F:/script/QtProject/OpenGLTest001/container2_specular.png",&width,&height,0,SOIL_LOAD_RGBA);
    if(image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout<<"Failed to load image"<<std::endl;
    }
    stbi_image_free(image);    //释放图像
    //SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D,0);

    ourShader.use();
    ourShader.setInt("material.diffuse",0);
    ourShader.setInt("material.specular",1);

    //SDLProcessEvent sdlEvent(sdlWindow);
    //sdlEvent.Init(&camera);
    // render loop
    // -----------
    //SDL_Event e;
    //while(!sdlEvent.sdlEventloop(&e)){

//  while(SDL_PollEvent(&e))
    while (!glfwWindowShouldClose(window))
    {

        // 时间差，用于控制不同计算机的延迟
        // --------------------
        float currentFrame = glfwGetTime();     //运行秒数
        deltaTime = currentFrame - lastFrame;   //时间差，当前帧减去上一帧的时间
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        do_movement();
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色和深度缓冲区

        //必须创建的对角矩阵
        glm::mat4 model=glm::mat4(1.0f);
        glm::mat4 view=glm::mat4(1.0f);
        glm::mat4 projection=glm::mat4(1.0f);
        //灯光位置
        glm::vec3 lightPos(1.2f,1.5f,2.0f);
        //创建透视投影
        view=camera.GetViewMatrix();
        projection=glm::perspective(camera.Zoom,(GLfloat)SCR_WIDTH/(GLfloat)SCR_HEIGHT,0.1f,1000.0f);

        //物体shader
        ourShader.use();
        glm::vec3 lightColor=glm::vec3(1.0f);
//        lightColor.x=sin(currentFrame*2.0f);
//        lightColor.y=sin(currentFrame*0.7f);
//        lightColor.z=sin(currentFrame*1.3f);

        glm::vec3 diffuseColor =lightColor*glm::vec3(0.5f);
        glm::vec3 ambientColor =diffuseColor*glm::vec3(0.2f);
        //GLint modelLoc=glGetUniformLocation(ourShader.ID,"model");  //查找到着色器的变量
        //glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glm::vec3 viewPos=glm::vec3(camera.Position.x,camera.Position.y,camera.Position.z);
        ourShader.setVec3("objectColor",objectColor);
        ourShader.setVec3("lightColor",lightColor);
        ourShader.setMat4("view",view);
        ourShader.setMat4("projection",projection);
        ourShader.setVec3("viewPos",viewPos);
        //设置GLSL材质参数
        //ourShader.setVec3("material.ambient",glm::vec3(1.0f,0.5f,0.31f));
        //ourShader.setVec3("material.diffuse",glm::vec3(1.0f,0.5f,0.31f));
        //ourShader.setVec3("material.specular",glm::vec3(0.0f));
        ourShader.setFloat("material.shininess",256.0f);
        //灯光影响材质参数
        ourShader.setVec3("light.ambient",ambientColor);    //环境色
        ourShader.setVec3("light.diffuse",diffuseColor);    //漫反射色
        ourShader.setVec3("light.specular",glm::vec3(1.0f,1.0f,1.0f));      //高光
        //ourShader.setVec3("light.direction",glm::vec3(-0.2,-1.0f,-0.3f)); //方向光源时的参数

        ourShader.setFloat("light.constant",1.0f);
        ourShader.setFloat("light.linear",0.09);
        ourShader.setFloat("light.quadratic",0.032);

        //Directional light
        ourShader.setVec3("dirLight.direction",glm::vec3(-0.2f,-1.0f,-0.3f));   //方向法线
        ourShader.setVec3("dirLight.ambient",glm::vec3(0.05f)); //环境光
        ourShader.setVec3("dirLight.diffuse",glm::vec3(0.4f));   //漫反射
        ourShader.setVec3("dirLight.speculer",glm::vec3(0.5f));   //高光
        //spotlight
        ourShader.setVec3("spotLight.position",camera.Position);    //聚光位置
        ourShader.setVec3("spotLight.direction",camera.Front);  //聚光方向
        ourShader.setFloat("spotLight.cutOff",glm::cos(glm::radians(12.5f)));   //聚光角度,这个角度转余弦在glsl中计算量大所在在此传递
        ourShader.setFloat("spotLight.outerCutOff",glm::cos(glm::radians(17.5f)));

        ourShader.setVec3("spotLight.ambient",glm::vec3(0.0f));  //环境光
        ourShader.setVec3("spotLight.diffuse",glm::vec3(1.0f));   //漫反射
        ourShader.setVec3("spotLight.specular",glm::vec3(1.0f));  //高光

        ourShader.setFloat("spotLight.constant",1.0f);
        ourShader.setFloat("spotLight.linear",0.09f);
        ourShader.setFloat("spotLight.quadratic",0.032f);
        //Pointlight
        const int pointLightCount=4;

        for(int i=0;i<pointLightCount;i++)
        {
            glm::vec3 pointPointion=glm::vec3(pointLightPosition[i].x,pointLightPosition[i].y,pointLightPosition[i].z);
            //std::cout<<QString("pointLight[%1].position").arg(i).toStdString()<<std::endl;
            ourShader.setVec3(QString("pointLight[%1].position").arg(i).toStdString(),pointPointion);    //灯光位置
            ourShader.setVec3(QString("pointLight[%1].ambient").arg(i).toStdString(),glm::vec3(0.5f));  //环境光
            ourShader.setVec3(QString("pointLight[%1].diffuse").arg(i).toStdString(),glm::vec3(0.8f));   //漫反射
            ourShader.setVec3(QString("pointLight[%1].specular").arg(i).toStdString(),glm::vec3(1.0f));  //高光

            ourShader.setFloat(QString("pointLight[%1].constant").arg(i).toStdString(),1.0f);
            ourShader.setFloat(QString("pointLight[%1].linear").arg(i).toStdString(),0.09f);
            ourShader.setFloat(QString("pointLight[%1].quadratic").arg(i).toStdString(),0.032f);
        }

        objModel.Draw(ourShader);       //载入模型
        //使用纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,specularMap);
        //物体VAO
        // render boxes
        glBindVertexArray(VAO);
        //绘制10个box
        for(GLint i=0;i<10;++i)
        {
            //旋转
            GLfloat angle=20.0f * i;
            glm::mat4 model=glm::mat4(1.0f);
            model=glm::rotate(model,angle,glm::vec3(0.3f));
            model=glm::scale(model,glm::vec3(1.0f));
            model = glm::translate(model, cubePositions[i]);
            ourShader.setMat4("model",model);

            glDrawArrays(GL_TRIANGLES,0,36);       //直接绘制图元
        }

        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);     //使用顶点索引绘制图元
        glBindVertexArray(0);

        //灯光shader
        lightShader.use();
        lightShader.setMat4("view",view);
        lightShader.setMat4("projection",projection);

        //灯光VAO
        glBindVertexArray(lightVAO);
        for(int i=0;i<pointLightCount;i++)
        {
            //旋转
            glm::mat4 model=glm::mat4(1.0f);
            model=glm::translate(model,pointLightPosition[i]);
            model=glm::scale(model,glm::vec3(0.3f));
            lightShader.setMat4("model",model);

            glDrawArrays(GL_TRIANGLES,0,36);       //直接绘制图元
        }
        glBindVertexArray(0);

        //SDL_GL_SwapWindow(sdlWindow);
        glfwSwapBuffers(window);
        glfwPollEvents();   //检测并调用事件
    }
//}
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1,&lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    //SDL
//    SDL_GL_DeleteContext(gl_Context);
//    SDL_DestroyWindow(sdlWindow);
//    SDL_Quit();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //键盘控制移动
//    float cameraSpeed = 2.5 * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        cameraPos += cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        cameraPos -= cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
//按键回调函数
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(window,GLFW_PRESS);
    if(key >= 0 && key < 1024)
    {
        if(action==GLFW_PRESS)
            keys[key]=true;
        else if(action==GLFW_RELEASE)
            keys[key]=false;
    }

}
//键盘控制移动
void do_movement()
{
    if(keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD,deltaTime);
    if(keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    if(keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT,deltaTime);
    if(keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT,deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX=xpos;
        lastY=ypos;
        firstMouse=false;
    }
    GLfloat xoffset=xpos-lastX;
    GLfloat yoffset=lastY-ypos;

    lastX=xpos;
    lastY=ypos;

    camera.ProcessMouseMovement(xoffset,yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
