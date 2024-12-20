PROG=programmet.exe
SOURCES=main.cpp
DEPS=
CC=g++
CFLAGS=-Wall  -std=c++20
DEBUG?=1

ifeq ($(DEBUG), 1)
	CFLAGS += -g
	OUTPUTDIR=bin/debug
	PROG=programmet-debug.exe
else
	CFLAGS += -g0 -O3
	OUTPUTDIR=bin/release
endif


OBJS =  $(SOURCES:.cpp=.o)

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

$%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $< 

