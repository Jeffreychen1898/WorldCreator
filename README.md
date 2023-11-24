# World Creator

A simple program to create a 3d world

## Compiling
The current Makefile only compiles the program for Linux. I am still working on writing a Makefile for Windows.
#### Compiling the dependencies
	make build_deps
#### Building the program
	make WorldCreator
#### OR
#### Compile the dependencies and build the program
	make all
#### OR
#### Compile the dependencies, build the program, then run it
	make run

## Note

* Modifications to any libraries have a "--> MODIFIED BELOW" comment on top of the modified code

## Todo

* Implement the UI for creating and modifying properties of different shapes
* Implement a way to make shape geometry defined by an equation
* Implement a way to write custom shaders for each shape
  
## ToDo ... Eventually
  * Level of depth
  * Tessellation Shader
  * Use plugins (dlopen, dlsym, etc) for more control?
