#ifndef PARSER_RESPONSE_H
#define PARSER_RESPONSE_H
#include "ParserRequest.h"
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
    constexpr static int SERVICE_UNAVAILABLE = 503;
    ParserResponse(const string version)noexcept;
    ParserResponse(int responseCode, const string version, const map<string, string> &headers, const Body &body) noexcept;
    ~ParserResponse();
    void handleHeadReq(const map<string, string> &reqheaders);
    void handleGetReq();
    void handlePostReq();

    static ParserResponse deserializeResponse(ParserRequest &request);
    string serializeResponse();
    static ParserResponse handleMacroErrors(const string error); //Devuelve respuesta a errores de sintaxis en request
};

#endif