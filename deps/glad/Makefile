CXX := g++
CXXFLAGS := -std=c++17

.PHONY: all
all:
	$(CXX) $(CXXFLAGS) -c -o glad.o glad.c
	ar rcs library.a glad.o

.PHONY: clean
clean:
	rm -f *.a *.o
