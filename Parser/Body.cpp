#include "Body.h"
Body::Body(){};
Body::Body(string &contentType,string &data):contentType(contentType), data(data)
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