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
//set
void Body::setData(const string& newData){
    this->data=newData;
}


//funciones
Body Body::clasificarType(string contentType, string StringBody){
    if(contentType.compare("text/html")==0){
        cout<<"es texto/HTML"<<endl;
        return Body(contentType,StringBody);
    }
    else{
        std::cout << "No es HTML" << std::endl;
        return Body(contentType,StringBody);
    }
}

Body::~Body()
{
}