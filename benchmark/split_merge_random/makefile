FLAGS = -std=c++17 -O2

all: average sms sms_mo treap splay
	bash ./run.sh

average: average.cpp
	g++ -o average average.cpp $(FLAGS)
	
sms: sms.cpp
	g++ -o sms sms.cpp $(FLAGS)
	
sms_mo: sms_mo.cpp
	g++ -o sms_mo sms_mo.cpp $(FLAGS)
	
treap: treap.cpp
	g++ -o treap treap.cpp $(FLAGS)
	
splay: splay.cpp
	g++ -o splay splay.cpp $(FLAGS)

clean:
	rm -f input output average sms sms_mo treap splay
