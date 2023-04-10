
#ifndef PARSER_RESPONSE_H
#define PARSER_RESPONSE_H
#include "ParserRequest.h"
#include <filesystem>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
namespace fs = std::filesystem;
class ParserResponse
{
private:
    int responseCode;
    string version;
    map<string, string> headers;
    Body body;

public:
    constexpr static int OK = 200;
    constexpr static int CREATED = 201;
    constexpr static int ACCEPTED = 202;
    constexpr static int NO_CONTENT = 203;
    constexpr static int BAD_REQUEST = 400;
    constexpr static int FORBIDDEN = 403;
    constexpr static int NOT_FOUND = 404;
    constexpr static int REQUEST_TIMEOUT = 408;
    constexpr static int INTERNAL_SERVER_ERROR = 500;
    constexpr static int BAD_GATEWAY = 502;
    constexpr static int HTTP_VERSION_NOT_SUPPORTED = 505;
    constexpr static int SERVICE_UNAVAILABLE = 503;
    constexpr static int METHOD_NOT_ALLOWED= 405;
    ParserResponse(const string version)noexcept;
    ParserResponse(int responseCode, const string version, const map<string, string> &headers, const Body &body) noexcept;
    ~ParserResponse();
    void handleHeadReq(string path,const string& documentRootPath);
    void handleGetReq(string path,const string& documentRootPath);
    void handlePostReq(string path,const string& documentRootPath,Body bodyReq);

    Body getBody();


    static ParserResponse deserializeResponse(ParserRequest &request,const string& absPath);
    string serializeResponse();
    static ParserResponse handleMacroErrors(const string error); //Devuelve respuesta a errores de sintaxis en request
    static int verificarDir(string path,const string& absPath);
    static string extraerExtension(string path);
    static string extensionFromContent(string rutaxd);
    static int writeFile(const std::string& filename, const char* buffer, size_t bufferSize);
    
};

#endif