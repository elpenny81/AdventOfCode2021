#include <string>
#include <vector>

namespace tools {

    static std::vector<std::string> string_split(const std::string& st, const std::string& delim)
    {
        std::vector<std::string> ret;
        std::string s = st;
        std::size_t pos = 0;

        while ((pos = s.find(delim)) != std::string::npos) {
            ret.push_back(s.substr(0, pos));
            s.erase(0, pos + delim.length());
        }

        ret.push_back(s);

        return ret;
    }
}