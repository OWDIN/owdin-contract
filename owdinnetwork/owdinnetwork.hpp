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
            // Token Action
            ACTION create( asset maximum_supply );
            ACTION issue( asset quantity, string memo );
            ACTION transfer( name from, name to, asset quantity, string memo );
            ACTION burn( name account, asset quantity, string memo );

            // OWDIN Action
            ACTION signup( name account, string pubkey, string uidx, int idx, string uuid );
            ACTION set( name account, string playbook, string playhash );
            ACTION check( name account, string result );
            ACTION addmon( name account, string name, string proc, uint64_t port, uint16_t key );
            ACTION removemon( name account, uint16_t key );
            ACTION status( name account, uint16_t key, bool status, string memo );
            ACTION logging( name account, uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused, uint16_t statuscode, string status, string message );
            ACTION reward( name account, asset balance, string memo );
            ACTION activate( name account, bool activate );

            // Structure
            struct specific {
                string   uidx;    // unique device index
                string   pubkey;  // user public key
                int      idx;     // unique user index
                string   uuid;    // device uuid
                uint64_t updated; // update time
                uint64_t created; // create time
            };

            struct config {
                name     receiver; // receiver
                string   conf;     // config data
                string   hash;     // config hash string
                string   result;   // work result
                uint64_t updated;  // update date
            };

            struct usage {
                uint64_t cpu;        // cpu usage
                uint64_t memory;     // memory usage
                uint64_t disk;       // disk usage
                uint64_t bandwidth;  // network bandwidth usage
                uint64_t fsused;     // file system usage
                uint16_t statuscode; // device status code
                string   status;     // device status
                uint64_t updated;    // updated time
            };

            struct monitor {
                uint16_t key;     // index
                string   name;    // status name
                string   proc;    // process name
                uint64_t port;    // process port number
                bool     stat;    // status
                uint64_t updated; // updated time
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
                name             account;  //
                uint64_t         reward;   // reward balance
                vector<specific> spec;     // device h/w spec
                vector<config>   configs;  //
                vector<usage>    usages;   //
                vector<monitor>  stat;     //
                vector<temp>     tmp1;     //
                vector<temp>     tmp2;     //
                bool             isactive; // activated user
                uint64_t         created;  //
                uint64_t         updated;  //

                uint64_t primary_key() const { return account.value; }
            };

            using account_index = multi_index<"accounts"_n, account>;
            using currency_index = multi_index<"stat"_n, currency_stats, indexed_by< "byissuer"_n, const_mem_fun< currency_stats, uint64_t, &currency_stats::get_issuer>>>;
            using users_index = multi_index<"users"_n, users>;

        private:
            void sub_balance(name owner, asset value);
            void add_balance(name owner, asset value, name ram_payer);
            void sub_supply(asset quantity);
            void add_supply(asset quantity);
            uint64_t reward_balance( uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused );
            uint64_t factor(uint64_t val, uint64_t fac);
    };
}
