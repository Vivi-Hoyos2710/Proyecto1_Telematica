#ifndef UTILITY_FUNCTIONS
#define UTILITY_FUNCTIONS 


/// librerias necesarias
#include "../../Librerias/StringLibraries.h"
#include <numeric> // para metodo accumulate
//
using namespace std;
vector<string> split(const string& str,const string& delimitador) noexcept;
string concat(const std::vector<std::string>& strings, const std::string& delim) noexcept;
void replace(string& str,string &fromStr,const string& toStr);
#endif // !utility