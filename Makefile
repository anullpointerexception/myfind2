#Makefile for myfind Project
#Authors: Max, Simon

all: myfind

myfind: main.cpp
	gcc main.cpp -lstdc++ -Wall -o myfind -std=gnu++17

clean:
	rm -f myfind
