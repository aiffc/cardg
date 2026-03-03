#include "../inc/character.hpp"
#include "../inc/card.hpp"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <chrono>
#include <random>

namespace cg {

Character::Character() = default;
Character::~Character() = default;

void Character::draw() {
    if (m_heap.empty()) {
        if (m_recycle_bin.empty()) {
            spdlog::warn("no card in heap");
            return;
        } else {
            // if heap is empty and get card from recycle bin and disrupt
            m_heap.reserve(m_recycle_bin.size());
            m_heap.swap(m_recycle_bin);
            m_recycle_bin.clear();
            disrupt();
        }
    }
    auto head = m_heap.begin();
    m_hand.push_back(std::move(*head));
    m_heap.erase(head);
}

void Character::disrupt() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_heap.begin(), m_heap.end(),
                 std::default_random_engine(seed));
}

void Character::addCard2Hand(std::unique_ptr<Card> &&card) {
    if (card) {
        m_hand.push_back(std::move(card));
    }
}

void Character::addCard2Heap(std::unique_ptr<Card> &&card) {
    if (card) {
        m_heap.push_back(std::move(card));
    }
}

void Character::showHand() const {
    spdlog::info("hand:");
    for (const auto &card : m_hand) {
        spdlog::info("{}", card->name());
    }
    spdlog::info("\n");
}

void Character::showHeap() const {
    spdlog::info("heap:");
    for (const auto &card : m_heap) {
        spdlog::info("{}", card->name());
    }
    spdlog::info("\n");
}

void Character::showRecycleBin() const {
    spdlog::info("recycle bin:");
    for (const auto &card : m_recycle_bin) {
        spdlog::info("{}", card->name());
    }
    spdlog::info("\n");
}

void Character::useSelect(Character *cast_obj) {
    if (m_select_card_index < 0 ||
        (size_t)m_select_card_index >= m_hand.size()) {
        spdlog::warn("can not use select card {}", m_select_card_index);
        return;
    }

    auto it = m_hand.begin();
    it += m_select_card_index;
    (*it)->cast(cast_obj);
    m_recycle_bin.push_back(std::move(m_hand[m_select_card_index]));
    m_hand.erase(it);
    m_select_card_index = -1; // reset
}

void Character::useSelect(std::vector<Character *> &cast_obj) {
    if (m_select_card_index < 0 ||
        (size_t)m_select_card_index >= m_hand.size()) {
        return;
    }

    auto it = m_hand.begin();
    it += m_select_card_index;
    (*it)->cast(cast_obj);
    m_recycle_bin.push_back(std::move(m_hand[m_select_card_index]));
    m_hand.erase(it);
}
} // namespace cg
