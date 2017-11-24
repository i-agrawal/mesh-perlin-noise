object := main.o noise.o
output := simul

CFLAGS := -O2 -Wno-deprecated
LFLAGS := -framework OpenGL -framework GLUT

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(output) : $(object)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

.PHONY: clean

clean :
	rm -rf $(object) $(output)