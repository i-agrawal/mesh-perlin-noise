object := main.o noise.o
output := simul

CFLAGS := -O2 -Wno-deprecated

ifneq ($(OS),Windows_NT)
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LIBS := -lglut -lGLU -lGL
	endif
	ifeq ($(UNAME),Darwin)
		LIBS := -framework OpenGL -framework GLUT
	endif
endif

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(output) : $(object)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

.PHONY: clean

clean :
	rm -rf $(object) $(output)