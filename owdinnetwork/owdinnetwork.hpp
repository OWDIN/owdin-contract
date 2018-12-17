#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

namespace owdin {
    CONTRACT owdinnetwork : public contract {
        public:
            using contract::contract;
            owdinnetwork(name receiver, name code, datastream<const char *> ds)
                : contract(receiver, code, ds) { }

            // Debug Action
            ACTION debug( name account );

            // Token Action
            ACTION create( asset maximum_supply );
            ACTION issue( asset quantity, string memo );
            ACTION transfer( name from, name to, asset quantity, string memo );
            ACTION burn( name account, asset quantity, string memo );

            // OWDIN Action
            ACTION signup( name account, string pubkey, string uidx, string idx, uint128_t bandwidth, uint128_t memory, uint128_t cpu, uint128_t disk, uint128_t netype, uint8_t usertype );
            ACTION set( name account, string playbook, string playhash );
            ACTION check( name account, string stat );
            ACTION logging( name account, uint128_t cpu, uint128_t memory, uint128_t disk, uint128_t bandwidth, uint128_t fsused, uint16_t statuscode, string status, string message );
            ACTION reward( name account, asset balance, string memo );
            ACTION activate( name account, bool activate );

            // Structure
            struct specific {
                string    uidx;       // unique device number
                string    pubkey;     // user public key ( encrypt owdin public key )
                string    idx;        // unique user index ( encrypt public ip address )
                uint128_t bandwidth;  // network bandwidth ( user select data )
                uint128_t cpu;        // device cpu speed
                uint128_t memory;     // device memory size
                uint128_t disk;       // device disk size
                uint8_t   netype;     // device network type ( NAT or Public )
                uint8_t   usertype;   // user type ( public user or enterprise user )
                uint64_t  updated;    // update time
                uint64_t  created;    // create time
            };

            struct config {
                name     receiver;    // receiver
                string   conf;        // config data
                string   hash;        // config hash string
                string   status;      // work status
                uint64_t updated;     // update date
            };

            struct usage {
                uint128_t cpu;        // cpu usage
                uint128_t memory;     // memory usage
                uint128_t disk;       // disk usage
                uint128_t bandwidth;  // network bandwidth usage
                uint128_t fsused;     // file system usa`ge
                uint16_t  statuscode; // device status code
                string    status;     // device status
                uint64_t  updated;    // updated time
            };

            struct status {
                string   name;    // 
                uint64_t period;  // 
                string   msg;     // 
                uint64_t updated; // 
            };

            struct temp {
                string   name;
                uint64_t updated;
            };

            // Multi Index
            TABLE currency_stats {
                asset supply;
                asset max_supply;
                name  issuer;

                uint64_t primary_key() const { return supply.symbol.code().raw(); }
                uint64_t get_issuer() const { return issuer.value; }
            };

            TABLE account {
                asset balance;

                uint64_t primary_key() const { return balance.symbol.code().raw(); }
            };

            TABLE users {
                name             account;    // 
                uint128_t        reward;     // reward balance
                vector<specific> spec;       // device h/w spec
                vector<config>   configs;    // 
                vector<usage>    usages;     // 
                vector<status>   stat;       // 
                vector<temp>     tmp1;       // 
                vector<temp>     tmp2;       // 
                bool             isactive;   // activated user
                uint64_t         created;    // 
                uint64_t         updated;    // 

                uint64_t primary_key() const { return account.value; }
            };

            using account_index = eosio::multi_index<"accounts"_n, account>;
            using currency_index = multi_index<"stat"_n, currency_stats, indexed_by< "byissuer"_n, const_mem_fun< currency_stats, uint64_t, &currency_stats::get_issuer>>>;
            using users_index = multi_index<"users"_n, users>;

        private:
            void sub_balance(name owner, asset value);
            void add_balance(name owner, asset value, name ram_payer);
            void sub_supply(asset quantity);
            void add_supply(asset quantity);
            uint128_t reward_balance( uint128_t cpu, uint128_t memory, uint128_t disk, uint128_t bandwidth, uint128_t fsused );
            uint128_t factor(uint128_t val, uint128_t fac);
    };
}
