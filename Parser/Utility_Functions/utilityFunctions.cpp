#include "utilityFunctions.h"
vector<string> split(const string& s, const string& delimiter) noexcept {
    vector<string> lista_strings;
    size_t start_pos = 0, end_pos;

    while ((end_pos = s.find(delimiter, start_pos)) != string::npos) {
        lista_strings.push_back(s.substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + delimiter.size();
    }

    lista_strings.push_back(s.substr(start_pos));
    return lista_strings;
}
string concat(const vector<string> &strings, const string &delim) noexcept
{
    return accumulate(begin(strings), end(strings), string(),
                           [&](const string &a, const string &b)
                           {
                               return a + delim + b;
                           });
};
//  [&](const string &a, const string &b) Es una función lambda