#include "../inc/card.hpp"
#include "../inc/character.hpp"
#include <string_view>

namespace cg {

Card::Card(CardType type, CardCastObj cast_type, std::string_view v)
    : m_type(type), m_cast_type(cast_type), m_name(v) {}
Card::~Card() = default;

CommonCard::CommonCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Warrior, cast_type, name) {}
CommonCard::~CommonCard() = default;

WarriorCard::WarriorCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Warrior, cast_type, name) {}
WarriorCard::~WarriorCard() = default;

MageCard::MageCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Mage, cast_type, name) {}
MageCard::~MageCard() = default;

PriestCard::PriestCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Mage, cast_type, name) {}
PriestCard::~PriestCard() = default;

TankCard::TankCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Tank, cast_type, name) {}
TankCard::~TankCard() = default;

ArcherCard::ArcherCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Archer, cast_type, name) {}
ArcherCard::~ArcherCard() = default;

AssassinCard::AssassinCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Assassin, cast_type, name) {}
AssassinCard::~AssassinCard() = default;

RangerCard::RangerCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Ranger, cast_type, name) {}
RangerCard::~RangerCard() = default;

HunterCard::HunterCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Hunter, cast_type, name) {}
HunterCard::~HunterCard() = default;

AlchemyCard::AlchemyCard(CardCastObj cast_type, std::string_view name)
    : Card(CardType::Alchemy, cast_type, name) {}
AlchemyCard::~AlchemyCard() = default;

} // namespace cg
