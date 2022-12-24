all: say_something compile_demo1
say_something: 
	@echo "Compile C++ " 
compile_demo1: 
	g++ -o olcExampleProgram *.cpp   -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
