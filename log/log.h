#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
class log
{
private:
    std::string path;
public:
    log();
    ~log();
    static char *getCurrentTime();
    void createLog(std::string &logName);
    void appendToLog(std::string &info);
};


