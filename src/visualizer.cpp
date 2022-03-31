#include "visualizer.h"

visualizer::visualizer() {
    p = fftw_plan_dft_r2c_1d(2048, in, *out, FFTW_MEASURE);
    int *freq;
    for(int j = 5; j > 0; j--) {
        memset(bars[j], 0, sizeof bars[j]);
    }


    audio.source = (char*)"auto";
    audio.format = -1;
    audio.terminate = 0;
    for (int i = 0; i < 2048; i++) {
		audio.audio_out[i] = 0;
	}
    getPulseDefaultSink((void*)&audio);
	thr_id = pthread_create(&p_thread, NULL, input_pulse, (void*)&audio); 
	audio.rate = 44100;


}
int visualizer::getSleep() {
    //copies the current audio data from the thread
    int silence = 1;
    	for (int i = 0; i < 2050; i++) {
			if (i < 2048) {
				in[i] = audio.audio_out[i];
				if (in[i]) silence = 0;
			} else {
				in[i] = 0;
			}
        }
}
void visualizer::logic() {
        for(int j = 0; j < 4; j++) {
            memcpy(bars[j+1], bars[j], sizeof bars[j]);
        }
        fftw_execute(p);
        for (int i = 0; i < BARCOUNT; i++) { //begin parsing bars
            int ab = sqrt(pow(*out[i * 2][0], 2) + pow(*out[i * 2][1], 2)); //changed this to a sqrt since pow(n,0.5f) is pretty dum
            ab = ab + (ab * i / 30);
            float bar = (float)ab / (float)3500000;
            if (bar > 1) bar = 1;
            if (bar > bars[0][i]) {
                bars[0][i] = bar;
            } else {
                bars[0][i] -= 1 / fps;
            }
            if (bars[0][i] < 0) bars[0][i] = 0;
        }


}

void visualizer::render() {
    int w = 1280;
    int h = 720;
        
    for (int i = 0; i < BARCOUNT; i++) {
        for(int j = 4; j > 0; j--) {
            float bar = bars[j][i];
            float width = (float)w / (float)BARCOUNT;
            float height = (bar * MAX_HEIGHT) + j*5; //cursed sign function. i figured since this runs in a loop that i could do this faster.
            float posY = (h / 2) - (height / 2);
            float posX = width * i;
            struct color hs = hsvtorgb(fmod(rainbowhue+i*5,255),255,255-j*20); // what the hell
            SDL_SetRenderDrawColor(graphics::render, hs.r,hs.g,hs.b,255);
            SDL_Rect r = {posX, posY, width, height};
            SDL_RenderFillRect(graphics::render,&r); //still the bottle neck :(
        }
    }
    double lasty = 0;
    int lastx = 0;
    for (int i = 0; i < 256; i++) {
        struct color hs = hsvtorgb(fmod(rainbowhue+i*5,255),255,255);
        SDL_SetRenderDrawColor(graphics::render, hs.r,hs.g,hs.b,255);
        double curry = (((*out[i*2][0]))/ (float)3500000)*500;
        int currx = (i)*(1280/128)+100;
        SDL_RenderDrawLine(graphics::render,lastx,lasty+720/2,currx, curry+720/2);
        lasty = curry;
        lastx = currx;
        
    }
    SDL_SetRenderDrawColor(graphics::render, 0,0,0,255);
    rainbowhue+=0.05;
}

struct color visualizer::hsvtorgb(int h, int s, int v) {
    struct color rgb;
    int region, remainder, p, q, t;

    if (s == 0)
    {
        rgb.r = v;
        rgb.g = v;
        rgb.b = v;
        return rgb;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = v;
            break;
        default:
            rgb.r = v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}