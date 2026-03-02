#include "../inc/card.hpp"
#include "../inc/factory.hpp"
#include "../inc/player.hpp"
#include <memory>
#include <spdlog/spdlog.h>

int main(int argc [[maybe_unused]], char **argv [[maybe_unused]]) {
    auto player = std::make_unique<cg::Player>();

    player->addCard2Heap(cg::makeAlchemyThrowPoison());
    player->addCard2Heap(cg::makeArcherRoughShoot());
    player->addCard2Heap(cg::makeAssassinStabbed());
    player->addCard2Heap(cg::makeCommonBlank());
    player->addCard2Heap(cg::makeHunterSummonDog());
    player->addCard2Heap(cg::makePriestCure());

    player->showHeap();

    return 0;
}
