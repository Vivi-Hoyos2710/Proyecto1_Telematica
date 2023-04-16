OBJ_DIR=obj/
DIR_UT=Parser/Utility_Functions/
OBJ_FILES= $(OBJ_DIR)server.o $(OBJ_DIR)ParserRequest.o $(OBJ_DIR)ParserResponse.o $(OBJ_DIR)log.o $(OBJ_DIR)Body.o $(OBJ_DIR)HeaderClass.o $(OBJ_DIR)utilityFunctions.o 

server:$(OBJ_FILES)
	g++ -o $@ $^

$(OBJ_DIR)server.o: server.cpp Librerias/commonlibraries.h constante_server.h Parser/ParserRequest.h Parser/ParserResponse.h log/log.h
	g++ -c server.cpp -o $(OBJ_DIR)server.o

$(OBJ_DIR)ParserRequest.o: Parser/ParserRequest.cpp Parser/ParserRequest.h $(OBJ_DIR)HeaderClass.o $(OBJ_DIR)Body.o
	g++ -c Parser/ParserRequest.cpp -o $(OBJ_DIR)ParserRequest.o

$(OBJ_DIR)ParserResponse.o: Parser/ParserResponse.cpp Parser/ParserResponse.h  $(OBJ_DIR)ParserRequest.o
	g++ -c Parser/ParserResponse.cpp -o $(OBJ_DIR)ParserResponse.o

$(OBJ_DIR)log.o: log/Log.cpp log/log.h
	g++ -c log/Log.cpp -o $(OBJ_DIR)log.o

$(OBJ_DIR)HeaderClass.o: Parser/HeaderClass.cpp Parser/HeaderClass.h Librerias/StringLibraries.h $(OBJ_DIR)utilityFunctions.o
	g++ -c Parser/HeaderClass.cpp -o $(OBJ_DIR)HeaderClass.o

$(OBJ_DIR)Body.o: Parser/Body.cpp Parser/Body.h Librerias/StringLibraries.h 
	g++ -c Parser/Body.cpp -o $(OBJ_DIR)Body.o

$(OBJ_DIR)utilityFunctions.o: $(DIR_UT)utilityFunctions.cpp $(DIR_UT)utilityFunctions.h Librerias/StringLibraries.h
	g++ -c $(DIR_UT)utilityFunctions.cpp -o $(OBJ_DIR)utilityFunctions.o

clean:
	rm $(OBJ_DIR)*.o server
cleanLog:
	rm log/*.txt
