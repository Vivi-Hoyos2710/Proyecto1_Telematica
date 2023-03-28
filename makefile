OBJ_DIR=obj/

Prueba: $(OBJ_DIR)server.o $(OBJ_DIR)ParserRequest.o $(OBJ_DIR)HeaderClass.o $(OBJ_DIR)utilityFunctions.o
	g++ -o $@ $(OBJ_DIR)server.o $(OBJ_DIR)ParserRequest.o $(OBJ_DIR)HeaderClass.o $(OBJ_DIR)utilityFunctions.o

$(OBJ_DIR)server.o: server.cpp Librerias/commonlibraries.h
	g++ -c server.cpp -o $(OBJ_DIR)server.o

$(OBJ_DIR)ParserRequest.o: Parser/ParserRequest.cpp Parser/ParserRequest.h Parser/HeaderClass.h 
	g++ -c Parser/ParserRequest.cpp -o $(OBJ_DIR)ParserRequest.o

$(OBJ_DIR)HeaderClass.o: Parser/HeaderClass.cpp Parser/HeaderClass.h Parser/Utility_Functions/utilityFunctions.h Librerias/StringLibraries.h
	g++ -c Parser/HeaderClass.cpp -o $(OBJ_DIR)HeaderClass.o

$(OBJ_DIR)utilityFunctions.o: Parser/Utility_Functions/utilityFunctions.cpp Parser/Utility_Functions/utilityFunctions.h Librerias/StringLibraries.h
	g++ -c Parser/Utility_Functions/utilityFunctions.cpp -o $(OBJ_DIR)utilityFunctions.o

clean:
	rm $(OBJ_DIR)*.o Prueba
