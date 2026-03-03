#pragma once

#include "spdlog/spdlog.h"
#include <functional>
#include <string>
#include <string_view>

namespace cg {

class Character;

enum class CardType {
    Common,
    Warrior,
    Tank,
    Mage,
    Priest,
    Archer,
    Assassin,
    Ranger,
    Hunter,
    Alchemy,
};

enum class CardCastObj {
    Group,
    Single,
};

class Card {
  protected:
    CardType m_type;
    CardCastObj m_cast_type;
    std::string m_name;
    std::string m_doc{};
    std::function<void(std::vector<Character *> &cast_obj)> m_effect_group;
    std::function<void(Character *cast_obj)> m_effect_single;

  public:
    Card(CardType type, CardCastObj cast_type, std::string_view v);
    virtual ~Card();

    const std::string &name() const { return m_name; }
    void name(std::string_view v) { m_name = std::string(v); }

    void effect(
        const std::function<void(std::vector<Character *> &cast_obj)> &effect) {
        m_effect_group = effect;
    }

    void effect(const std::function<void(Character *cast_obj)> &effect) {
        m_effect_single = effect;
    }

    void cast(std::vector<Character *> &obj) {
        if (m_effect_group) {
            m_effect_group(obj);
        } else {
            spdlog::warn("group effect function not defined");
        }
    }

    void cast(Character *obj) {
        if (m_effect_single && obj) {
            m_effect_single(obj);
        } else {
            spdlog::warn("effect function not defined or obj is null");
        }
    }
};
/* ---------------------------------------------------------------------------------------
 */
class CommonCard : public Card {
  public:
    CommonCard(CardCastObj cast_type, std::string_view name);
    ~CommonCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class WarriorCard : public Card {
  public:
    WarriorCard(CardCastObj cast_type, std::string_view name);
    ~WarriorCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class TankCard : public Card {
  public:
    TankCard(CardCastObj cast_type, std::string_view name);
    ~TankCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class PriestCard : public Card {
  public:
    PriestCard(CardCastObj cast_type, std::string_view name);
    ~PriestCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class MageCard : public Card {
  public:
    MageCard(CardCastObj cast_type, std::string_view name);
    ~MageCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class ArcherCard : public Card {
  public:
    ArcherCard(CardCastObj cast_type, std::string_view name);
    ~ArcherCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class AssassinCard : public Card {
  public:
    AssassinCard(CardCastObj cast_type, std::string_view name);
    ~AssassinCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class RangerCard : public Card {
  public:
    RangerCard(CardCastObj cast_type, std::string_view name);
    ~RangerCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class HunterCard : public Card {
  public:
    HunterCard(CardCastObj cast_type, std::string_view name);
    ~HunterCard() override;
};
/* ---------------------------------------------------------------------------------------
 */
class AlchemyCard : public Card {
  public:
    AlchemyCard(CardCastObj cast_type, std::string_view name);
    ~AlchemyCard() override;
};

} // namespace cg
