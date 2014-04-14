EXEC = pj

all: Project.cpp
	g++ -std=c++0x -o $(EXEC) Project.cpp 

clean:
	rm -rf pj *.o *~ *.gch