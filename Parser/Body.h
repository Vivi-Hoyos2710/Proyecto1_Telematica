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
    ssize_t count;
    char* buffer;
    int contentLength;
public:
    Body();
    Body(string &contentType, string &data);
    Body(string &contenType, int &file_fd, off_t &offset, ssize_t &count);
    Body(string &contentType, const char* buffer,int contentLength);

    ~Body();
    //getters
    const string& getData();
    const int& getLen();
    const string& getDataType();
    const int& getFile_fd();
    const off_t& getOffset();
    const ssize_t& getCount();
    const char* getBuffer() const;
    //setters
    void setData(const string& newData);
    
};
#endif