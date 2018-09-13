#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include "models/account_index/account_index.hpp"
#include "models/economy_index/currency_index.hpp"

using namespace owdin;
using namespace eosio;
using namespace std;

namespace owdin {
    class owdinnetwork : public contract {
        private:
            using contract::contract;
            
        public:
            owdinnetwork( account_name self ) : contract( self ) { };

            //@abi action
            void debug( account_name account );

            //@abi action
            void create( asset maximum_supply );
            //@abi action
            void issue( asset quantity, string memo );
            //@abi action
            void transfer( account_name from, account_name to, asset quantity, string memo );
            //@abi action
            void reward( account_name account, asset quantity, string memo );
            //@abi action
            void burn( account_name account, asset quantity, string memo );

            inline asset get_supply( symbol_name sym )const;
            inline asset get_balance( account_name owner, symbol_name sym )const;

        private:
            typedef multi_index<N(accounts), account> accounts;
            typedef multi_index<N(stat), currency> stats;

            void auth( account_name account );
            void sub_balance( account_name owner, asset value );
            void add_balance( account_name owner, asset value, account_name ram_payer );
    };
}

