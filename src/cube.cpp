#include "cube.h"
#include "math.h"
#include "defs.h"

cube::cube(int x0, int y0, int w, int h) {
    texture = SDL_CreateTexture(graphics::render,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,w,h);
    rotateZ3D(rad(30));
    rotateY3D(rad(30));
    rotateX3D(rad(30));
    width = w;
    height = h;
    x = x0;
    y = y0;
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

}

void cube::logic() {
    rotateZ3D(rad(graphics::deltaTime * (cos(rotation/500)*50)));
    rotateY3D(rad(graphics::deltaTime * (sin(rotation/500)*50)));
    //rotateX3D(rad(deltatime/50));
    rotation += graphics::deltaTime*100;
}
void cube::render(int r, int g, int b) {

    SDL_Texture* temp = SDL_GetRenderTarget(graphics::render);
    SDL_SetRenderTarget(graphics::render, texture);
    SDL_RenderClear(graphics::render);
    SDL_SetRenderDrawColor(graphics::render, r, g, b, 255);
    for (int* indices : edges) {
        double* n1 = nodes[indices[0]];
        double* n2 = nodes[indices[1]];
        double x1 = n1[0] * (width / 640) + x;
        double y1 = n1[1] * (height / 480) + y;
        double x2 = n2[0] * (width / 640) + x;
        double y2 = n2[1] * (height / 480) + y;

        SDL_RenderDrawLine(graphics::render, x1,y1,x2,y2);
    }
    SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255);
    SDL_SetRenderTarget(graphics::render, temp);
}
void cube::rotateZ3D(double t) {
    double c = cos(t), s = sin(t);
    for (double* xyz : nodes) {
        double x = xyz[0], y = xyz[1];
        xyz[0] = x * c - y * s;
        xyz[1] = y * c + x * s;
    }
}

void cube::rotateY3D(double t) {
    double c = cos(t), s = sin(t);
    for (double* xyz : nodes) {
        double x = xyz[0], z = xyz[2];
        xyz[0] = x * c - z * s;
        xyz[2] = z * c + x * s;
    }
}

void cube::rotateX3D(double t) {
    double c = cos(t), s = sin(t);
    for (double* xyz : nodes) {
        double y = xyz[1], z = xyz[2];
        xyz[1] = y * c - z * s;
        xyz[2] = z * c + y * s;
    }
}

double cube::rad(double deg) {
    return deg * M_PI / 180;
}
