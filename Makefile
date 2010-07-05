all:	
	g++ -I. -fpic -c rtfparser.cpp -g -Wall -Wextra 
	g++ -I. -fpic -c rtfdoctype.cpp	 -g -Wall -Wextra
	g++ -I/usr/include/python2.5 -I. -fpic -c rtfpythoninterface.cpp -g -Wall
	g++ rtfdoctype.o rtfparser.o rtfpythoninterface.o  -fpic -shared -o _PyRtfParser.so -g -Wall
#	g++ rtfdoctype.o rtfparser.o -o gen -g