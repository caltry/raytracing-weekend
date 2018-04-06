INCLUDES += -I../include/
CXXFLAGS += $(INCLUDES) -O2

all: scene.ppm scene.png
scene.o: ../include/vec3.hpp ../include/ray.h

scene.ppm: scene
	./$< >$@

%.png: %.ppm
	convert $< $@

%: %.o
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	$(RM) *.o

realclean: clean
	$(RM) scene scene.ppm scene.png
