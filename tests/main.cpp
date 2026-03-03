#include "../inc/card.hpp"
#include "../inc/character.hpp"
#include "../inc/engine/core/app.hpp"
#include "../inc/factory.hpp"
#include "test_scene.hpp"
#include <SDL3/SDL_init.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <utility>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

std::unique_ptr<cg::engine::App> app;

SDL_AppResult SDL_AppInit(void **appstate [[maybe_unused]],
                          int argc [[maybe_unused]],
                          char **argv [[maybe_unused]]) {

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

    app = std::make_unique<cg::engine::App>();
    if (!app->init()) {
        return SDL_APP_FAILURE;
    }

    auto scene = std::make_unique<TestScene>("test");
    app->pushScene(std::move(scene));

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate [[maybe_unused]], SDL_Event *event) {
    if (!app->event(event)) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate [[maybe_unused]]) {
    app->update();
    app->render();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate [[maybe_unused]],
                 SDL_AppResult result [[maybe_unused]]) {
    app->deinit();
}
