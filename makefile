all: Edge.o main.o
	g++ -std=c++11 -lrt main.o Edge.o -lpthread

Edge.o: Edge.cpp Edge.hpp
	g++ -std=c++11 -c -lrt Edge.cpp -o Edge.o -lpthread

main.o: main.cpp Edge.hpp Car.hpp
	g++ -std=c++11 -c -lrt main.cpp -o main.o -lpthread

clean:
	rm -rf *.o a.out