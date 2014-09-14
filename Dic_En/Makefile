.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=dict
OBJS=lexicon.o lexi_main.o
$(BIN):$(OBJS)
	$(CC)  $(CFLAGS) $^ -o $@ 
%.o:%.cpp
	$(CC) $(CFLAGS) -c $<  
clean:
	rm -rf *.o $(BIN)  DicEn.txt 
