compile : main.cpp graph.cpp
	g++ -std=c++11 main.cpp graph.cpp -o prog

prob1 :
	./prog --reverse --graph=g.in

prob2 :
	./prog --longestcycle --graph=g.in

prob3 :
	./prog --maxarbitrage --graph=g.in

all :
	./prog --reverse --graph=g.in
	./prog --longestcycle --graph=g.in
	./prog --maxarbitrage --graph=g.in

clean :
	rm prog
