#pragma once

#include <string>
#include <string_view>
namespace cg {

class Player;

enum class CardType {
    Common,
    Attack,
    Magic,
    Defense,
    MagicDefense,
    Buff,
    Debuff,
};

class Card {
  private:
    CardType m_type = CardType::Common;
    std::string m_name;

  public:
    Card(std::string_view v);
    ~Card();

    const std::string &name() const { return m_name; }
    void name(std::string_view v) { m_name = std::string(v); }

    virtual void cast(Player *obj) = 0;
};

} // namespace cg
