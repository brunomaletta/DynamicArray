FLAGS = -std=c++17 -O2

all: sample
	./sample

sample: sample.cpp
	g++ -o sample sample.cpp $(FLAGS)

clean:
	rm -f sample
