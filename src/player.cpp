#include "../inc/player.hpp"
#include "../inc/card.hpp"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <chrono>
#include <random>

namespace cg {

Player::Player() = default;
Player::~Player() = default;

void Player::draw() {
    if (m_heap.empty()) {
        spdlog::warn("no card in heap");
        return;
    }
    auto head = m_heap.begin();
    m_hand.push_back(std::move(m_heap[0]));
    m_heap.erase(head);
}

void Player::disrupt() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_heap.begin(), m_heap.end(),
                 std::default_random_engine(seed));
}

void Player::addCard2Hand(std::unique_ptr<Card> &&card) {
    if (card) {
        m_hand.push_back(std::move(card));
    }
}

void Player::addCard2Heap(std::unique_ptr<Card> &&card) {
    if (card) {
        m_heap.push_back(std::move(card));
    }
}

void Player::showHand() const {
    for (const auto &card : m_hand) {
        spdlog::info("{}", card->name());
    }
}

void Player::showHeap() const {
    for (const auto &card : m_heap) {
        spdlog::info("{}", card->name());
    }
}
} // namespace cg
