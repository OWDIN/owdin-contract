#pragma once

using namespace std;
using namespace eosio;

//@abi table sysconf i64
struct sysconf {
    account_name account;
    account_name checker;
    uint64_t     status;
    string       data;
    string       memo;
    time         created;
    time         updated;

    uint64_t primary_key() const { return account; }

    EOSLIB_SERIALIZE(sysconf, (account)(checker)(status)(data)(created)(updated))
};

typedef multi_index<N(sysconf), sysconf> sysconfIndex;