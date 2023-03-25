#include "commonlibraries.h"
using namespace std;
void *handle_client(void *arg)
{
    int socket_cliente = *(int *)arg;
    char buffer[RECV_BUFFER_SIZE];
    int bytes_read;

    // Receive data from the client
    while ((bytes_read = recv(socket_cliente, buffer, sizeof(buffer), 0)) > 0)
    {
        // Process the data
        cout << "Received data: " << buffer << endl;
        cout << bytes_read << buffer << endl;
        // Echo the data back to the client
        send(socket_cliente, buffer, bytes_read, 0);
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
    else{
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
            exit(EXIT_FAILURE);
        }
        pthread_t hiloClient;
        if (pthread_create(&hiloClient, NULL, handle_client, &socketCliente) != 0)
        {
            cerr << "Fallo al crear hilo para manejo de concurrencia de clientes" << endl;
            continue;
            pthread_detach(hiloClient);
        }
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
