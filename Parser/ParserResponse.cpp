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

int ParserResponse::verificarDir(string rutaAbuscar,const string& absPath){
    fs::path directory_path = absPath; // directorio donde buscar
    fs::path inputPath = absPath + rutaAbuscar; // path que se está buscando

    for (const auto& entry : fs::recursive_directory_iterator(directory_path)) {
        if (fs::exists(inputPath)) {
            return 1;
            break;
        }
    }
    return 0;

}

string ParserResponse::extraerExtension(string rutaxd){
  fs::path path(rutaxd);
  string extension = path.extension().string();
  if(extension == ".jpg" || extension == ".jpeg"){
    return "image/jpeg";
  }else if(extension == ".html"){
    return "text/html";
  }else if(extension == ".css"){
    return "text/css";
  }else if(extension == ".json"){
    return "application/json";
  }else if(extension == ".gif"){
    return "image/gif";
  }
  return 0;
}

Body ParserResponse::getBody(){
    return this->body;
}

// Metodos
void ParserResponse::handleHeadReq(string path,const string& documentRootPath)
{
 int existe = verificarDir(path,documentRootPath);
    if(!existe){
        //no encontro la ruta
        this->responseCode=NOT_FOUND;
        string tipo = "text/html"; // 
        string data = "<!DOCTYPE html>\r\n<html><head><title> 404 Not found</title></head><body><h1> 404 Not Found </h1><p> No hemos encontrado la ruta del archivo que buscas</p></body></html>";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Content-Type", "text/html"},
            {"Content-Length", to_string(data.length())}};
        this->headers = cabecera;
        this->body = nuevoBody;
    }
    else{
        string tipo = extraerExtension(path);
        fs::path inputPath = documentRootPath + path; // aca concateno la document root y la path para usar el archivo
        const char *cstr = inputPath.c_str();
        int file_fd = open(cstr, O_RDONLY);
        off_t offset = 0;
        struct stat file_stat;
        map<string,string> cabecera = {
            {"Content-Type", tipo},
            {"Content-Length", to_string(file_stat.st_size)}
        };
        Body nuevoBody= Body();
        this->responseCode=OK;
        this->headers = cabecera;
        this->body = nuevoBody;

    }
    
}
void ParserResponse::handleGetReq(string path,const string& documentRootPath) { // con esta funcion estamos manejando los get que nos mandan
    int existe = verificarDir(path,documentRootPath);
    if(!existe){
        //no encontro la ruta
        this->responseCode=NOT_FOUND;
        string tipo = "text/html"; // 
        string data = "<!DOCTYPE html>\r\n<html><head><title> 404 Not found</title></head><body><h1> 404 Not Found </h1><p> No hemos encontrado la ruta que buscas</p></body></html>";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Content-Type", "text/html"},
            {"Content-Length", to_string(data.length())}};
        this->headers = cabecera;
        this->body = nuevoBody;
    }else{
        string tipo = extraerExtension(path);//Pendiente como hacer para organizar el content type segun el archivo
        fs::path inputPath = documentRootPath + path; // aca concateno la document root y la path para usar el archivo
        const char *cstr = inputPath.c_str();
        int file_fd = open(cstr, O_RDONLY);
        off_t offset = 0;
        struct stat file_stat;
        if (fstat(file_fd, &file_stat) < 0) {
            cout << "Error al obtener la información del archivo" << endl;   
        }
        fstat(file_fd, &file_stat);
        Body nuevoBody = Body(tipo, file_fd, offset, file_stat.st_size);
        map<string,string> cabecera = {
            {"Content-Type", tipo},
            {"Content-Length", to_string(file_stat.st_size)}
        };
        this->responseCode=OK;
        this->headers = cabecera;
        this->body = nuevoBody;
    }

    
}
void ParserResponse::handlePostReq(string path,const string& documentRootPath)// funcion para manejar las recibidas de lo post
{
    

}

ParserResponse ParserResponse::deserializeResponse(ParserRequest &request,const string& absPath) // funcion para deserializar los response
{
    
    ParserResponse RespuestaCliente = ParserResponse(request.getVersion()); // objeto clase respuesta con version HTTP/1.1
    if (request.getMethod().compare("HEAD") == 0)
    {
        RespuestaCliente.handleHeadReq(request.getResource(),absPath);
    }
    else if (request.getMethod().compare("GET") == 0)
    {
        RespuestaCliente.handleGetReq(request.getResource(),absPath);
    }
    else
    {
        RespuestaCliente.handlePostReq();
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
            {"Connection","close"},
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
            {"Connection","close"},
            {"Content-Length", to_string(data.length())}};
        return ParserResponse(BAD_REQUEST, "HTTP/1.1", cabecera, nuevoBody);
    }
    else{
        return ParserResponse("HTTP/1.1");
    }
}
