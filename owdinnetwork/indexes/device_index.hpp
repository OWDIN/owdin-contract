#pragma once

using namespace std;
using namespace eosio;

//@abi table device i64
struct device {
    account_name account;
    string       pubkey;
    string       idx;
    uint64_t     bandwidth;
    uint64_t     memory;
    uint64_t     disk;
    time         created;
    time         updated;

    uint64_t primary_key() const { return account; }

    EOSLIB_SERIALIZE(device, (account)(pubkey)(idx)(bandwidth)(memory)(disk)(created)(updated))
};

typedef multi_index<N(device), device> deviceIndex;