CXX := g++
CXXFLAGS := -std=c++17

# find all .cpp files
SRC := $(shell find . -name "*.cpp")
OBJ := $(SRC:%.cpp=%.o)

# compile the .cpp to .o
$(OBJ) : %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# build the library
.PHONY: all
all: $(OBJ)
	ar rcs library.a $^
	
.PHONY: clean
clean:
	rm -f *.o *.a
