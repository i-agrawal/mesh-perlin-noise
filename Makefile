object := main.o noise.o
output := simul

CFLAGS := -O2 -Wno-deprecated

ifneq ($(OS),Windows_NT)
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LDLAGS := -lglut -lGLU -lGL
	endif
	ifeq ($(UNAME),Darwin)
		LDLAGS := -framework OpenGL -framework GLUT
	endif
endif

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(output) : $(object)
	$(CC) $(CFLAGS) $(LDLAGS) -o $@ $^

.PHONY: clean

clean :
	rm -rf $(object) $(output)