#include "ParserRequest.h"
ParserRequest::ParserRequest(const string &method, const string &resource, const map<string, HeaderClass> &headers, const string &version) noexcept : version(version), headers(headers), method(method), resource(resource)
{
}
ParserRequest::~ParserRequest()
{
}

string ParserRequest::method_from_string(const string &metodo)
{
    cout << "Metodo " << metodo << endl;
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
        perror("Metodo inválido");
        return "ERROR";
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
        perror("Version inválida");
        return "ERROR";
    }
}
ParserRequest ParserRequest::deserializeRequest(const string &request)
{
    vector<string> lines = split(request, LINE_END);

    if (lines.size() < 1)
    {
        throw runtime_error("HTTP Request ('" + string(request) + "') tiene " + to_string(lines.size()) + " lineas, deberian ser más de una.");
    }

    vector<string> segments = split(lines[0], " ");

    if (segments.size() != 3)
    {
        throw runtime_error("First line of HTTP request ('" + string(request) + "') consisted of " + to_string(segments.size()) + " space separated segments, should be 2.");
    }

    const string metodo = method_from_string(segments[0]);
    const string resource = segments[1];
    const string version = HTTPversion_from_string(segments[2]);

    map<string, HeaderClass> headers;

    for (size_t i = 1; i < lines.size(); i++)
    {
        if (lines[i].size() > 0)
        {
            const HeaderClass header = HeaderClass::deserialize(lines[i]);
            headers.insert(make_pair(header.get_key(), header));
        }
    }

    return ParserRequest(metodo, resource, headers, version);
}
void ParserRequest::printRequest(ParserRequest &peticion)
{
    cout << this->method <<" "<< this->resource<<" "<<this->version  <<" "<< endl;
    string headerString;
    for (const pair<const string, HeaderClass> &header : this->headers)
    {
        headerString += header.second.serialize();
    }
    cout<<headerString<<endl;
};