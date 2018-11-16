#include "camera.hxx"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    :Front(glm::vec3(0.0f,0.0f,-1.0f)),
      MoveMentSpeed(SPEED),
      MouseSensitivity(SENSITIVTY),
      Zoom(ZOOM)
{
    this->Position=position;
    this->WorldUp=up;
    this->Yaw=yaw;
    this->Pitch=pitch;

}

//Camera::Camera(GLfloat posX,GLfloat posY,GLfloat posZ,GLfloat upX,GLfloat upY,GLfloat upZ,GLfloat yaw,GLfloat pitch)
//    :Front(glm::vec3(0.0f,0.0f,-1.0f)),
//      MoveMentSpeed(SPEED),
//      MouseSensitivity(SENSITIVTY),
//      Zoom(ZOOM)
//{
//    this->Position=position;
//    this->WorldUp=up;
//    this->Yaw=yaw;
//    this->Pitch=pitch;
//    this->updateCameraVectors();
//}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position,this->Position+this->Front,this->Up);
}

void Camera::ProcessKeyboard(Camera_movement direction,GLfloat deltaTime)
{
    GLfloat velocity=this->MoveMentSpeed*deltaTime;
    if(direction==FORWARD)
        this->Position+=Front*velocity;
    if(direction==BACKWARD)
        this->Position-=Front*velocity;
    if(direction==LEFT)
        this->Position-=this->Right*velocity;
    if(direction==RIGHT)
        this->Position+=this->Right*velocity;
    this->updateCameraVectors();
}

void Camera::ProcessMouseMovement(GLfloat xoffset,GLfloat yoffset,GLboolean constrainPitch)
{
    /* -------------------------------------------------------
    1.计算鼠标和上一帧的偏移量。
    2.把偏移量添加到摄像机和俯仰角和偏航角中。
    3.对偏航角和俯仰角进行最大和最小值的限制。
    4.计算方向向量。
    -------------------------------------------------------*/
    // 用于调节鼠标的偏移大小
    xoffset*=this->MouseSensitivity;
    yoffset*=this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    if(constrainPitch)
    {
        // 限制俯仰pitch的角度
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }
    this->updateCameraVectors();
}
// 鼠标滚轮回调函数
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    //计算得到实际方向向量
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}
