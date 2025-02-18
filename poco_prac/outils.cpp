#include "inc/outils.hpp"

namespace odds {
    bool startswith(const std::string &s, const std::string &what) {
        return s.find(what) == 0;
    }
}