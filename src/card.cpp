#include "../inc/card.hpp"
#include <string_view>

namespace cg {

Card::Card(std::string_view v) : m_name(v) {}
Card::~Card() = default;

} // namespace cg
