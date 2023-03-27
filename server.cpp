#include "commonlibraries.h"
#include "ParserRequest.h"
using namespace std;
void show_client_ip(const sockaddr_storage& client_addr) {
    // determine the address family (IPv4 or IPv6)
    int addr_family = client_addr.ss_family;

    // declare variables to hold the IP address and port number
    char ip_str[INET6_ADDRSTRLEN];
    int port;

    // cast the sockaddr_storage pointer to the appropriate address type
    if (addr_family == AF_INET) {
        const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(&client_addr);
        inet_ntop(addr_family, &addr->sin_addr, ip_str, sizeof(ip_str));
        port = ntohs(addr->sin_port);
    } else { // AF_INET6
        const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(&client_addr);
        inet_ntop(addr_family, &addr->sin6_addr, ip_str, sizeof(ip_str));
        port = ntohs(addr->sin6_port);
    }

    // print the IP address and port number
    printf("Client IP: %s\n", ip_str);
    printf("Client port: %d\n", port);
}
void *handle_client(void *arg)
{
    int socket_cliente = *(int *)arg;
    char buffer[RECV_BUFFER_SIZE];
    int bytes_read;

    // Receive data from the client
    while ((bytes_read = recv(socket_cliente, buffer, sizeof(buffer), 0)) > 0)
    {
        
        cout << "Received data: " << buffer << endl;
        ParserRequest requestCliente= ParserRequest::deserializeRequest(string(buffer));
        string metodo= ParserRequest::method_from_string("GET");
        send(socket_cliente, buffer, bytes_read, 0);
    }

    // Close the client socket and exit the thread
    close(socket_cliente);
    pthread_exit(NULL);
}

void serverIni(int puerto)
{
    cout << "Iniciando servidor..." << endl;
    
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

    while (true)
    {
        struct sockaddr_storage dir_client; // Aca se almacenara info de familia, puerto y dir IP del cliente
        addr_size = sizeof dir_client;
        
        // aceptando la primer coneccion en cola del listen
        socketCliente = accept(socketIni, (struct sockaddr *)&dir_client, &addr_size); // socket para manejar info del cliente conectado
        if (socketCliente < 0)
        {
            perror("Error de conexion con cliente \n");
            continue;
        }
        show_client_ip(dir_client);
        //Creación de hilos
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
