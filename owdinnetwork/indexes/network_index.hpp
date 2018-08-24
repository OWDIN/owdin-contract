#pragma once

using namespace std;
using namespace eosio;

//@abi table network i64
struct network {
    account_name account;
    account_name checker;
    uint64_t     status;
    string       data;
    string       memo;
    time         created;
    time         updated;

    uint64_t primary_key() const { return account; }
    
    EOSLIB_SERIALIZE(network, (account)(checker)(status)(data)(created)(updated))
};

typedef multi_index<N(network), network> networkIndex;