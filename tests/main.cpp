#include "../inc/card.hpp"
#include "../inc/player.hpp"
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

int main(int argc [[maybe_unused]], char **argv [[maybe_unused]]) {
    auto player = std::make_unique<cg::Player>();

    for (uint32_t i = 0; i < 20; ++i) {
        player->addCard2Heap(std::make_unique<cg::Card>(std::to_string(i)));
    }
    spdlog::info("round 1");
    player->showHand();
    player->showHeap();
    spdlog::info("round 2");
    player->disrupt();
    player->showHeap();
    spdlog::info("round 3");
    player->draw();
    player->draw();
    player->draw();
    player->showHand();

    return 0;
}
