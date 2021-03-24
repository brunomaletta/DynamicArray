FLAGS = -std=c++17 -O2

all: average sms treap splay
	bash ./run.sh

average: average.cpp
	g++ -o average average.cpp $(FLAGS)
	
sms: sms.cpp
	g++ -o sms sms.cpp $(FLAGS)
	
treap: treap.cpp
	g++ -o treap treap.cpp $(FLAGS)
	
splay: splay.cpp
	g++ -o splay splay.cpp $(FLAGS)

clean:
	rm -f input output average sms treap splay
