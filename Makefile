# For the fern

CC = gcc
CFLAGS = -Wall -Wextra -ggdb -std=c11 -O0

FFPMEG = E:/ffmpeg/bin/ffmpeg.exe
FFPLAY = E:/ffmpeg/bin/ffplay.exe

WIDTH := 800
HEIGHT := 800
FRAMES := 120
FPS := 30

all: bf.exe out.mp4

bf.exe: fractal_tree.c
	$(CC) $(CFLAGS) -DWIDTH=$(WIDTH)\
		-DHEIGHT=$(HEIGHT)\
		-DFRAMES=$(FRAMES) -o $@ $^

out.mp4: bf.exe
	./bf.exe | $(FFPMEG) -f rawvideo\
		-r $(FPS)\
		-pix_fmt rgba\
		-s $(WIDTH)x$(HEIGHT)\
		-i pipe:0 $@ -y

run: out.mp4
	$(FFPLAY) $^

clean:
	rm bf.exe out.mp4

.PHONY: all run
