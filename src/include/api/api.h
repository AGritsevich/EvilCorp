#pragma once

#ifdef API_EXPORTS  
#define EVIL_API __declspec(dllexport)   
#else  
#define EVIL_API __declspec(dllimport)   
#endif // API_EXPORTS

#pragma pack(push, 1)

using byte = unsigned char;
using func = void(*)(const byte*, const size_t);
using Value = float;
using Size = float;

static_assert(sizeof(Size) % 4 == 0, "size must be a multiple of 4");
static_assert(sizeof(Value) % 4 == 0, "value must be a multiple of 4");

namespace evil {
  class EVIL_API api {
  public:
    // WTF! Why it's working in my context? It's sync method
    virtual void get_raw_rare(func callback) =0;
    virtual void get_test_string(char* ptr /*366 symbols*/) =0;
    virtual ~api() {};
  };

  enum class RareType : byte {
    Jug,
    Helm,
    Coin,
    Sword,
  };

  struct Jug {
    Value cost;
    Size  R;
    Size  H;
  };

  struct Helm {
    Value cost;
    Size  size;
    Value armor;
  };

  struct Coin {
    Value cost;
  };

  struct Sword {
    Value cost;
    Value damage;
    Value height;
  };

  union Things {
    Jug jug;
    Helm helm;
    Coin coin;
    Sword sword;
  };

  struct Item {
    RareType type;
    Things thing;

    inline bool operator==(const Item& lhs) {
      if (this->type == lhs.type) {
        return false;
      }

      switch (lhs.type) {
      case RareType::Jug:
        if (this->thing.jug.cost != lhs.thing.jug.cost ||
          this->thing.jug.H != lhs.thing.jug.H ||
          this->thing.jug.R != lhs.thing.jug.R) {
          return false;
        }
        break;
      case RareType::Helm:
        if (this->thing.helm.cost != lhs.thing.helm.cost ||
          this->thing.helm.armor != lhs.thing.helm.armor ||
          this->thing.helm.size != lhs.thing.helm.size) {
          return false;
        }
        break;
      case RareType::Coin:
        if (this->thing.coin.cost != lhs.thing.coin.cost) {
          return false;
        }
        break;
      case RareType::Sword:
        if (this->thing.sword.cost != lhs.thing.sword.cost ||
          this->thing.sword.damage != lhs.thing.sword.damage ||
          this->thing.sword.height != lhs.thing.sword.height) {
          return false;
        }
        break;
      default:
        return false;
      }
    return true;
    }
  };
}; // evil

#pragma pack(pop)