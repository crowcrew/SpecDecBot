#This is the makefile used by the make utility to compile the SpecDecBot project
SPECDECBOT : main.cpp sql.hpp
	sudo g++ main.cpp -o ../bin/main -l sqlite3 -std=c++14
	sudo chmod +x ../bin/main
	sudo chmod 777 ../bin/main
	../bin/main

