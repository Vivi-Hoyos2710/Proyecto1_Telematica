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
const string &ParserRequest::getVersion()
{
    return this->version;
}
const string &ParserRequest::getResource()
{
    return this->resource;
}
Body &ParserRequest::getBody()
{
    return this->bodyReq;
}
const map<string, string> &ParserRequest::getHeaders()
{
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
ParserRequest ParserRequest::deserializeRequest(const char *request)
{
    vector<string> lines = split(string(request), LINE_END);

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
        int contentLength = stoi(headers.at("Content-Length"));
        int bodyPositionStart = string(request).find(BODY_LINE, 0) + string(BODY_LINE).length();
        char *bodyBuffer = new char[contentLength + 1];
        int bytesRead = 0;
        cout<<bodyPositionStart<<contentLength<<endl;
        for (int i = bodyPositionStart; i < contentLength+bodyPositionStart; i++)
        {
            bodyBuffer[bytesRead] = request[i];
            bytesRead++;
        }
        bodyBuffer[bytesRead] = '\0';
        string tipoContenido = headers.at("Content-Type");
        Body bodyRequest = Body(tipoContenido, bodyBuffer,contentLength);
        delete[] bodyBuffer;
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
    if (this->method=="POST")
    {
        cout << this->bodyReq.getBuffer() << endl;
    }
    
    
    cout << "-------termina print request---------------" << endl;
};