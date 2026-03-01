#pragma once

#include <string>
#include <string_view>
namespace cg {

class Player;

enum class CardType {
    Common,
    Attack,
    Magic,
    Defense,
    MagicDefense,
    Health,
    Buff,
    Debuff,
};

class Card {
  private:
    CardType m_type = CardType::Common;
    std::string m_name;
    std::string m_doc{};

  public:
    Card(std::string_view v);
    virtual ~Card();

    const std::string &name() const { return m_name; }
    void name(std::string_view v) { m_name = std::string(v); }

    virtual void cast(Player *obj) = 0;
};
/* ---------------------------------------------------------------------------------------
 */
class CommonCard : public Card {
  public:
    CommonCard(std::string_view name);
    ~CommonCard() override;

    void cast(Player *obj) override;
};
/* ---------------------------------------------------------------------------------------
 */
class AttackCard : public Card {
  private:
    int32_t m_value;

  public:
    AttackCard(std::string_view name, int32_t value);
    ~AttackCard() override;

    void cast(Player *obj) override;
};
/* ---------------------------------------------------------------------------------------
 */
class MagicCard : public Card {
  private:
    int32_t m_value;

  public:
    MagicCard(std::string_view name, int32_t value);
    ~MagicCard() override;

    void cast(Player *obj) override;
};
/* ---------------------------------------------------------------------------------------
 */
class DefenseCard : public Card {
  private:
    int32_t m_value;

  public:
    DefenseCard(std::string_view name, int32_t value);
    ~DefenseCard() override;

    void cast(Player *obj) override;
};
/* ---------------------------------------------------------------------------------------
 */
class MagicDefenseCard : public Card {
  private:
    int32_t m_value;

  public:
    MagicDefenseCard(std::string_view name, int32_t value);
    ~MagicDefenseCard() override;

    void cast(Player *obj) override;
};
/* ---------------------------------------------------------------------------------------
 */
class HealthCard : public Card {
  private:
    int32_t m_value;

  public:
    HealthCard(std::string_view name, int32_t value);
    ~HealthCard() override;

    void cast(Player *obj) override;
};
/* ---------------------------------------------------------------------------------------
 */
class BuffCard : public Card {
  private:
    int32_t m_value;

  public:
    BuffCard(std::string_view name, int32_t value);
    ~BuffCard() override;

    void cast(Player *obj) override;
};
/* ---------------------------------------------------------------------------------------
 */
class DebuffCard : public Card {
  private:
    int32_t m_value;

  public:
    DebuffCard(std::string_view name, int32_t value);
    ~DebuffCard() override;

    void cast(Player *obj) override;
};

} // namespace cg
