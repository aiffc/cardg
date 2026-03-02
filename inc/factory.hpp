#pragma once

#include <memory>
namespace cg {
class CommonCard;
class WarriorCard;
class TankCard;
class MageCard;
class PriestCard;
class ArcherCard;
class AssassinCard;
class RangerCard;
class HunterCard;
class AlchemyCard;
/* ---------------------------------------------------------------  */
enum class CommonCardTable {
    Blank,
};
std::unique_ptr<CommonCard> makeCommonBlank();
/* ---------------------------------------------------------------  */
enum class WarriorCardTable {
    Slash,
};
std::unique_ptr<WarriorCard> makeWarriorSlash();
/* ---------------------------------------------------------------  */
enum class TankCardTable {
    RaiseTheShield,
};
std::unique_ptr<TankCard> makeTankRaiseTheShield();
/* ---------------------------------------------------------------  */
enum class MegeCardTable {
    FireBall,
};
std::unique_ptr<MageCard> makeMegeFireBall();
/* ---------------------------------------------------------------  */
enum class PriestCardTable {
    Cure,
};
std::unique_ptr<PriestCard> makePriestCure();
/* ---------------------------------------------------------------  */
enum class ArcherCardTable {
    RoughShoot,
};
std::unique_ptr<ArcherCard> makeArcherRoughShoot();
/* ---------------------------------------------------------------  */
enum class AssassinCardTable {
    Stabbed,
};
std::unique_ptr<AssassinCard> makeAssassinStabbed();
/* ---------------------------------------------------------------  */
enum class RangerCardTable {
    RapidFire,
};
std::unique_ptr<RangerCard> makeRangerRapidFire();
/* ---------------------------------------------------------------  */
enum class HunterCardTable {
    SummonDog,
};
std::unique_ptr<HunterCard> makeHunterSummonDog();
/* ---------------------------------------------------------------  */
enum class AlchemyCardTable {
    ThrowPoison,
};
std::unique_ptr<AlchemyCard> makeAlchemyThrowPoison();
} // namespace cg
