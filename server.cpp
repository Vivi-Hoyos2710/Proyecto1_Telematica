#include "Librerias/commonlibraries.h"
#include "constante_server.h"
#include "Parser/ParserRequest.h"
#include "Parser/ParserResponse.h"
#include <csignal>
#include "log/log.h"
using namespace std;

static string direccion_absoluta_DRF; // aquí declaro variable estática para guardar la dirAbsoluta del documentRootFolder
log logObjet;
void show_client_ip(const sockaddr_storage &client_addr)
{
    //(IPv4 or IPv6)
    int addr_family = client_addr.ss_family;

   
    char ip_str[INET6_ADDRSTRLEN];
    int port;

    
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
    int socketCliente = *(int *)arg;
    char buffer[RECV_BUFFER_SIZE];
    char bodyBuffer[RECV_BUFFER_SIZE];
    char bufferEnvio[RECV_BUFFER_SIZE];
    int bytes_read;
    int msgSize = 0;
    int content_length = 0;
    bool greater;
    // Receive data from the client
    // limpio buffer antes de leer.
    memset(buffer, 0, sizeof(buffer)); // clear the buffer before reading
    while (true)
    {
        
        bytes_read = recv(socketCliente, buffer + msgSize, RECV_BUFFER_SIZE - msgSize, 0);
        if (bytes_read <= 0)
        {
            close(socketCliente);
            pthread_exit(NULL);

            break;
        }
        msgSize += bytes_read;
        buffer[msgSize] = '\0';
        char *end = strstr(buffer, "\r\n\r\n");
        int headerSize = end - buffer;
        if (end != NULL)
        {
            char *content_length_str = strstr(buffer, "Content-Length:");
            if (content_length_str != NULL)
            {
                content_length = atoi(content_length_str + strlen("Content-Length:"));
                
                if (msgSize - (headerSize + 4) == content_length)
                {
                    
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    try
    {
        
        ParserRequest requestCliente = ParserRequest::deserializeRequest(buffer);
        string request=requestCliente.requestToString();
        string tiempo = string(logObjet.getCurrentTime());
        logObjet.appendToLog(request);
        logObjet.appendToLog(tiempo);
        cout<<request+'\n'<<tiempo<<endl;
        ParserResponse RespuestaCliente = ParserResponse::deserializeResponse(requestCliente, direccion_absoluta_DRF);
        string res = RespuestaCliente.serializeResponse();
        string tiempoRes = string(logObjet.getCurrentTime());
        string shortResponse=RespuestaCliente.shortResponse();
        logObjet.appendToLog(shortResponse);
        logObjet.appendToLog(tiempoRes);
        cout<<shortResponse+'\n'<<tiempoRes<<endl;
        strcpy(bufferEnvio, res.c_str());
        ssize_t bytes_sent = send(socketCliente, bufferEnvio, strlen(bufferEnvio), 0);
        if (bytes_sent == -1)
        {
            std::cerr << "error enviando response...\n";
        }

        if (requestCliente.getMethod() == "GET" && RespuestaCliente.getBody().getData() == "")
        {
            int file_fd = RespuestaCliente.getBody().getFile_fd();
            off_t offset = RespuestaCliente.getBody().getOffset();
            ssize_t count = RespuestaCliente.getBody().getCount();
            ssize_t bytes_sent = sendfile(socketCliente, file_fd, &offset, count);

            if (bytes_sent == -1)
            {
                std::cerr << "sendfile failed...\n";
            }
            close(file_fd);
        }

        memset(bufferEnvio, 0, sizeof(buffer));
    }
    catch (const exception &e) // Errores de sintaxis en la escritura del request.
    {
        cerr << "ERROR PROCESANDO PETICION:  " << e.what() << '\n';
        ParserResponse RespuestaCliente = ParserResponse::handleMacroErrors(string(e.what()));
        string errores = RespuestaCliente.serializeResponse();
        string shortRes= RespuestaCliente.shortResponse()+" "+string(e.what());
        logObjet.appendToLog(shortRes);
        string tiempoRes = string(logObjet.getCurrentTime());
        logObjet.appendToLog(tiempoRes);
        strcpy(bufferEnvio, errores.c_str());
        int bytes_sent = send(socketCliente, bufferEnvio, strlen(bufferEnvio), 0);

    }
    close(socketCliente);
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
        int opt = 1;
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
        addr_size = sizeof dir_client;
        // aceptando la primer coneccion en cola del listen
        socketCliente = accept(socketIni, (struct sockaddr *)&dir_client, &addr_size); // socket para manejar info del cliente conectado
        if (socketCliente < 0)
        {
            perror("Error creando socket del cliente \n");
            break;
        }
        show_client_ip(dir_client);
        pthread_t hiloClient;
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        if (setsockopt(socketCliente, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
            perror("setsockopt");
            close(socketCliente);
            continue;
        }
        if (pthread_create(&hiloClient, NULL, handle_client, &socketCliente) != 0)
        {
            cerr << "Fallo al crear hilo para manejo de concurrencia de clientes" << endl;
            continue;
        }
        pthread_detach(hiloClient);
    }
    cout << "Apagando servidor..." << endl;
    close(socketIni);
}

int main(int argc, char const *argv[])
{
    try
    {
        int port=atoi(argv[1]);
        if (argc != 4)
        {
            throw invalid_argument("Faltan argumentos para ejecutar el servidor: <HTTP PORT> <Log File> <DocumentRootFolder>");
        }
        fs::path directorio(argv[3]);
        if (port==0){
             throw invalid_argument("Puerto no valido");
        }
        if (!filesystem::exists(directorio))
        {
            throw runtime_error("Directorio para <DocumentRootFolder> no existe");
        }
        direccion_absoluta_DRF = fs::absolute(directorio).string();
        string logName=argv[2];
        logObjet= log();
        logObjet.createLog(logName);

        serverIni(port);
        
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';
    }

    return 0;
}