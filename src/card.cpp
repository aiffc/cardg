#include "../inc/card.hpp"
#include "../inc/player.hpp"
#include <string_view>

namespace cg {

Card::Card(std::string_view v) : m_name(v) {}
Card::~Card() = default;

CommonCard::CommonCard(std::string_view name) : Card(name) {}
CommonCard::~CommonCard() = default;

AttackCard::AttackCard(std::string_view name, int32_t value)
    : Card(name), m_value(value) {}
AttackCard::~AttackCard() = default;
void AttackCard::cast(Player *obj) {
    if (obj && !obj->isDead()) {
        int32_t armor = obj->armor();
        int32_t hurt = m_value;
        if (armor > 0) {
            hurt -= armor;
            obj->addArmor(-m_value);
        }
        if (hurt > 0) {
            obj->hurt(hurt);
        }
    }
}

MagicCard::MagicCard(std::string_view name, int32_t value)
    : Card(name), m_value(value) {}
MagicCard::~MagicCard() = default;
void MagicCard::cast(Player *obj) {
    if (obj && !obj->isDead()) {
        int32_t armor = obj->magicArmor();
        int32_t hurt = m_value;
        if (armor > 0) {
            hurt -= armor;
            obj->addMagicArmor(-m_value);
        }
        if (hurt > 0) {
            obj->hurt(hurt);
        }
    }
}

DefenseCard::DefenseCard(std::string_view name, int32_t value)
    : Card(name), m_value(value) {}
DefenseCard::~DefenseCard() = default;
void DefenseCard::cast(Player *obj) {
    if (obj && !obj->isDead()) {
        obj->addArmor(m_value);
    }
}

MagicDefenseCard::MagicDefenseCard(std::string_view name, int32_t value)
    : Card(name), m_value(value) {}
MagicDefenseCard::~MagicDefenseCard() = default;
void MagicDefenseCard::cast(Player *obj) {
    if (obj && !obj->isDead()) {
        obj->addMagicArmor(m_value);
    }
}

HealthCard::HealthCard(std::string_view name, int32_t value)
    : Card(name), m_value(value) {}
HealthCard::~HealthCard() = default;
void HealthCard::cast(Player *obj) {
    if (obj && !obj->isDead()) {
        obj->addHealth(m_value);
    }
}

BuffCard::BuffCard(std::string_view name, int32_t value)
    : Card(name), m_value(value) {}
BuffCard::~BuffCard() = default;
void BuffCard::cast(Player *obj [[maybe_unused]]) {}

DebuffCard::DebuffCard(std::string_view name, int32_t value)
    : Card(name), m_value(value) {}
DebuffCard::~DebuffCard() = default;
void DebuffCard::cast(Player *obj [[maybe_unused]]) {}

} // namespace cg
