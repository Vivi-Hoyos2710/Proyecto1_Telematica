#include "Body.h"
Body::Body(){};
Body::Body(string &contentType,string &data):contentType(contentType), data(data)
{

}

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