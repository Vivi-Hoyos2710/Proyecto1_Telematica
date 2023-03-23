#ifndef COMMON_LIBRARIES
#define COMMON_LIBRARIES   
///SOCKET LIBRARIES
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <unistd.h>
//STRINGS Y MANEJO DE ERRORES
#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <stdio.h> 
#include <string> 
#include <cstring>
///Constantes 
#include "constante_server.h"
///Para manejo de los hilos
#include <thread>

#endif // !COMMON_LIBRARIES
