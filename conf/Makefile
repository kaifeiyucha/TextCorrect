.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=conf
OTHER=-std=c++0x
LINKS=-lpthread
OBJS=test_config.o get_config.o
$(BIN):$(OBJS)
	$(CC) $(CFALGS) $^ -o $@ $(LINKS)
%.o:%.cpp
	$(CC) $(CFALGS) -c $< $(LINKS)
clean:
	rm -rf *.o $(BIN)
