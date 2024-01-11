
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <math.h>

#include "constants.h"
#include "manr.h"
#include "canvas.h"
#include "window.h"

int term_width, term_height;

void draw_text(Canvas* canvas, int x, int y, char* text){
	for(int i=0; i<strlen(text); i++){
		setCanvasCharacter(canvas, x + i, y, text[i]);
		setCanvasFgColor(canvas, x + i, y, WHITE);
	}
}

void clear_canvas(Canvas* canvas){
	for(int i=0; i<canvas->height; i++){
		for(int j=0; j<canvas->width; j++){
			setCanvasCharacter(canvas, j, i, ' ');
		}
	}
}

void draw_sine(Canvas* canvas, int offset, float amplitude, float period){
	int x = 0;
	int y = term_height / 2;

	for(float i=0.0f; i<term_width; i += 0.1){
		float sine = amplitude * sin((period * (i + offset)) * M_PI / 180);
		int height = sine * (term_height / 4);
		setCanvasCharacter(canvas, i, y - height, '#');
	}

}

int main(){
	int noise[MAX_VIEW_AREA];
	int quit = 0;
	float ticks = 0.0f;
	Window* window = createWindow();
	char* message = malloc(128);
	float amplitude = 1.0f;
	float period = 1.0f;
	float timescale = 1.0f;

	char* quit_message = "Press 'q' to quit.";

	while(!quit){

		char user_input = input();
		if(user_input == 'a') amplitude -= 0.01;
		if(user_input == 's') amplitude += 0.01;
		if(user_input == 'd') period -= 0.1;
		if(user_input == 'f') period += 0.1;
		if(user_input == 'z') timescale -= 0.1;
		if(user_input == 'x') timescale += 0.1;

		if (user_input == 'q') quit = 1;

		if(period <= 0.1f) period = 0.1f;
		if(period >= 20.0f) period = 20.0f;
		if(amplitude >= 2.0f) amplitude = 2.0f;
		if(amplitude <= -2.0f) amplitude = -2.0f;
		if(timescale <= -10.0f) timescale = -10.0f;
		if(timescale >= 10.0f) timescale = 10.0f;

		clear_canvas(window->canvas);
		draw_sine(window->canvas, ticks, amplitude, period);
		sprintf(message, "Amplitude [A/S]: %f, Frequency [D/F]: %f, Timescale [Z/X]: %f", amplitude, period, timescale);
		draw_text(window->canvas, term_width / 2 - strlen(message) / 2,
				term_height - 1, message);
		draw_text(window->canvas, term_width / 2 - strlen(quit_message) / 2,
				0, quit_message);
		termRefresh(window);
		ticks += timescale;
		usleep(1000000/60);
	}
	endTerm(window);
	return 0;
}
