#pragma once

namespace owdin {
    //@abi table accounts i64
    struct account {
        eosio::asset balance;

        uint64_t primary_key()const { return balance.symbol.name(); }
    };
}