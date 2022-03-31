#include <SDL2/SDL.h>

class graphics {
    public:
        static SDL_Renderer* render;
        static SDL_Window* window;
        static int sign(int);
        static double deltaTime;
        static void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
    //uhhh put other static stuff here.
};
