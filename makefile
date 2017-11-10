VPATH = src
CPP = g++
CFLAGS = -O0 -std=c++14 -fpermissive -g
OBJ = log.o attotime.o main.o mem.o cpu.o cpu_arith.o cpu_io.o cpu_jump.o cpu_misc.o cpu_mov.o keyboard.o

tietokone: $(OBJ)
	$(CPP) $(OBJ) -o "tietokone"

all: tietokone

clean:
	rm *.o

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<
