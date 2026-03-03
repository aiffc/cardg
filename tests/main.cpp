#include "../inc/card.hpp"
#include "../inc/character.hpp"
#include "../inc/factory.hpp"
#include <memory>
#include <spdlog/spdlog.h>

int main(int argc [[maybe_unused]], char **argv [[maybe_unused]]) {
    auto player = std::make_unique<cg::Character>();
    auto monster = std::make_unique<cg::Character>();

    player->addCard2Heap(cg::makeAlchemyThrowPoison());
    player->addCard2Heap(cg::makeArcherRoughShoot());
    player->addCard2Heap(cg::makeAssassinStabbed());
    player->addCard2Heap(cg::makeCommonBlank());
    player->addCard2Heap(cg::makeHunterSummonDog());
    player->addCard2Heap(cg::makePriestCure());

    player->draw();
    player->draw();
    player->draw();
    player->draw();
    player->draw();
    player->draw();
    player->showHeap();
    player->showHand();
    player->showRecycleBin();
    spdlog::info("--------------------------------");
    player->select(1);
    player->useSelect(monster.get());
    player->select(1);
    player->useSelect(monster.get());
    player->select(1);
    player->useSelect(monster.get());
    player->showHeap();
    player->showHand();
    player->showRecycleBin();
    spdlog::info("--------------------------------");
    player->draw();
    player->draw();
    player->draw();
    player->draw();
    player->showHeap();
    player->showHand();
    player->showRecycleBin();

    return 0;
}
