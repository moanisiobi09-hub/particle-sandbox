# compiler of choice for this project
CXX := g++
# SFML compiler flags
SFMLFLAGS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
# C++ compiler flags
CPPFLAGS := -std=c++20 -c
# include path for SFML
INCLUDE := "external/SFML-3.0.2/include"
# include path for SFML during linking
LINK := "external/SFML-3.0.2/lib"
# all of the source files for this project, the entry point in Main.cpp
SRCS := $(wildcard src/**/*.cpp) src/Main.cpp
# a list of all the object files needed for compilation
OBJS := $(addprefix build/obj/,$(notdir $(patsubst %.cpp, %.o, $(SRCS))))

# the main rule for creating compiling the project
all: sandbox.exe

# the rule for creating the central executable file
sandbox.exe: $(OBJS)
	@echo "Creating sandbox.exe executable file"
	$(CXX) $(OBJS) -o build/bin/sandbox.exe -L$(LINK) $(SFMLFLAGS)

# the rule for compiling every .cpp file in this projects (except for the entry point)
build/obj/%.o: src/**/%.cpp
	@echo "Compiling $< source file"
	$(CXX) $(CPPFLAGS) $< -I$(INCLUDE) -o build/obj/$(notdir $(patsubst %.cpp, %.o, $<))

# rule for compiling the entry point
build/obj/Main.o: src/Main.cpp
	@echo "Compiling entry point"
	$(CXX) $(CPPFLAGS) src/Main.cpp -I$(INCLUDE) -o build/obj/Main.o

# rule to run the final executable
run: build\bin\sandbox.exe
	./build/bin/sandbox.exe

# clean rule to delete all the ojects and the central executable, we need to replace backslashes with forward slahses
clean:
	@echo "Removing object and executable files"
	del /Q $(subst /,\, $(OBJS)) build\bin\sandbox.exe