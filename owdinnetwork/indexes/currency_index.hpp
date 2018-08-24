#pragma once

#include <eosiolib/asset.hpp>

using namespace std;
using namespace eosio;

//@abi table currency i64
struct currency {
    eosio::asset supply;
    eosio::asset max_supply;
    account_name issuer;

    uint64_t primary_key()const { return supply.symbol.name(); }
};

typedef multi_index<N(currency), currency> currencyIndex;