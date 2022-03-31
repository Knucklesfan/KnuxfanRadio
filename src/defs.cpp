#include "defs.h"


SDL_Renderer* graphics::render = nullptr;
SDL_Window* graphics::window = nullptr;
double graphics::deltaTime = 0;

int graphics::sign(int x) { //generic sign function
        if (x > 0) return 1;
        if (x < 0) return -1;
        return 0;

}

void graphics::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
    SDL_Rect sprite;
    SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h);
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;
    if (center) {
        sprite.x = x - oldwidth / 2;
        sprite.y = y - oldheight / 2;
    }
    else {
        sprite.x = x + oldwidth / 2 - sprite.w / 2;
        sprite.y = y + oldheight / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(render, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}
