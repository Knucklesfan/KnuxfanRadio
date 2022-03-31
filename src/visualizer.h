#define BARCOUNT 50
#include <SDL2/SDL.h>
#include <fftw3.h>
#include <iostream>

#include "input/pulse.h"
#include <math.h>
#include <pthread.h>
#include "defs.h"

class visualizer {
    public:
        float fps = 120;
        fftw_plan p;
        int thr_id;
        pthread_t p_thread;
        audio_data audio;
        int MAX_HEIGHT = 256;
        float bars[5][BARCOUNT];
        visualizer();
        int getSleep();
        double in[2050];
        fftw_complex out[1025][2];
        void logic();
        void render();
        double rainbowhue;
        struct color hsvtorgb(int h, int s, int v);
};
struct color {
    double r = 0;
    double g = 0;
    double b = 0;
};

