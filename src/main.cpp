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
 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
 #include <math.h>
#include <pulse/simple.h>
#include <pulse/error.h>
 #include <SDL2/SDL.h>
#include <fftw3.h>

#define BUFSIZE 16
 #define WIDTH 800
#define HEIGHT 600
#define SAMPLE_RATE 44100
#define BARS 60
#define THICKNESS 15
#define DISTANCE 16
#define FIT_FACTOR 30

double Get16bitAudioSample(Uint8* bytebuffer)
{
    Uint16 val= 0x0;
        val=((uint16_t)bytebuffer[0] << 8) | (uint16_t)bytebuffer[1];
    return val/65535.0;
}


/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd, const void*data, size_t size) {
    ssize_t ret = 0;
 
    while (size > 0) {
        ssize_t r;
 
        if ((r = write(fd, data, size)) < 0)
            return r;
 
        if (r == 0)
            break;
 
        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }
 
    return ret;
}
 
int main(int argc, char*argv[]) {
    //handles sdl stuff
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)==-1) {
        exit(1);
    }
    
    SDL_Window* window = SDL_CreateWindow("KnuxRadio",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer =SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);







    /* The sample type to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    pa_simple *s = NULL;
    pa_simple *b = NULL;
    int ret = 1;
    int error;
    fftw_complex *in, *out;
    fftw_plan p;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * BUFSIZE);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * BUFSIZE);

    p = fftw_plan_dft_1d(BUFSIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }

    while(true) {
        
        uint8_t buf[BUFSIZE];
 
        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

        double max[BARS];
        double multiplier;
        int winndow_size = 2;
        int count= 0;
        int sum;

        double freq_bin[BARS+1];
        double re,im;
        float CONSTANT=(float)BUFSIZE/WIDTH;
        float freq;
        double magnitude;
        int startx=0,starty=HEIGHT;

        int NSAMPLES = BUFSIZE;

        //construct a range of frequencies based on NSAMPLES
        for(int i=0;i<BARS;i++)
        {
            max[i]=1.7E-308;
            freq_bin[i]=i*(SAMPLE_RATE/NSAMPLES)+i;
            //printf("%d:%lf\t",i,freq_bin[i]);
        }
        freq_bin[BARS]=SAMPLE_RATE/2;

        for(int i=0;i<NSAMPLES;i++)
        {
            //getting values from stream and applying hann windowing function
            multiplier= 0.5*(1-cos(2*M_PI*i/NSAMPLES));
            
            in[i][0]=Get16bitAudioSample(
                                        buf
                                        )*multiplier;
            in[i][1]=0.0;
        }
        fftw_execute(p);

                for(int j=0;j < NSAMPLES/2;j++)
        {
            re=out[j][0];
            im=out[j][1];

            magnitude=sqrt((re*re)+(im*im));
            
            freq=j*((double)SAMPLE_RATE/NSAMPLES);

            for(int i=0;i < BARS;i++)
                if((freq>freq_bin[i]) && (freq<=freq_bin[i+1]))
                    if(magnitude > max[i]) 
                        max[i]=magnitude;
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderClear(renderer);
        
        for(int i=0;i<BARS;i++)
        {
            SDL_SetRenderDrawColor(renderer,
                                    255, 
                                    0 ,
                                    0,
                                    255); 
            
            if(max[i]>2.0)
                max[i]=log(max[i]);

            for(int j=0;j<THICKNESS;j++) {
                SDL_RenderDrawLine(renderer,
                                    startx+(i*DISTANCE+j),
                                    starty,
                                    startx+(i*DISTANCE+j),
                                    starty-(FIT_FACTOR*max[i]));
            }
        startx+=50;
        printf("%i\n",i);
        }
        SDL_RenderPresent(renderer);
    }

 
    ret = 0;
 
finish:
 
    if (s)
        pa_simple_free(s);
 
    return ret;
}



