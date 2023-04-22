#include "ParserResponse.h"

// constructores
ParserResponse::ParserResponse(const string version) noexcept : version(version)
{
} // El vacio
ParserResponse::ParserResponse(int responseCode, const string version, const map<string, string> &headers, const Body &body) noexcept : responseCode(responseCode), version(version), headers(headers), body(body)
{
}

ParserResponse::~ParserResponse()
{
}

int ParserResponse::verificarDir(string rutaAbuscar, const string &absPath)
{
    fs::path directory_path = absPath;          // directorio donde buscar
    fs::path inputPath = absPath + rutaAbuscar; // path que se está buscando

    for (const auto &entry : fs::recursive_directory_iterator(directory_path))
    {
        if (fs::exists(inputPath))
        {
            return 1;
            break;
        }
    }
    return 0;
}

string ParserResponse::extraerExtension(string rutaxd)
{
    fs::path path(rutaxd);
    string extension = path.extension().string();
    if (extension == ".jpg" || extension == ".jpeg")
    {
        return "image/jpeg";
    }
    else if (extension == ".html")
    {
        return "text/html";
    }
    else if (extension == ".css")
    {
        return "text/css";
    }
    else if (extension == ".txt")
    {
        return "text/plain";
    }
    else if (extension == ".json")
    {
        return "application/json";
    }
    else if (extension == ".gif")
    {
        return "image/gif";
    }
    else if (extension == ".png")
    {
        return "image/png";
    }
    else if (extension == ".exe")
    {
        return "application/octet-stream";
    }
    else if (extension == ".docx")
    {
        return "application/msword";
    }
    else if (extension == ".xml")
    {
        return "application/xml";
    }
    else if (extension == ".pptx")
    {
        return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    }
    else if (extension == ".xlsx")
    {
        return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    }
    else
    {
        return "error";
    }
}
string ParserResponse::extensionFromContent(string contenido)
{
    if (contenido == "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")
    {
        return ".xlsx";
    }
    if (contenido == "application/xml")
    {
        return ".xml";
    }
    if (contenido == "application/octet-stream" || contenido == "application/x-msdos-program")
    {
        return ".exe";
    }
    if (contenido == "application/msword" || contenido == "application/vnd.openxmlformats-officedocument.wordprocessingml.document")
    {
        return ".docx";
    }
    if (contenido == "application/vnd.openxmlformats-officedocument.presentationml.presentation")
    {
        return ".pptx";
    }
    if (contenido == "image/jpeg")
    {
        return ".jpg";
    }
    if (contenido == "image/png")
    {
        return ".png";
    }
    if (contenido == "text/html")
    {
        return ".html";
    }

    if (contenido == "application/x-msdos-program")
    {
        return ".exe";
    }
    if (contenido == "text/css")
    {
        return ".css";
    }
    if (contenido == "application/json" || contenido == "application/x-www-form-urlencoded")
    {
        return ".json";
    }
    if (contenido == "image/gif")
    {
        return ".gif";
    }
    if (contenido == "text/plain")
    {
        return ".txt";
    }
    else
    {
        return "error";
    }
}
Body ParserResponse::getBody()
{
    return this->body;
}

// Metodos
void ParserResponse::handleHeadReq(string path, const string &documentRootPath)
{
    int existe = verificarDir(path, documentRootPath);
    if (!existe || extraerExtension(path).compare("error") == 0)
    {
        // no encontro la ruta
        this->responseCode = NOT_FOUND;
        Body nuevoBody = Body();
        map<string, string> cabecera = {};
        this->headers = cabecera;
        this->body = nuevoBody;
    }
    else
    {
        string tipo = extraerExtension(path);
        fs::path inputPath = documentRootPath + path; // aca concateno la document root y la path para usar el archivo
        const char *cstr = inputPath.c_str();
        int file_fd = open(cstr, O_RDONLY);
        off_t offset = 0;
        struct stat file_stat;
        map<string, string> cabecera = {
            {"Content-Type", tipo},
            {"Content-Length", to_string(file_stat.st_size)}};
        Body nuevoBody = Body();
        this->responseCode = OK;
        this->headers = cabecera;
        this->body = nuevoBody;
    }
}
void ParserResponse::handleGetReq(string path, const string &documentRootPath)
{ // con esta funcion estamos manejando los get que nos mandan
    int existe = verificarDir(path, documentRootPath);
    if (!existe || extraerExtension(path).compare("error") == 0)
    {
        // no encontro la ruta
        this->responseCode = NOT_FOUND;
        string tipo = "text/html"; //
        string data = "<!DOCTYPE html>\r\n<html><head><title> 404 Not found</title></head><body><h1> 404 Not Found </h1><p> No hemos encontrado la ruta que buscas</p></body></html>";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Content-Type", "text/html"},
            {"Content-Length", to_string(data.length())}};
        this->headers = cabecera;
        this->body = nuevoBody;
    }
    else
    {
        string tipo = extraerExtension(path);         // Pendiente como hacer para organizar el content type segun el archivo
        fs::path inputPath = documentRootPath + path; // aca concateno la document root y la path para usar el archivo
        const char *cstr = inputPath.c_str();
        int file_fd = -1;
        while (file_fd == -1)
        {
            file_fd = open(cstr, O_RDONLY);
            if (file_fd == -1)
            {
                cerr << "error" << endl;
            }
        }

        off_t offset = 0;
        struct stat file_stat;
        if (fstat(file_fd, &file_stat) < 0)
        {
            cout << "Error al obtener la información del archivo" << endl;
        }
        fstat(file_fd, &file_stat);
        Body nuevoBody = Body(tipo, file_fd, offset, file_stat.st_size);
        map<string, string> cabecera = {
            {"Content-Type", tipo},
            {"Content-Length", to_string(file_stat.st_size)}};
        this->responseCode = OK;
        this->headers = cabecera;
        this->body = nuevoBody;
    }
}
void ParserResponse::handlePostReq(string path, const string &documentRootPath, Body bodyReq, map<string, string> headers) // funcion para manejar las recibidas de lo post
{
    int existe = verificarDir(path, documentRootPath);
    if (!existe || !extraerExtension(path).compare("error") == 0)
    {
        // no encontro la ruta
        this->responseCode = NOT_FOUND;
        string tipo = "text/html"; //
        string data = "<!DOCTYPE html>\r\n<html><head><title> 404 Not found</title></head><body><h1> 404 Not Found </h1><p> No hemos encontrado la ruta que buscas</p></body></html>";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Content-Type", "text/html"},
            {"Content-Length", to_string(data.length())}};
        this->headers = cabecera;
        this->body = nuevoBody;
    }
    else if (bodyReq.getLen() != 0)
    {
        fs::path inputPath = documentRootPath + path;
        string contentType = bodyReq.getDataType();
        cout << contentType << endl;

        string downloadName;

        if (headers.find("nameFile") != headers.end())
        { // encuentra nombre de archivo en los headers de la request.

            downloadName = headers.at("nameFile");
        }
        // nombre deafult
        else
        {
            downloadName = "defaultName";
        }

        string extension = extensionFromContent(contentType);
        string nameFile = inputPath.string() + "/" + downloadName + extension;

        string keepAlive;
        if (headers.at("Connection") == "keep-alive")
        {
            keepAlive = "timeout=5, max=0 for response";
        }
        else
        {
            keepAlive = "close";
        }
        bool creado;
        if (contentType == "application/x-www-form-urlencoded")
        {
            string jsonFinal = "{";
            string info = string(bodyReq.getBuffer());
            cout<<"LLEGA: "<<info<<endl;
            vector<string> campos = split(info, "&");
            cout<<"size "<<campos.size()<<endl;
            for (int i = 0; i < campos.size(); i++)
            {
                vector<string> pareja = split(campos[i], "=");
                
                int pos=0;
                string delim="%20";
                string delim2="+";
                replace(pareja[0],delim," ");
                replace(pareja[0],delim2," ");
                replace(pareja[1],delim," ");
                replace(pareja[1],delim2," ");
                cout<<"Parejas: "<<pareja[0]<<" "<<pareja[1]<<endl;
                if(i==campos.size()-1){
                    jsonFinal += "\"" + pareja[0]+ "\"" + ":" +"\""+ pareja[1] +"\""+ "\n";
                }
                else{
                    jsonFinal += "\"" + pareja[0]+ "\"" + ":" +"\""+ pareja[1] +"\""+ "," + "\n";
                }
                
            }
            jsonFinal += "}";
            cout << jsonFinal << endl;
             creado = writeFile(nameFile, jsonFinal.c_str(),jsonFinal.length());
        }
        else{
             creado = writeFile(nameFile, bodyReq.getBuffer(), bodyReq.getLen());
        }
        
        if (creado)
        {
            this->responseCode = CREATED;
            string responseB = "Archivo " + downloadName + " creado satisfactoriamente";
            map<string, string> cabecera = {
                {"Location", nameFile},
                {"Content-Length", to_string(responseB.length())},
                {"Keep-Alive", keepAlive}};
            this->headers = cabecera;
            Body nuevoBody = Body(contentType, responseB);
            this->body = nuevoBody;
        }
    }
    else
    {
        throw invalid_argument("BAD REQUEST");
    }
}

ParserResponse ParserResponse::deserializeResponse(ParserRequest &request, const string &absPath) // funcion para deserializar los response
{

    ParserResponse RespuestaCliente = ParserResponse("HTTP/1.1"); // objeto clase respuesta con version HTTP/1.1
    if (request.getMethod().compare("HEAD") == 0)
    {
        RespuestaCliente.handleHeadReq(request.getResource(), absPath);
    }
    else if (request.getMethod().compare("GET") == 0)
    {
        RespuestaCliente.handleGetReq(request.getResource(), absPath);
    }
    else
    {
        RespuestaCliente.handlePostReq(request.getResource(), absPath, request.getBody(), request.getHeaders());
    }
    return RespuestaCliente;
}
string ParserResponse::serializeResponse()
{
    string response_str;

    // Primera linea de respuesta
    response_str += this->version + " " + to_string(responseCode) + "\r\n";

    // Headers
    for (const auto &header : headers)
    {
        response_str += header.first + ": " + header.second + "\r\n";
    }

    // Append end of headers marker
    response_str += "\r\n";

    // Append body
    response_str += body.getData();
    return response_str;
}
ParserResponse ParserResponse::handleMacroErrors(const string error)
{
    if (error.compare("Method Not Allowed") == 0)
    {
        string tipo = "text/plain";
        string data = "Metodo no permitido: use HEAD,POST o GET\r\n";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Allow", "HEAD,GET,POST"},
            {"Connection", "close"},
            {"Content-Type", "text/plain"},
            {"Content-Length", to_string(data.length())}};
        return ParserResponse(METHOD_NOT_ALLOWED, "HTTP/1.1", cabecera, nuevoBody);
    }
    else if (error.compare("HTTP Version Not Supported") == 0)
    {
        string tipo = "text/html";
        string data = "<!DOCTYPE html>\r\n<html><head><title> HTTP version not supported</title></head><body><h1> Bad Request</h1><p> Your browser sent a request that this server could not understand.</p></body></html>\r\n";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Connection", "close"},
            {"Content-Length", to_string(data.length())}};
        return ParserResponse(HTTP_VERSION_NOT_SUPPORTED, "HTTP/1.1", cabecera, nuevoBody);
    }
    else if (error.compare("BAD REQUEST") == 0)
    {
        string tipo = "text/plain";
        string data = "Petición invalida: BAD REQUEST\r\n";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Content-Type", "text/plain"},
            {"Connection", "close"},
            {"Content-Length", to_string(data.length())}};
        return ParserResponse(BAD_REQUEST, "HTTP/1.1", cabecera, nuevoBody);
    }
    else
    {
        return ParserResponse("HTTP/1.1");
    }
}
int ParserResponse::writeFile(const string &filename, const char *buffer, size_t bufferSize)
{

    cout<<"WRITE: "<<buffer<<endl;
    ofstream file(filename, ios::binary | ios::app);
    if (file.is_open())
    {
        file.write(buffer, bufferSize);
        file << "\n";
        file.close();
        cout << "File saved successfully\n";
        return true;
    }
    else
    {
        cerr << "Unable to open file for writing\n";
        return false;
    }
}

string ParserResponse::shortResponse()
{

    string response = this->version + " " + to_string(responseCode);
    return response;
}
