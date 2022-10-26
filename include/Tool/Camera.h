#ifndef CAMERA.H
#define CAMERA.H
#include <glad/glad.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
enum Camera_Movement
{
    Forward,
    Backward,
    Left,
    Right,
};


const float YAw = -90.0f;//偏航角
const float PItch = 0.0f; //俯仰角
const float SEnsitivity = 0.1f;     //灵敏度
const float FOv = 60.0f;
const float SPeed = 2.5f;

class Camera
{
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 Worldup;

    float Yaw;
    float Pitch;
    float Sensitivity;
    float Fov;
    float MovementSpeed;

    Camera(glm::vec3 position= glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAw , float pitch = PItch ):Front(glm::vec3(0.0f,0.0f,-1.0f)),Sensitivity(SEnsitivity),MovementSpeed(SPeed),Fov(FOv)
    {
        Position = position;
        Worldup = up;
        Yaw = yaw;
        Pitch = pitch;
        updateMouseMartix();
    }

    Camera(float posx,float posy,float posz,float upx,float upy,float upz,float yaw = YAw,float pitch = PItch):Front(glm::vec3(0.0f,0.0f,-1.0f)),Sensitivity(SEnsitivity),MovementSpeed(SPeed),Fov(FOv)
    {
        Position = glm::vec3(posx,posy,posz);

        Worldup = glm::vec3(upx,upy,upz);

        Yaw = yaw;

        Pitch = pitch;

        updateMouseMartix();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position,Position+Front,Up);
    }

     void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= Sensitivity;
        yoffset *= Sensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        updateMouseMartix();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Fov -= (float)yoffset;
        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 45.0f)
            Fov = 45.0f; 
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == Forward)
            Position += Front * velocity;
        if (direction == Backward)
            Position -= Front * velocity;
        if (direction == Left)
            Position -= Right * velocity;
        if (direction == Camera_Movement::Right)
            Position += Right * velocity;
    }

private:
    void updateMouseMartix()
    {
        glm::vec3 front = glm::vec3(1.0f);
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front,Worldup));

        Up = glm::normalize(glm::cross(Right,Front));
    }
};

#endif