CFLAGS = -I../..
ifeq ($(OS),Windows_NT)
	LDFLAGS = -lopengl32 -lgdi32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LDFLAGS = -framework OpenGL -framework Cocoa
	else ifeq ($(UNAME_S),Linux)
		LDFLAGS = -lGLU -lGL -lX11
	endif
endif

demo : demo.cpp ../../tigr.c
	g++ $^ -s -Os -o $@ $(CFLAGS) $(LDFLAGS)
