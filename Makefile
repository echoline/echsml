ALL: test

test: doc.o parser.o test.c
	cc -g -o test test.c doc.o parser.o

doc.o: doc.c doc.h
	cc -g -c doc.c

parser.o: parser.c doc.h
	cc -g -c parser.c

clean:
	rm -f *.o test
