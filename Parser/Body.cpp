#include "Body.h"
Body::Body(){};
Body::Body(string &contentType,string &data):contentType(contentType), data(data)
{}
Body::Body(string &contentType,int &file_fd, off_t &offset, ssize_t &count):contentType(contentType), file_fd(file_fd), offset(offset),count(count)
{}
//getters
const string& Body::getData(){
    return this->data;
}
const string& Body::getDataType(){
    return this->contentType;
}

const int& Body::getFile_fd(){
    return this->file_fd;
}

const off_t& Body::getOffset(){
    return this->offset;
}

const ssize_t& Body::getCount(){
    return this->count;
}



//set
void Body::setData(const string& newData){
    this->data=newData;
}




Body::~Body()
{
}