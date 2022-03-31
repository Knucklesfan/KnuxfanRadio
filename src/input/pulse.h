#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>

//this entire pulseaudio handling header and source was taken from https://github.com/TheBITLINK/desktop-visualizer, a GREAT example of a cool visualizer! THANK YOU FOR MAKING THIS POSSIBLE!!!



struct audio_data {
  int audio_out[2048];
  int format;
  unsigned int rate ;
  char *source; //alsa device, fifo path or pulse source
	int terminate; // shared variable used to terminate audio thread
};


void cb(__attribute__((unused)) pa_context *pulseaudio_context, const pa_server_info *i, void *userdata);
void pulseaudio_context_state_callback(pa_context *pulseaudio_context, void *userdata);
void getPulseDefaultSink(void* data);
void* input_pulse(void* data);