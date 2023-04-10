#ifndef PARSER_REQUEST_H_
#define PARSER_REQUEST_H_
#include "HeaderClass.h"
#include "Body.h"
#include <stdexcept>
class ParserRequest
{
private:
    string version;
    string method;
    string resource;
    map<string, string> headers;
    Body bodyReq;

public:
    ParserRequest(const string &method, const string &resource, const map<string, string> &headers, const string &version) noexcept;
    ParserRequest(const string &method, const string &resource, const map<string, string> &headers, const string &version, Body &bodyReq) noexcept;
    ~ParserRequest();
    void printRequest();
    const string &getMethod();
    const string &getVersion();
    const string &getResource();
    Body &getBody();
    const map<string, string> &getHeaders();
    static string method_from_string(const string &metodo);
    static string HTTPversion_from_string(const string &version);
    static ParserRequest deserializeRequest(const char *request); // Devuelve objeto ParserRequest a partir de la linea entregada por el cliente
};
#endif // PARSER_REQUEST_H_
