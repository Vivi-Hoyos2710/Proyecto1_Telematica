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

// Metodos
void ParserResponse::handleHeadReq(const map<string, string> &reqheaders)
{
    this->responseCode = OK;
    this->headers = reqheaders;
    this->body.setData(" ");
}
void ParserResponse::handleGetReq() {}
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
        RespuestaCliente.handleGetReq();
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
