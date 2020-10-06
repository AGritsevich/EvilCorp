#pragma once
#ifndef evil_api_h__
#define evil_api_h__

using byte = unsigned char;
using func = void(*)(const byte*, const size_t);
using Value = size_t;
using Size = float;

static_assert(sizeof(Size) == 4, "size must equal 4");
static_assert(sizeof(Value) == 4, "value must equal 4");

namespace evil
{
    class api /* export external symbols: 'api' */
    {
    public:
        virtual void get_raw_rare(func callback) = 0;
        virtual void get_test_string(char* ptr /*366 symbols*/) = 0;
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
};

#endif// !evil_api_h__