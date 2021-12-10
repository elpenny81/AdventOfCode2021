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

    template <typename T, typename U>
    static bool contains(const T& cont, U elem) {
        return std::find(cont.begin(), cont.end(), elem) != cont.end();
    }

    template<typename T, typename E>
    static int64_t index_of(const T& cont, const E& elem) {
        T::const_iterator it;
        if ((it = std::find(cont.begin(), cont.end(), elem)) != cont.end())
            return std::distance(cont.begin(), it);
        return -1;
    }
}