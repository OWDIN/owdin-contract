#pragma once

using namespace std;

//@abi table usaged i64
struct usaged {
    uint64_t     key;
    account_name account;      // device own user
    uint64_t     cpu;          // cpu usage
    uint64_t     memory;       // memory usage
    uint64_t     disk;         // disk usage
    uint64_t     bandwidth;    // network bandwidth usage
    uint64_t     fsused;       // file system usage
    uint16_t     statuscode;   // device status code
    string       status;       // device status
    string       message;      // transaction memo
    time         created;      // create time
    time         updated;      // update time
    uint64_t     create_block; // create block number
    uint64_t     update_block; // update block number

    uint64_t primary_key() const { return key; }
    //uint64_t primary_key() const { return account; }

    EOSLIB_SERIALIZE( usaged, (key)(account)(cpu)(memory)(disk)(bandwidth)(fsused)(statuscode)(status)(message)(created)(updated)(create_block)(update_block) )
};

typedef eosio::multi_index<N(usaged), usaged> usagedIndex;
