#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace cg {

class Card;

using CardHeap = std::vector<std::unique_ptr<Card>>;
using CardHand = std::vector<std::unique_ptr<Card>>;
using CardRecycleBin = std::vector<std::unique_ptr<Card>>;

class Character {
  private:
    CardHeap m_heap;
    CardHand m_hand;
    CardRecycleBin m_recycle_bin;
    int32_t m_select_card_index = -1;
    int32_t m_health;
    int32_t m_armor;
    int32_t m_magic_armor;
    // TODO buff

  public:
    Character();
    ~Character();

    void draw();
    void disrupt();

    void addCard2Hand(std::unique_ptr<Card> &&card);
    void addCard2Heap(std::unique_ptr<Card> &&card);

    void showHand() const;
    void showHeap() const;
    void showRecycleBin() const;

    /* return true if player is dead */
    bool isDead() const { return m_health <= 0; }

    int32_t health() const { return m_health; }
    void health(int32_t v) { m_health = v; }
    void addHealth(int32_t v) { m_health += v; }
    void hurt(int32_t v) { m_health -= v; }

    int32_t armor() const { return m_armor; }
    void armor(int32_t v) { m_armor = v; }
    void addArmor(int32_t v) { m_armor += v; }

    int32_t magicArmor() const { return m_magic_armor; }
    void magicArmor(int32_t v) { m_magic_armor = v; }
    void addMagicArmor(int32_t v) { m_magic_armor += v; }

    int32_t select() const { return m_select_card_index; }
    void select(int32_t v) {
        if ((size_t)v >= m_hand.size() || v < 0) {
            m_select_card_index = -1;
        } else {
            m_select_card_index = v;
        }
    }

    void useSelect(Character *cast_obj);
    void useSelect(std::vector<Character *> &cast_obj);

    Character(Character &) = delete;
    Character(Character &&) = delete;
    Character &operator&=(Character &) = delete;
    Character &operator&=(Character &&) = delete;
};

} // namespace cg
