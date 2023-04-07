#include "ParserResponse.h"
string documentRoot = "/mnt/c/users/usuario/documents/telematica/Proyecto1_Telematica/documentRootFolder";
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

int ParserResponse::verificarDir(string rutaAbuscar){
    fs::path directory_path = documentRoot; // directorio donde buscar
    fs::path input_path = documentRoot + rutaAbuscar; // path que se está buscando

    for (const auto& entry : fs::recursive_directory_iterator(directory_path)) {
        if (fs::exists(input_path)) {
            return 1;
            break;
        }
    }
    return 0;

}

Body ParserResponse::getBody(){
    return this->body;
}

// Metodos
void ParserResponse::handleHeadReq(const map<string, string> &reqheaders)
{
    this->responseCode = OK;
    this->headers = reqheaders;
    this->body.setData(" ");
}
void ParserResponse::handleGetReq(string path) {
    int existe = verificarDir(path);
    if(!existe){
        //no encontro la ruta
        this->responseCode=NOT_FOUND;
        string tipo = "text/html";
        string data = "<!DOCTYPE html>\r\n<html><head><title> 404 Not found</title></head><body><h1> 404 Not Found </h1><p> No hemos encontrado la ruta que buscas</p></body></html>";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Content-Type", "text/html"},
            {"Content-Length", to_string(data.length())}};
        this->headers = cabecera;
        this->body = nuevoBody;
    }else{
        string tipo = "image/jpeg";// aca le pongo el content type
        fs::path input_path = documentRoot + path; // aca concateno la document root y la path que busco
        const char *cstr = input_path.c_str();
        int file_fd = open(cstr, O_RDONLY);
        off_t offset = 0;
        struct stat file_stat;
        if (fstat(file_fd, &file_stat) < 0) {
            cout << "Error al obtener la información del archivo" << endl;   
        }
        fstat(file_fd, &file_stat);
        Body nuevoBody = Body(tipo, file_fd, offset, file_stat.st_size);
        map<string,string> cabecera = {
            {"Content-Type", "image/jpeg"},
            {"Content-Length", to_string(file_stat.st_size)}
        };
        this->responseCode=OK;
        this->headers = cabecera;
        this->body = nuevoBody;
        

    }

    
}
void ParserResponse::handlePostReq()
{
}

ParserResponse ParserResponse::deserializeResponse(ParserRequest &request)
{
    ParserResponse RespuestaCliente = ParserResponse(request.getVersion()); // objeto clase respuesta con version HTTP/1.1
    if (request.getMethod().compare("HEAD") == 0)
    {
        RespuestaCliente.handleHeadReq(request.getHeaders());
    }
    else if (request.getMethod().compare("GET") == 0)
    {
        RespuestaCliente.handleGetReq(request.getResource());
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

    // Append HTTP version and status code
    response_str += this->version + " " + to_string(responseCode) + "\r\n";

    // Append headers
    for (const auto &header : headers)
    {
        response_str += header.first + ": " + header.second + "\r\n";
        cout << response_str << endl;
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
        string data = "Metodo no permitido: use HEAD,POST o GET";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Allow", "HEAD,GET,POST"},
            {"Content-Type", "text/plain"},
            {"Content-Length", to_string(data.length())}};
        return ParserResponse(405, "HTTP/1.1", cabecera, nuevoBody);
    }
    else if (error.compare("HTTP Version Not Supported") == 0)
    {
        string tipo = "text/html";
        string data = "<!DOCTYPE html>\r\n< html ><head><title> 400 Bad Request</ title></ head><body><h1> Bad Request</ h1><p> Your browser sent a request that this server could not understand.</ p></ body></ html>";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Connection", "close"},
            {"Content-Length", to_string(data.length())}};
            return ParserResponse(505, "HTTP/1.1", cabecera, nuevoBody);
    }
    else if (error.compare("BAD REQUEST") == 0)
    {
        string tipo = "text/plain";
        string data = "Petición invalida";
        Body nuevoBody = Body(tipo, data);
        map<string, string> cabecera = {
            {"Content-Type", "text/plain"},
            {"Content-Length", to_string(data.length())}};
        return ParserResponse(400, "HTTP/1.1", cabecera, nuevoBody);
    }
    else{
        return ParserResponse("HTTP/1.1");
    }
}
