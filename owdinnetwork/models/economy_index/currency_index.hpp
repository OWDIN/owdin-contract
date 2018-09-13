#pragma once

using namespace std;

namespace owdin {
    //@abi table currencyaaa i64
    struct currency {
        asset        supply;
        asset        max_supply;
        account_name issuer;

        uint64_t primary_key()const { return supply.symbol.name(); }
    };
}
