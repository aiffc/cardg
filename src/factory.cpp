#include "../inc/factory.hpp"
#include "../inc/card.hpp"
#include "../inc/player.hpp"

namespace cg {
/* ---------------------------------------------------------------  */
std::unique_ptr<CommonCard> makeCommonBlank() {
    auto ret = std::make_unique<CommonCard>(CardCastObj::Single, "Blank");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(1);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<WarriorCard> makeWarriorSlash() {
    auto ret = std::make_unique<WarriorCard>(CardCastObj::Single, "Slash");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<TankCard> makeTankRaiseTheShield() {
    auto ret =
        std::make_unique<TankCard>(CardCastObj::Single, "Raise The Shield");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->addArmor(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<MageCard> makeMegeFireBall() {
    auto ret = std::make_unique<MageCard>(CardCastObj::Single, "Fire Ball");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<PriestCard> makePriestCure() {
    auto ret = std::make_unique<PriestCard>(CardCastObj::Single, "Cure");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<ArcherCard> makeArcherRoughShoot() {
    auto ret = std::make_unique<ArcherCard>(CardCastObj::Single, "Rough Shoot");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<AssassinCard> makeAssassinStabbed() {
    auto ret = std::make_unique<AssassinCard>(CardCastObj::Single, "Stabbed");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<RangerCard> makeRangerRapidFire() {
    auto ret = std::make_unique<RangerCard>(CardCastObj::Single, "Rapid Fire");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<HunterCard> makeHunterSummonDog() {
    auto ret = std::make_unique<HunterCard>(CardCastObj::Single, "Summon Dog");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}
/* ---------------------------------------------------------------  */
std::unique_ptr<AlchemyCard> makeAlchemyThrowPoison() {
    auto ret =
        std::make_unique<AlchemyCard>(CardCastObj::Single, "Throw Poison");
    ret->effect([](Player *cast_obj) {
        if (cast_obj && !cast_obj->isDead()) {
            cast_obj->hurt(10);
        }
    });
    return ret;
}

} // namespace cg
