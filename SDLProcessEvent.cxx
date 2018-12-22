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
    bool quit=false;
    while(SDL_PollEvent(event)!=0)
    {
        switch (event->type) {
        case SDL_QUIT:
        {
            quit=true;
            break;
        }
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
        case SDL_MOUSEMOTION:
        {
            if(event->motion.x>0)
            {
                HandleMousePressEvent(event->motion.x,event->motion.y);
            }else if(event->motion.x<0)
            {
                HandleMousePressEvent(event->motion.x,event->motion.y);
            }
            if(event->motion.y>0)
            {
                HandleMousePressEvent(event->motion.x,event->motion.y);
            }else if(event->motion.y<0)
            {
                HandleMousePressEvent(event->motion.x,event->motion.y);
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            switch (event->key.keysym.sym) {
            case SDLK_UP:
            {
                //camera.ProcessKeyboard(FORWARD,deltaTime);
                camera->ProcessKeyboard(FORWARD,0.10);
                break;
            }
            case SDLK_DOWN:
            {
                //camera.ProcessKeyboard(BACKWARD,deltaTime);
                camera->ProcessKeyboard(BACKWARD,0.10);
                break;
            }
            case SDLK_LEFT:
            {
                //camera.ProcessKeyboard(LEFT,deltaTime);
                camera->ProcessKeyboard(LEFT,0.10);
                break;
            }
            case SDLK_RIGHT:
            {
                //camera.ProcessKeyboard(RIGHT,deltaTime);
                camera->ProcessKeyboard(RIGHT,0.1);
                break;
            }
            case SDLK_ESCAPE:
            {
                qDebug()<<"keydown w or end";
                //camera.ProcessKeyboard(RIGHT,deltaTime);
                quit=true;
                break;
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
        default:
            break;
        }
    }
    return quit;
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

