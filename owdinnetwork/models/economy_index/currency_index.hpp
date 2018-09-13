#pragma once

namespace owdin {
    //@abi table currencyaaa i64
    struct currency {
        eosio::asset        supply;
        eosio::asset        max_supply;
        account_name issuer;

        uint64_t primary_key()const { return supply.symbol.name(); }
    };
}
