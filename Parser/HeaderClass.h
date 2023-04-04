#include "../Librerias/StringLibraries.h"
#include "Utility_Functions/utilityFunctions.h"
using namespace std;
class HeaderClass
{
    private:
        string key;
        string value;

    public:
        HeaderClass (const string& key, const string& value)noexcept;
        void set_value (const string& value) noexcept;

        const string& get_key() const noexcept;
        const string& get_value() const noexcept;
        string serialize() const noexcept;

        static HeaderClass deserialize(const string& header);
};