#run:
#	source ~/srir/source_bash.sh && mpicxx -ggdb -std=c++1y map.cpp dijkstra.cpp main.cpp -DDEBUG -o main && mpiexec -n 2 ./main testcases/testcase0
all:
	mpicxx -ggdb -std=c++1y map.cpp dijkstra.cpp main.cpp -DDEBUG -o main
	
test0:
	mpiexec -n 3 ./main testcases/testcase0

test1:
	mpiexec -n 3 ./main testcases/testcase1
