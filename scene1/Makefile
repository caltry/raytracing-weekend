INCLUDES += -I../include/
CXXFLAGS += $(INCLUDES) -O2 -pthread -std=c++11

all: scene.png
scene.o: $(wildcard ../include/*.hpp ../include/*.h)

scene.ppm: scene
	time ./$< >$@

%.png: %.ppm
	convert $< $@

%: %.o
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	$(RM) *.o

realclean: clean
	$(RM) scene scene.ppm scene.png
