#ifndef BODY_REQ
#define BODY_REQ
#include "../Librerias/StringLibraries.h"
using namespace std;
class Body
{
private:
    string contentType;
    string data;
    vector<unsigned char> image;

public:
    Body();
    Body(string &contentType, string &data);
    ~Body();
    //getters
    const string& getData();
    const string& getDataType();
    //setters
    void setData(const string& newData);
    static Body clasificarType(string contentType, string stringBody);
};
#endif