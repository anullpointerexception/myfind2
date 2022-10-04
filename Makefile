#Makefile for myfind Project
#Authors: Max, Simon

all: myfind

myfind: main.cpp
	gcc main.cpp -lstdc++ -Wall -o myfind

clean:
	rm -f myfind