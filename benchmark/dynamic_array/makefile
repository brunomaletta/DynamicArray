FLAGS = -std=c++17 -O2

all: average dyn_array
	bash ./run.sh

average: average.cpp
	g++ -o average average.cpp $(FLAGS)
	
dyn_array: dyn_array.cpp
	g++ -o dyn_array dyn_array.cpp $(FLAGS)

clean:
	rm -f input output average dyn_array
