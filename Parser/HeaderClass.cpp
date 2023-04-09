#include "HeaderClass.h"

using namespace std;

HeaderClass::HeaderClass(const string &key, const string &value) noexcept : key(key), value(value)
{
}

void HeaderClass::set_value(const string &value) noexcept
{
    this->value = value;
}

const string& HeaderClass::get_key() const noexcept
{
    return this->key;
}
const string& HeaderClass::get_value() const noexcept{
    return this->value;
}

string HeaderClass::serialize() const noexcept
{
    string header;
    header += this->key;
    header += ": ";
    header += this->value;
    header += LINE_END;

    return header;
}

HeaderClass HeaderClass::deserialize(const string &header)
{
    
    vector<string> segments = split(header," ");

    const string key = segments[0].substr(0, segments[0].size() - 1);

    segments.erase(segments.begin());

    const string value = concat(segments,"");

    return HeaderClass(key, value);
}
