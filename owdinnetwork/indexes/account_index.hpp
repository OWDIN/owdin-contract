#pragma once

#include <eosiolib/asset.hpp>

using namespace std;
using namespace eosio;

//@abi table account i64
struct account {
    eosio::asset balance;

    uint64_t primary_key()const { return balance.symbol.name(); }
};

typedef multi_index<N(accounts), account> accountIndex;