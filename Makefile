CC := g++
LIBS := gl glu freeglut
CFLAGS := --std=c++14 `pkg-config --cflags $(LIBS)`
LFLAGS := `pkg-config --libs $(LIBS)`

FILES := Math Transformation Scene Graphics
CFILES := $(FILES:%=%.cc)
OFILES := main.o $(FILES:%=%.o)

a.out: $(OFILES)
	$(CC) $(CFLAGS) $^ $(LFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

$(CFILES): %.cc: %.h

clean:
	rm -rf *.o *.out

rebuild: clean a.out
