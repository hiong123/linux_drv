CC = gcc
CFLAGS = -Wall -g -O0
test : test.c btree.c linkqueue.c linkstack.c
	    $(CC) $(CFLAGS) -o $@ $^

clean:
	    $(RM) test .*.sw?
