#pragma once

#include <string>
#include <string_view>
namespace cg {

class Card {
    private:
        std::string m_name;

    public:
        Card(std::string_view v);
        ~Card();

        const std::string &name() const { return m_name; }
        void name(std::string_view v) { m_name = std::string(v); }

};

} // namespace cg
