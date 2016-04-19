:: todo: change to [cq]make
::
:: The dynamic linker can't find OR finds wrong version of libstdc++-6.dll
:: The easiest way to fix it is to compile with -static instead.
g++ -std=c++14 -Wall -Wextra -static -ggdb main.cpp -o main.exe && main.exe testcases\\testcase1
