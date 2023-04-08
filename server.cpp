#include "Librerias/commonlibraries.h"
#include "constante_server.h"
#include "Parser/ParserRequest.h"
#include "Parser/ParserResponse.h"

using namespace std;
void show_client_ip(const sockaddr_storage &client_addr)
{
    //(IPv4 or IPv6)
    int addr_family = client_addr.ss_family;

    // declare variables to hold the IP address and port number
    char ip_str[INET6_ADDRSTRLEN];
    int port;

    // cast the sockaddr_storage pointer to the appropriate address type
    if (addr_family == AF_INET)
    {
        const sockaddr_in *addr = reinterpret_cast<const sockaddr_in *>(&client_addr);
        inet_ntop(addr_family, &addr->sin_addr, ip_str, sizeof(ip_str));
        port = ntohs(addr->sin_port);
    }
    else
    { // AF_INET6
        const sockaddr_in6 *addr = reinterpret_cast<const sockaddr_in6 *>(&client_addr);
        inet_ntop(addr_family, &addr->sin6_addr, ip_str, sizeof(ip_str));
        port = ntohs(addr->sin6_port);
    }

    // print the IP address and port number
    printf("IP de Cliente: %s\n", ip_str);
    printf("Puerto de cliente: %d\n", port);
}
void *handle_client(void *arg)
{
    int socket_cliente = *(int *)arg;
    char buffer[RECV_BUFFER_SIZE];
    int bytes_read;
    int numeroErrores = 0;
    // Receive data from the client
    while ((bytes_read = recv(socket_cliente, buffer, sizeof(buffer), 0)) > 0)
    {
        char bufferEnvio[RECV_BUFFER_SIZE];
        try
        {
            ParserRequest requestCliente = ParserRequest::deserializeRequest(string(buffer));
            requestCliente.printRequest();
            ParserResponse RespuestaCliente = ParserResponse::deserializeResponse(requestCliente);
            string res = RespuestaCliente.serializeResponse();
            strcpy(bufferEnvio, res.c_str());
            // quiero que este if compruebe si en el body hay un data para que sepa si es un file o no
            if(RespuestaCliente.getBody().getData() == ""){
                int file_fd = RespuestaCliente.getBody().getFile_fd();
                off_t offset = RespuestaCliente.getBody().getOffset();
                ssize_t count = RespuestaCliente.getBody().getCount();
                send(socket_cliente, bufferEnvio, strlen(bufferEnvio), 0);
                ssize_t bytes_sent = sendfile(socket_cliente, file_fd, &offset, count);
                if (bytes_sent== -1) {
                    std::cerr << "sendfile failed...\n";
                    exit(0);
                }
            }else{
                send(socket_cliente, bufferEnvio, strlen(bufferEnvio), 0);
            } 
            // funcion que nos diga que tipo de archivo vamos a retornar
        }
        catch (const exception &e) //Errores de sintaxis en la escritura del request.
        {
            cerr << "ERROR PROCESANDO PETICION:  " << e.what() << '\n';
            numeroErrores++;
            if(numeroErrores>5){close(socket_cliente);}
            ParserResponse RespuestaCliente = ParserResponse::handleMacroErrors(string(e.what()));
            string hola = RespuestaCliente.serializeResponse();
            strcpy(bufferEnvio, hola.c_str());
            int bytes_sent = send(socket_cliente, bufferEnvio, strlen(bufferEnvio), 0);
        }
    }

    // Close the client socket and exit the thread
    close(socket_cliente);
    pthread_exit(NULL);
}

void serverIni(int puerto)
{
    cout << "Iniciando servidor..." << endl;
    struct sockaddr_storage dir_client; // Aca se almacenara info de familia, puerto y dir IP del cliente
    socklen_t addr_size;
    int socketIni, socketCliente;
    // Creando el socket----
    socketIni = socket(AF_INET, SOCK_STREAM, 0); // Dominio en la red, tipo stream(tambien hay datagram)(orientado a la conexion),tipo de protocolo , TCP ==0 por default
    if (socketIni < 0)
    {
        perror("Error creando socket \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket creado \n");
    }
    // Bind del socket => asiganción ip y puerto
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP_SERVER);
    addr.sin_port = htons(puerto); // Puerto

    int ret = bind(socketIni, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        printf("error de bind en direccion %s \n", IP_SERVER);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Binding exitoso en puerto %d e IP %s \n", puerto, IP_SERVER);
    }
    cout << "finaliza binding" << endl;
    /// Escuchar los llamados de clientes
    if (listen(socketIni, 10) < 0)
    {
        perror("Error en listen socket");
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "El socket está escuchando..." << endl;
    }

    while (1)
    {
        addr_size = sizeof dir_client;
        // aceptando la primer coneccion en cola del listen
        socketCliente = accept(socketIni, (struct sockaddr *)&dir_client, &addr_size); // socket para manejar info del cliente conectado
        if (socketCliente < 0)
        {
            perror("Error creando socket del cliente \n");
        }
        show_client_ip(dir_client);
        pthread_t hiloClient;
        if (pthread_create(&hiloClient, NULL, handle_client, &socketCliente) != 0)
        {
            cerr << "Fallo al crear hilo para manejo de concurrencia de clientes" << endl;
            continue;
        }
        pthread_detach(hiloClient);
    }
    close(socketIni);
}
int main(int argc, char const *argv[])
{
    try
    {
        if (argc != 4)
        {
            throw 505;
        }
        serverIni(atoi(argv[1]));
    }
    catch (...)
    {
        cout << "No pasó argumentos necesarios" << endl;
    }

    return 0;
}