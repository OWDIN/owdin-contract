#include <eosiolib/eosio.hpp>

namespace owdin {
    class owdinnetwork : public eosio::contract {
        private:
            using contract::contract;

        public:
            owdinnetwork( account_name self ) : contract( self ) { };

            //@abi action
            void debug( account_name account );
    
    };
}

