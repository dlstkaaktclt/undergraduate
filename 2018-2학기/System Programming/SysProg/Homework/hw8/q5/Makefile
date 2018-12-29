CC = gcc

TARGETS = hello q5

all : $(TARGETS)

hello : hello.o
	$(CC) -o $@ $^

q5 : q5.o
	$(CC) -o $@ $^

clean :
	rm *.o hello q5
