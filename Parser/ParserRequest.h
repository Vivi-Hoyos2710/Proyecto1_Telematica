#ifndef PARSER_REQUEST_H_
#define PARSER_REQUEST_H_
#include "HeaderClass.h"

class ParserRequest
{
private:
    string version;
    string method;
    string resource;
    map<string, HeaderClass> headers;

public:
    ParserRequest(const string &method, const string &resource, const std::map<std::string, HeaderClass> &headers, const string &version) noexcept;
    ~ParserRequest();
    void printRequest(ParserRequest& peticion);
    static string method_from_string(const string &metodo);
    static string HTTPversion_from_string(const string &version);
    static ParserRequest deserializeRequest(const string &request);
};
#endif // PARSER_REQUEST_H_
