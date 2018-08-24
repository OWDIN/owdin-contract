#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

#include "indexes/account_index.hpp"
#include "indexes/currency_index.hpp"
#include "indexes/device_index.hpp"
#include "indexes/error_index.hpp"
#include "indexes/filesystem_index.hpp"
#include "indexes/network_index.hpp"
#include "indexes/status_index.hpp"
#include "indexes/system_index.hpp"

#include "libs/owdin_account.hpp"

#include "libs/deviceConfig/owdin_fs.hpp"
#include "libs/deviceConfig/owdin_sys.hpp"
#include "libs/deviceConfig/owdin_proc.hpp"
#include "libs/deviceConfig/owdin_net.hpp"

#include "libs/deviceLog/owdin_log.hpp"
#include "libs/deviceLog/owdin_stat.hpp"

#include "utils/owdin_auth.hpp"

using namespace eosio;
using namespace std;

namespace owdin {
    class owdinnetwork : public eosio::contract, public owdin::owdin_auth {
        private:
            
            account_name self;
            owdin_account account_controller;
            owdin_fs fs_controller;
            owdin_log log_controller;
            owdin_net net_controller;
            owdin_stat stat_controller;
            owdin_sys sys_controller;
            owdin_proc proc_controller;
        public:
            using contract::contract;
            owdinnetwork( account_name self ) : contract( self ) { }

            //@abi action
            void debug( account_name user );

            //@abi action
            void create( account_name account, asset maximum_supply );
            //@abi action
            void issue( account_name account, asset quantity, string memo );
            //@abi action
            void transfer( account_name account, account_name sender, account_name receiver, asset quantity, string memo );
            //@abi action
            void reward( account_name account, asset quantity, string memo );

            //@abi action
            void add( account_name account, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, account_name receiver ) {
                account_controller.add( account, pubkey, idx, bandwidth, memory, disk, receiver );
            }
            //@abi action
            void reset( account_name account, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, account_name receiver ) {
                account_controller.reset( account, pubkey, idx, bandwidth, memory, disk, receiver );
            }

            //@abi action
            void owdinport( account_name account, string config, uint64_t stat, uint64_t object_type, uint64_t action_type, account_name receiver, string memo ) {
                if ( object_type == 0 ) {
                    fs_controller.action( account, config, stat, action_type, receiver, memo );
                } else if ( object_type == 1 ) {
                    net_controller.action( account, config, stat, action_type, receiver, memo );
                } else if ( object_type == 2 ) {
                    sys_controller.action( account, config, stat, action_type, receiver, memo );
                } else if ( object_type == 3 ) {
                    proc_controller.action( account, config, stat, action_type, receiver, memo );
                } else {
                    eosio_assert( true, "object type error" );
                }
            }

            //@abi action
            void errlog( account_name account, string data, uint64_t status, uint64_t type ) {
                log_controller.errlog( account, data, status, type );
            }

            //@abi action
            void statlog( account_name account, uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused, uint16_t statuscode, string status, string message ) {
                stat_controller.statlog( account, cpu, memory, disk, bandwidth, fsused, statuscode, status, message );
            }
        private:
            void sub_balance( account_name owner, asset value, string memo );
            void add_balance( account_name owner, asset value, account_name ram_payer, string memo );
    };
}
