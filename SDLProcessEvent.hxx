#ifndef EVENT_HXX
#define EVENT_HXX

struct SDL_Window;
typedef union SDL_Event SDL_Event;

class Camera;
class SDLProcessEvent
{
public:
    SDLProcessEvent(SDL_Window *window);
    ~SDLProcessEvent();

    void Init(Camera *cam);
    bool sdlEventloop(SDL_Event *event);

    //void HandleKeyPressEvent();
    void HandleMousePressEvent(double xpos,double ypos);
protected:
    //SDL_Window *g_Window;
    Camera *camera;
    float lastX;
    float lastY;
    bool firstMouse;
};

#endif // EVENT_HXX
