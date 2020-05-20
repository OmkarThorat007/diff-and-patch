all: project1 project2
project2: patch.o stack.o
	cc patch.o stack.o -o project2
project1: diff.o stack.o queue.o list.o clist.o
	cc diff.o stack.o queue.o list.o clist.o -o project1
stack.o: stack.c stack.h
	cc -c stack.c
list.o: list.c list.h
	cc -c list.c
clist.o: clist.c clist.h
	cc -c clist.c
queue.o: queue.c queue.h
	cc -c queue.c
diff.o: diff.c stack.h queue.h list.h clist.h
	cc -c diff.c
patch.o: patch.c stack.h
	cc -c patch.c
