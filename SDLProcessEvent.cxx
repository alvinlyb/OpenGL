#include "SDLProcessEvent.hxx"
#include "camera.hxx"
#include "SDL.h"
#include "qdebug.h"

static SDL_Window *g_Window=nullptr;

SDLProcessEvent::SDLProcessEvent(SDL_Window *window)
{
    g_Window=window;
    firstMouse=true;
    lastX =  800/2.0;
    lastY =  600 / 2.0;
}
SDLProcessEvent::~SDLProcessEvent()
{

}
void SDLProcessEvent::Init(Camera *cam)
{
    camera=cam;
}

bool SDLProcessEvent::sdlEventloop(SDL_Event *event)
{
    float cmaeraSpeed=2.5;
    while(SDL_PollEvent(event))
    {
        switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
        {
            switch (event->button.button) {
            case SDL_BUTTON_LEFT:
            {
                qDebug()<<" mouse button left";
                break;
            }
            case SDL_BUTTON_MIDDLE:
            {
                qDebug()<<"mouse button middle";
                break;
            }
            case SDL_BUTTON_RIGHT:
            {
                qDebug()<<"mouse button right";
                break;
            }
            default:
                break;
            }
            break;
        }
        case SDL_MOUSEWHEEL:
        {

            if(event->wheel.x>0)
            {
                HandleMousePressEvent(event->wheel.x,event->wheel.y);
            }else if(event->wheel.x<0)
            {
                HandleMousePressEvent(event->wheel.x,event->wheel.y);
            }
            if(event->wheel.y>0)
            {
                HandleMousePressEvent(event->wheel.x,event->wheel.y);
            }else if(event->wheel.y<0)
            {
                HandleMousePressEvent(event->wheel.x,event->wheel.y);
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            switch (event->key.keysym.sym) {
            case SDLK_UP:
            {
                qDebug()<<"keydown w or up";
                //camera.ProcessKeyboard(FORWARD,deltaTime);
                camera->ProcessKeyboard(FORWARD,0.010);
                break;
            }
            case SDLK_DOWN:
            {
                qDebug()<<"keydown w or down";
                //camera.ProcessKeyboard(BACKWARD,deltaTime);
                camera->ProcessKeyboard(BACKWARD,0.010);
                break;
            }
            case SDLK_LEFT:
            {
                qDebug()<<"keydown w or left";
                //camera.ProcessKeyboard(LEFT,deltaTime);
                camera->ProcessKeyboard(LEFT,0.010);
                break;
            }
            case SDLK_RIGHT:
            {
                qDebug()<<"keydown w or right";
                //camera.ProcessKeyboard(RIGHT,deltaTime);
                camera->ProcessKeyboard(RIGHT,0.01);
                break;
            }
            case SDLK_ESCAPE:
            {
                qDebug()<<"keydown w or end";
                //camera.ProcessKeyboard(RIGHT,deltaTime);
                return true;
            }
            case SDLK_w:
            {
                //camera.ProcessKeyboard(FORWARD,deltaTime);
                break;
            }
            case SDLK_s:
            {
                //camera.ProcessKeyboard(BACKWARD,deltaTime);
                break;
            }
            case SDLK_a:
            {
                //camera.ProcessKeyboard(LEFT,deltaTime);
                break;
            }
            case SDLK_d:
            {
                //camera.ProcessKeyboard(RIGHT,deltaTime);
                break;
            }
            default:
                break;
            }
        }
        case SDL_QUIT:
        {
            return true;
            break;
        }
        default:
            break;
        }
    }
    return false;
}
/*
void SDLProcessEvent::HandleKeyPressEvent()
{
    camera->ProcessKeyboard(FORWARD,1.0);
}
*/
void SDLProcessEvent::HandleMousePressEvent(double xpos, double ypos)
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

    camera->ProcessMouseMovement(xoffset,yoffset);
}

