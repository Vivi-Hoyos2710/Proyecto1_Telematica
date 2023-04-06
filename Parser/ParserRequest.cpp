#include "ParserRequest.h"
ParserRequest::ParserRequest(const string &method, const string &resource, const map<string, string> &headers, const string &version) noexcept
    : version(version), method(method), resource(resource), headers(headers), bodyReq() {}

ParserRequest::ParserRequest(const string &method, const string &resource, const map<string, string> &headers, const string &version, Body &bodyReq) noexcept
    : version(version), method(method), resource(resource), headers(headers), bodyReq(bodyReq) {}
ParserRequest::~ParserRequest()
{
}
// getters
const string &ParserRequest::getMethod()
{
    return this->method;
}
const string &ParserRequest::getVersion(){
    return this->version;
}
const string &ParserRequest::getResource(){
    return this->resource;
}
const map<string,string> &ParserRequest::getHeaders(){
    return this->headers;
}
//
string ParserRequest::method_from_string(const string &metodo)
{
    if (metodo.compare("GET") == 0)
    {
        return metodo;
    }
    else if (metodo.compare("HEAD") == 0)
    {
        return metodo;
    }
    else if (metodo.compare("POST") == 0)
    {
        return metodo;
    }
    else
    {

        throw invalid_argument("Method Not Allowed");
    }
}
string ParserRequest::HTTPversion_from_string(const string &version)
{
    if (version.compare("HTTP/1.1") == 0)
    {
        return version;
    }
    else
    {
        throw invalid_argument("HTTP Version Not Supported");
    }
}
ParserRequest ParserRequest::deserializeRequest(const string &request)
{
    vector<string> lines = split(request, LINE_END);

    if (lines.size() < 1)
    {
        throw invalid_argument("BAD REQUEST");
    }

    vector<string> segments = split(lines[0], " ");
    int sizeRequest = segments.size();
    if (sizeRequest != 3)
    {
        throw invalid_argument("BAD REQUEST");
    }

    const string metodo = method_from_string(segments[0]);
    const string resource = segments[1];
    const string version = HTTPversion_from_string(segments[2]);
    map<string, string> headers;
    int i = 1;
    while (lines[i].size() > 0)
    {
        const HeaderClass header = HeaderClass::deserialize(lines[i]);
        headers[header.get_key()] = header.get_value();
        i++;
    };

    if (metodo == "POST" && headers.find("Content-Length") != headers.end())
    {
        string bodyString;
        string content = headers.at("Content-Length");

        if (stoi(content) > 0)
        {
            for (i; i < lines.size(); i++)
            {
                if (lines[i].size() > 0)
                {
                    bodyString += lines[i];
                }
            }
        }

        string tipoContenido = headers.at("Content-Type");
        Body bodyRequest = Body::clasificarType(tipoContenido, bodyString);
        return ParserRequest(metodo, resource, headers, version, bodyRequest);
    }

    return ParserRequest(metodo, resource, headers, version);
}
void ParserRequest::printRequest()
{
    cout << "-------Inicia print request---------------" << endl;
    cout << this->method << " " << this->resource << " " << this->version << " " << endl;
    string headerString;
    for (const auto &header : this->headers)
    {
        cout << header.first << ": " << header.second << endl;
    }
    cout << headerString << endl;
    cout << this->bodyReq.getData() << endl;
    cout << "-------termina print request---------------" << endl;
};