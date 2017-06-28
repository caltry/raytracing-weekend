INCLUDES += -I../include/
CXXFLAGS += $(INCLUDES) -O2

all: scene.ppm
scene.o: ../include/vec3.hpp ../include/ray.h

scene.ppm: scene
	./$< >$@

%: %.o
	$(CXX) $(CXXFLAGS) $< -o $@
