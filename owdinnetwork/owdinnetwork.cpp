#include "owdinnetwork.hpp"

namespace owdin {
    void owdinnetwork::debug( account_name account ) {
        /*
         * test action
         */
        const char* ver_info = "v0.0.1";
        print( "[ ", ver_info, " - ", name{_self}, " ] : ", name{account} );
    }

    void owdinnetwork::create( asset maximum_supply ) {
        /*
         * create token
         */
        require_auth( _self );
        require_recipient( _self );

        auto sym = maximum_supply.symbol;
        eosio_assert( sym.is_valid(), "invalid symbol name" );
        eosio_assert( maximum_supply.is_valid(), "invalid supply");
        eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

        stats statstable( _self, sym.name() );
        auto existing = statstable.find( sym.name() );
        eosio_assert( existing == statstable.end(), "token with symbol already exists" );

        statstable.emplace( _self, [&]( auto& s ) {
            s.supply.symbol   = maximum_supply.symbol;
            s.max_supply      = maximum_supply;
            s.issuer          = _self;
        });
    }

    void owdinnetwork::issue( asset quantity, string memo ) {
        /*
         * issue token
         */
        require_auth( _self );
        require_recipient( _self );

        auto sym = quantity.symbol;
        eosio_assert( sym.is_valid(), "invalid symbol name" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        auto sym_name = sym.name();
        stats statstable( _self, sym_name );
        auto existing = statstable.find( sym_name );
        eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
        const auto& st = *existing;

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must issue positive quantity" );

        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

        statstable.modify( st, 0, [&]( auto& s ) {
            s.supply += quantity;
        });

        add_balance( st.issuer, quantity, st.issuer );
    }

    void owdinnetwork::transfer( account_name from, account_name to, asset quantity, string memo ) {
        /*
         * transfer token
         */
        eosio_assert( from != to, "cannot transfer to self" );
        require_auth( from );

        eosio_assert( is_account( to ), "to account does not exist");
        auto sym = quantity.symbol.name();
        stats statstable( _self, sym );
        const auto& st = statstable.get( sym );

        require_recipient( from );
        require_recipient( to );

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        sub_balance( from, quantity );
        add_balance( to, quantity, from );
    }

    void owdinnetwork::reward( account_name account, asset quantity, string memo ) {
        /*
         * reward token
         */
        eosio_assert( _self != account, "cannot reward to contract account" );
        
        require_auth( _self );

        eosio_assert( is_account( account ), "to account does not exist");
        auto sym = quantity.symbol.name();
        stats statstable( _self, sym );
        const auto& st = statstable.get( sym );

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        add_balance( account, quantity, account );
    }

    void owdinnetwork::burn( account_name account, asset quantity, string memo ) {
        /*
         * token burn
         */
        require_auth( _self );

        auto sym = quantity.symbol;
        eosio_assert( sym.is_valid(), "invalid symbol name" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        auto sym_name = sym.name();
        stats statstable( _self, sym_name );
        auto existing = statstable.find( sym_name );
        eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before burn" );
        const auto& st = *existing;

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount >= 0, "must burn positive or zero quantity" );

        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( quantity.amount <= st.supply.amount, "quantity exceeds available supply");

        statstable.modify( st, 0, [&]( auto& s ) {
            s.supply -= quantity;
        });

        sub_balance( account, quantity );
    }

    void owdinnetwork::sub_balance( account_name owner, asset value ) {
        accounts from_account( _self, owner );

        const auto& from = from_account.get( value.symbol.name(), "no balance object found" );
        eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );


        if( from.balance.amount == value.amount ) {
            from_account.erase( from );
        } else {
            from_account.modify( from, owner, [&]( auto& a ) {
                a.balance -= value;
            });
        }
    }

    void owdinnetwork::add_balance( account_name owner, asset value, account_name ram_payer ) {
        accounts to_account( _self, owner );
        auto to = to_account.find( value.symbol.name() );
        if( to == to_account.end() ) {
            to_account.emplace( ram_payer, [&]( auto& a ){
                a.balance = value;
            });
        } else {
            to_account.modify( to, 0, [&]( auto& a ) {
                a.balance += value;
            });
        }
    }

    asset owdinnetwork::get_supply( symbol_name sym ) const {
        stats statstable( _self, sym );
        const auto& st = statstable.get( sym );
        return st.supply;
    }

    asset owdinnetwork::get_balance( account_name owner, symbol_name sym ) const {
        accounts accountstable( _self, owner );
        const auto& ac = accountstable.get( sym );
        return ac.balance;
    }

}

EOSIO_ABI( owdin::owdinnetwork, (debug)(create)(issue)(transfer)(reward)(burn) )
