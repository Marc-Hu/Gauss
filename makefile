all: run

run: clearMain compile
	./gauss

compile:
	gcc gauss.c -Wall -o gauss

clearMain:
	rm -rf gauss
