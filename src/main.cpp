/***
  This file is part of PulseAudio.
 
  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.
 
  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/
 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "visualizer.h"
#include "cube.h"


float fps = 120;

double calc_dt()
{
    static int first = 1;
    static double inv_freq;
    static uint64_t prev;

    uint64_t now = SDL_GetPerformanceCounter();

    if (first) {
        first = 0;
        prev = now;
        inv_freq = 1.0 / (double)SDL_GetPerformanceFrequency();
    }

    double dt = ((double)(now - prev) * inv_freq);
    prev = now;
    return dt;
}

 
int main(int argc, char*argv[]) {
    //handles sdl stuff
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)==-1) {
        exit(1);
    }
    
    graphics::window = SDL_CreateWindow("KnuxRadio",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1280,720,SDL_WINDOW_OPENGL);
    graphics::render =SDL_CreateRenderer(graphics::window,-1,SDL_RENDERER_ACCELERATED);

    //TODO: make this into it's own separate class, and potentially own separate thread?

    struct timespec req = { .tv_sec = 0, .tv_nsec = 0 }; 
    int sleep = 0;
    bool quit = false;
    visualizer* vis = new visualizer();
    cube* cub = new cube(640/2,480/2,640,480);

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    graphics::deltaTime = calc_dt();
    while(!quit) {
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP: {
                    break;
                }
                case SDL_KEYDOWN: {
                    break;
                }
            }
        }


        int silence = vis->getSleep();

        if (silence == 1) {sleep++;} //YES, I KNOW DESKTOP-VISUALIZER DOESNT USE BRACKETS BUT DANG IT THEY GIVE ME ANXIETY NOT BEING THERE
        else {sleep = 0;} //the cs UIL in my blood bleeds deep

        if (sleep > fps * 5) { //vsync type beat, im gonna change this later since this doesn't really represent SDL2 too well
        //TODO: update this



        req.tv_sec = 1;
        req.tv_nsec = 0;
        nanosleep(&req, NULL);
        continue;
        }

        graphics::deltaTime = calc_dt();

        vis->logic();
        cub->logic();
        SDL_RenderClear(graphics::render);



        //handle rendering stuff
        vis->render();
        cub->render(255,255,255);
        graphics::drawTexture(cub->texture,320,120,0,2.0,false);
        SDL_RenderPresent(graphics::render);
    }

  
    return 0;
}



