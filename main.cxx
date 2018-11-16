#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <QApplication>
#include <string>
#include "qdebug.h"

#include "shader.hxx"
#include "camera.hxx"
#include "testdata.hxx"

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
    // glfw: initialize and configure
    // ------------------------------
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

    //shader的实例化glsl
    Shader ourShader("F:\\script\\QtProject\\OpenGLTest001\\vertex.vs", "F:\\script\\QtProject\\OpenGLTest001\\fragment.fs");
    // ------------------------------------------------------------------

    GLuint VBO, VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);  //生成顶点缓冲管理对象
    //glGenBuffers(2,&EBO);

    glBindVertexArray(VAO); //绑定VAO

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);  //绑定到EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //把新创建的VBO绑定到GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);   //以线框显示
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // 解除绑定的VAO
//    // 位置属性
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    // 颜色属性
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);


    // load and create a texture
    // -------------------------


    // -------------------------------------------------------------------------------------------
    //ourShader.use();


    // render loop
    // -----------
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture1);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        //必须创建的对角矩阵
        glm::mat4 model=glm::mat4(1.0f);
        glm::mat4 view=glm::mat4(1.0f);
        glm::mat4 projection=glm::mat4(1.0f);

        //创建透视投影
        view=camera.GetViewMatrix();
        projection=glm::perspective(camera.Zoom,(GLfloat)SCR_WIDTH/(GLfloat)SCR_HEIGHT,0.1f,1000.0f);

        //GLint modelLoc=glGetUniformLocation(ourShader.ID,"model");  //查找到着色器的变量
        //glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));

        ourShader.setMat4("view",view);
        ourShader.setMat4("projection",projection);

        //颜色变化
        GLfloat greenValue=(sin(currentFrame)/2)+0.5;
        glm::vec4 value=glm::vec4(0.8f, 0.6f, 0.3f,1.0f);
        ourShader.setVec4("ourColor",value);


        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
        //ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        //绘制10个box
        for(GLint i=0;i<10;i++)
        {
            //旋转
            GLfloat angle=20.0f * i;
            model=glm::rotate(model,angle,glm::vec3(0.0f,0.3f,0.0f));
            //位置
            model = glm::translate(model, cubePositions[i]);
            ourShader.setMat4("model",model);

            glDrawArrays(GL_TRIANGLES,0,36);       //直接绘制图元
        }

        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);     //使用顶点索引绘制图元
        glBindVertexArray(0);
        /*
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model=glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            //ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
*/
        glfwSwapBuffers(window);
        glfwPollEvents();   //检测并调用事件
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
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
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD,deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT,deltaTime);
    if(keys[GLFW_KEY_D])
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
