#include "log.h"
log::log(/* args */)
{
}

log::~log()
{
}
char *log::getCurrentTime()
{
    time_t now = time(0);

    // En string (array de char)
    char *dt = ctime(&now);
    return dt;
}

void log::createLog(std::string &logName)
{
    
    std::string archivo = "./log/"+ logName+".txt";
    this->path=archivo;
    std::fstream file(archivo, std::ios::in | std::ios::out);
    if (!file)
    {
        std::cout << "LogFile inexistente, creandolo..." << std::endl;
        file.close();
        file.open(archivo, std::ios::out);
        file.close();
    }
    else
    {
        std::cout << "LogFile ya existe." << std::endl;
    }
}

void log::appendToLog(std::string &info)
{
    std::ofstream file(this->path, std::ios::app);

    if (file.is_open())
    {
        // Write information to the file
        file << info+'\n';

        // Close the file
        file.close();
    }
    else
    {
        std::cout << "Error abriendo el archvo" << std::endl;
    }
}