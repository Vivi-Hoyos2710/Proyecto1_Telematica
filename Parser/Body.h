#ifndef BODY_REQ
#define BODY_REQ
#include "../Librerias/StringLibraries.h"
using namespace std;
class Body
{
private:
    string contentType;
    string data;
    int file_fd;
    off_t offset;
    size_t count;
 

public:
    Body();
    Body(string &contentType, string &data);
    Body(string &contenType, int &file_fd, off_t &offset, ssize_t &count);


    ~Body();
    //getters
    const string& getData();
    const string& getDataType();
    const int& getFile_fd();
    //setters
    void setData(const string& newData);
    static Body clasificarType(string contentType, string stringBody);
};
#endif