Prueba: server.o ParserRequest.o HeaderClass.o utilityFunctions.o
	g++ -o $@ server.o ParserRequest.o HeaderClass.o utilityFunctions.o

server.o: server.cpp commonlibraries.h
	g++ -c server.cpp

ParserRequest.o: ParserRequest.cpp ParserRequest.h commonlibraries.h HeaderClass.h
	g++ -c ParserRequest.cpp

HeaderClass.o: HeaderClass.cpp HeaderClass.h utilityFunctions.h StringLibraries.h
	g++ -c HeaderClass.cpp 

utilityFunctions.o: utilityFunctions.cpp utilityFunctions.h StringLibraries.h
	g++ -c utilityFunctions.cpp 

clean:
	rm *.o