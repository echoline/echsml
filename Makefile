ALL: test

test: doc.o doctree.o parser.o test.c
	cc -g -o test test.c doc.o doctree.o parser.o

doc.o: doc.c doc.h
	cc -g -c doc.c

doctree.o: doctree.c doctree.h
	cc -g -c doctree.c

parser.o: parser.c parser.h
	cc -g -c parser.c

clean:
	rm -f *.o test
