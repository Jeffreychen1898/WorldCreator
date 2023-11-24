APP_NAME := WorldCreator

CXX := g++
CXXFLAGS := -std=c++17

COMPILE_DEFS := -DDEBUG_MODE

LIBS_FLAG := -lGL -lX11 -lpthread -ldl

SRC := $(shell find ./src -name "*.cpp")
OBJ := $(SRC:./src/%.cpp=./obj/%.o)
FOLDERS_OBJ := $(sort $(dir $(OBJ)))

DEPS_INC := $(patsubst %,-I%,$(shell find ./deps -mindepth 1 -maxdepth 1 -type d))
DEPS_MAKEFILES := $(dir $(shell find ./deps -maxdepth 2 -name "Makefile"))
DEPS_LIB := $(shell find ./deps -maxdepth 2 -name "library.a")

# commands to build and clean dependencies
# must build deps before attempting to build target since build target
# will not build deps by default
.PHONY: build_deps
build_deps:
	$(foreach makefile,$(DEPS_MAKEFILES),make -C $(makefile) all;)

.PHONY: clean_deps
clean_deps:
	$(foreach makefile,$(DEPS_MAKEFILES),make -C $(makefile) clean;)

# create the folders to store the obj files
.PHONY: create_folder_obj
create_folder_obj:
	mkdir -p $(FOLDERS_OBJ)

# compile all the .cpp into .o
$(OBJ): ./obj/%.o : ./src/%.cpp | create_folder_obj
	$(CXX) $(CXXFLAGS) $(COMPILE_DEFS) -c -o $@ $< $(DEPS_INC)

# build the application
$(APP_NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(COMPILE_DEFS) -o $@ $^ $(DEPS_LIB) $(DEPS_INC) $(LIBS_FLAG)

# build the dependencies and the application
.PHONY: all
all:
	make build_deps
	make $(APP_NAME)

.PHONY: run
run:
	make build_deps
	make $(APP_NAME)
	./$(APP_NAME)

# cleanup
.PHONY: clean
clean:
	rm -rf obj $(APP_NAME) imgui.ini
