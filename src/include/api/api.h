#pragma once

#ifdef API_EXPORTS  
#define EVIL_API __declspec(dllexport)   
#else  
#define EVIL_API __declspec(dllimport)   
#endif // API_EXPORTS

#pragma pack(push, 1)

using byte = unsigned char;
using func = void(*)(const byte*, const size_t);
using Value = size_t;
using Size = float;

static_assert(sizeof(Size) % 4 == 0, "size must be a multiple of 4");
static_assert(sizeof(Value) % 4 == 0, "value must be a multiple of 4");

namespace evil
{
    class EVIL_API api /* export external symbols: 'api' */
    {
    public:
        virtual void get_raw_rare(func callback) =0;
        virtual void get_test_string(char* ptr /*366 symbols*/) =0;
        virtual ~api() {};
    };

    enum class RareType : unsigned char
    {
        Jug,
        Helm,
        Coin,
        Sword,
    };

    struct Jug
    {
        Value cost;
        Size  R;
        Size  H;
    };

    struct Helm
    {
        Value cost;
        Size  size;
        Value armor;
    };

    struct Coin
    {
        Value cost;
    };

    struct Sword
    {
        Value cost;
        Value damage;
        Value height;
    };

    struct Item {
      RareType type;

      union {
        Jug jug;
        Helm helm;
        Coin coin;
        Sword sword;
      };
    };
};

#pragma pack(pop)