#pragma once

#include "indexes/currency_index.hpp"

#include "utils/owdin_auth.hpp"

using namespace eosio;
using namespace std;

namespace owdin {
    class owdin_economy : public owdin::owdin_auth {
        private:
            account_name _self = owdin_auth::get_owdin_admin();
        public:
            void create( account_name account, asset maximum_supply ) {
                owdin_auth::auth_account( account, true );

                auto sym = maximum_supply.symbol;
                eosio_assert( sym.is_valid(), "invalid symbol name" );
                eosio_assert( maximum_supply.is_valid(), "invalid supply");
                eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

                currencyIndex owdin_currency( _self, sym.name() );
                auto existing = owdin_currency.find( sym.name() );
                eosio_assert( existing == owdin_currency.end(), "token with symbol already exists" );

                owdin_currency.emplace( _self, [&]( auto& s ) {
                    s.supply.symbol = maximum_supply.symbol;
                    s.max_supply    = maximum_supply;
                    s.issuer        = account;
                });
            }

            void issue(account_name account, asset quantity, string memo) {
                owdin_auth::auth_account( account, true );

                auto sym = quantity.symbol;
                eosio_assert( sym.is_valid(), "invalid symbol name" );
                eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

                auto sym_name = sym.name();
                currencyIndex owdin_currency( _self, sym_name );
                auto existing = owdin_currency.find( sym_name );
                eosio_assert( existing != owdin_currency.end(), "token with symbol does not exist, create token before issue" );
                const auto& st = *existing;

                eosio_assert( quantity.is_valid(), "invalid quantity" );
                eosio_assert( quantity.amount > 0, "must issue positive quantity" );

                eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
                eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

                owdin_currency.modify( st, 0, [&]( auto& s ) {
                s.supply += quantity;
                });

                add_balance( st.issuer, quantity, st.issuer, "currency action issue");

                if( account != st.issuer ) {
                    SEND_INLINE_ACTION( *this, transfer, {st.issuer,N(active)}, {st.issuer, account, quantity, memo} );
                }
            }

            void transfer( account_name account, account_name sender, account_name receiver, asset quantity, string memo ) {
                owdin_auth::auth_account( account, false );

                eosio_assert( sender != receiver, "cannot transfer to self" );

                eosio_assert( is_account( receiver ), "to account does not exist");
                auto sym = quantity.symbol.name();
                currencyIndex owdin_currency( _self, sym );
                const auto& st = owdin_currency.get( sym );

                require_recipient( sender );
                require_recipient( receiver );

                eosio_assert( quantity.is_valid(), "invalid quantity" );
                eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
                eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
                eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

                sub_balance( sender, quantity, memo );
                add_balance( sender, quantity, account, memo );
            }

            void reward( account_name account, asset quantity, string memo ) {
                owdin_auth::auth_account( account, true );

                /*
                 *
                 */
                add_balance( account, quantity, _self, memo );
            }
        private:
            void sub_balance( account_name owner, asset value, string memo ) {
                accountIndex accounts( _self, owner );

                const auto& from = accounts.get( value.symbol.name(), "no balance object found" );
                eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );

                if( from.balance.amount == value.amount ) {
                    accounts.erase( from );
                } else {
                    accounts.modify( from, owner, [&]( auto& a ) {
                        a.balance -= value;
                    });
                }
            }

            void add_balance( account_name owner, asset value, account_name ram_payer, string memo ) {
                accountIndex accounts( _self, owner );

                auto to = accounts.find( value.symbol.name() );

                if( to == accounts.end() ) {
                    accounts.emplace( ram_payer, [&]( auto& a ){
                        a.balance = value;
                    });
                } else {
                    accounts.modify( to, 0, [&]( auto& a ) {
                        a.balance += value;
                    });
                }
            }
    };
}



