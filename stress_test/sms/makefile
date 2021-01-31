FLAGS = -std=c++17 -O2
DEBUG_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -g -Wall -Wshadow

all: a b gen
	bash ./stress.sh a b gen

debug: a_db b_db gen_db
	bash ./stress.sh a_db b_db gen_db
	
a: a.cpp
	g++ -o a a.cpp $(FLAGS)

a_db: a.cpp
	g++ -o a_db a.cpp $(FLAGS) $(DEBUG_FLAGS)
	
b: b.cpp
	g++ -o b b.cpp $(FLAGS)

b_db: b.cpp
	g++ -o b_db b.cpp $(FLAGS) $(DEBUG_FLAGS)
	
gen: gen.cpp
	g++ -o gen gen.cpp $(FLAGS)

gen_db: gen.cpp
	g++ -o gen_db gen.cpp $(FLAGS) $(DEBUG_FLAGS)

clean:
	rm -f out_a.txt out_b.txt in a b gen a_db b_db gen_db
