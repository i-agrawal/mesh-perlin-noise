object := main.o noise.o
output := simul

CFLAGS := -O2 -Wno-deprecated

ifneq ($(OS),Windows_NT)
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Darwin)
		LFLAGS := -lGL -lglut
	endif
	ifeq ($(UNAME),Darwin)
		LFLAGS := -framework OpenGL -framework GLUT
	endif
endif

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(output) : $(object)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

.PHONY: clean

clean :
	rm -rf $(object) $(output)