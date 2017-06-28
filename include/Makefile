all: test
test.o: test.cpp vec3.hpp ray.h

%: %.o
	$(CXX) $(CXXFLAGS) $< -o $@
