#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>


#include "controller/config/owdin_filesystem.hpp"
#include "controller/config/owdin_network.hpp"
#include "controller/config/owdin_process.hpp"
#include "controller/config/owdin_system.hpp"
#include "controller/device/owdin_device.hpp"
#include "controller/logging/owdin_logging.hpp"

#include "models/account_index/account_index.hpp"
#include "models/economy_index/currency_index.hpp"

#include "define.hpp"

using namespace owdin;
using namespace eosio;
using namespace std;

namespace owdin {
    class owdinnetwork : public contract {
        private:
            using contract::contract;


            owdin_filesystem fs_controller;
            owdin_network    net_controller;
            owdin_process    proc_controller;
            owdin_system     sys_controller;
            owdin_logging    logging_controller;
            owdin_device     device_controller;
        public:
            owdinnetwork( account_name self ) : contract( self )
            , fs_controller( _self )
            , net_controller( _self )
            , proc_controller( _self )
            , sys_controller( _self )
            , logging_controller( _self )
            , device_controller( _self ) { };

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

        public:
            //@abi action
            void signup( account_name account, string pubkey, string uidx, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, uint8_t networktype, uint8_t usertype ) {
                require_auth( account );
                device_controller.signup( account, pubkey, uidx, idx, bandwidth, memory, disk, networktype, usertype );
            }
            //@abi action
            void reset( account_name account, string pubkey, string uidx, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, uint8_t networktype, uint8_t usertype ) {
                require_auth( account );
                device_controller.reset_account( account, pubkey, uidx, idx, bandwidth, memory, disk, networktype, usertype );
            }
            //@abi action
            void activate( account_name account, bool isactive ) {
                require_auth( account );
                device_controller.change_activate( account, isactive );
            }

            //@abi action
            void set( account_name account, string playbook, uint8_t object_type, string memo );
            //@abi action
            void remove( account_name account, uint8_t object_type, string memo );
            //@abi action
            void initial( account_name account, uint8_t object_type, string memo );
            //@abi action
            void clear( account_name account, uint8_t object_type, string memo );
            //@abi action
            void update( account_name account, uint8_t object_type, string stat, string memo );

            //@abi action
            void logging( account_name account, uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused, uint16_t statuscode, string status, string message );
    };
}

