#pragma once

using namespace std;
using namespace eosio;

//@abi table statuslog i64
struct statuslog {
    uint64_t     key;
    account_name account;
    uint64_t     cpu;
    uint64_t     memory;
    uint64_t     disk;
    uint64_t     bandwidth;
    uint64_t     fsused;
    uint16_t     statuscode;
    string       status;
    string       message;
    time         created;

    uint64_t primary_key() const { return account; }

    EOSLIB_SERIALIZE(statuslog, (key)(account)(cpu)(memory)(disk)(bandwidth)(fsused)(statuscode)(status)(message)(created))
};

typedef multi_index<N(statuslog), statuslog> statuslogIndex;
