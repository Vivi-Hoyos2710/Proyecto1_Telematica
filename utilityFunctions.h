#ifndef UTILITY_FUNCTIONS
#define UTILITY_FUNCYIONS 


/// librerias necesarias
#include "StringLibraries.h"
#include <numeric> // for std::accumulate
//
using namespace std;
vector<string> split(const string& str,const string& delimitador) noexcept;
string concat(const std::vector<std::string>& strings, const std::string& delim) noexcept;
#endif // !utility