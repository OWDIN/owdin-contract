#pragma once

using namespace std;
using namespace eosio;

//@abi table errorlog i64
struct errorlog {
    uint64_t     key;
    account_name account;
    uint16_t     statuscode;
    string       status;
    string       message;
    time         created;

    uint64_t primary_key() const { return account; }

    EOSLIB_SERIALIZE(errorlog, (key)(account)(statuscode)(status)(message)(created))
};

typedef multi_index<N(errorlog), errorlog> errorlogIndex;
