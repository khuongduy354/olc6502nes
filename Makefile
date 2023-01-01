all: say_something compile_demo2
say_something: 
	@echo "Compile C++ " 
compile_demo1: 
	g++ -o olcExampleProgram *.cpp   -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
compile_demo2: 
	g++ -o  olcExampleProgram2 *.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

