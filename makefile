demo : \
demo.o \
manr.o \
window.o \
canvas.o
	gcc manr.o \
		demo.o \
		fractal_noise.o \
		canvas.o \
		window.o \
		-o demo -g -lm

demo.o : demo.c
	gcc -c demo.c
manr.o : manr.c
	gcc -c manr.c
canvas.o : canvas.c
	gcc -c canvas.c
window.o : window.c
	gcc -c window.c
	
