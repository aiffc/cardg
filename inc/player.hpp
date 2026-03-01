#pragma once

#include <memory>
#include <vector>

namespace cg {

class Card;

using CardHeap = std::vector<std::unique_ptr<Card>>;
using CardHand = std::vector<std::unique_ptr<Card>>;

class Player {
  private:
    CardHeap m_heap;
    CardHand m_hand;

  public:
    Player();
    ~Player();

    void draw();
    void disrupt();

    void addCard2Hand(std::unique_ptr<Card> &&card);
    void addCard2Heap(std::unique_ptr<Card> &&card);

    void showHand() const;
    void showHeap() const;

    Player(Player &) = delete;
    Player(Player &&) = delete;
    Player &operator&=(Player &) = delete;
    Player &operator&=(Player &&) = delete;
};

} // namespace cg
