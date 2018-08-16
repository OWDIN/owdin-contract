#pragma once
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <string>

using std::string;
using namespace eosio;

namespace owdinnetwork {
    class owdinports : public contract {
        public:
            owdinports( account_name self ):contract(self){}
            static const account_name codeaccount = N(violetstairn);
            static const symbol_name syme = N(owdin);

            void version(const account_name account );
            void launch( const account_name account, asset maximum_supply );
            void create( const account_name user, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, const account_name receiver );
            void reset( const account_name user, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, const account_name receiver );
            void update( const account_name account, uint64_t type, string config );
            void setstat( const account_name account, uint64_t type, uint64_t stat );
            void remove( const account_name account, uint64_t type );
            void get( const account_name account );
            void logging( const account_name account, string data, uint64_t status, uint64_t type );
            void transfer( account_name user, account_name receiver, asset quantity, string memo );
            void reward( account_name user, asset quantity, string memo );

        private:
            void sub_balance( account_name owner, asset value );
            void add_balance( account_name owner, asset value, account_name ram_payer );
            void setnetconfig( const account_name user, string config, time current_time );
            void setfsconfig( const account_name user, string config, time current_time );
            void setsysconfig( const account_name user, string config, time current_time );
            void loggingstatus(const account_name account, string data, uint64_t status, uint64_t type );
            void loggingerror(const account_name account, string data, uint64_t status, uint64_t type );
            void permission( const account_name user, const account_name receiver );

            //@abi table owdinport i64
            struct owdinport {
                account_name account;
                string       pubkey;
                string       idx;
                uint64_t     bandwidth;
                uint64_t     memory;
                uint64_t     disk;
                time         created;
                time         updated;

                uint64_t primary_key() const { return account; }

                EOSLIB_SERIALIZE(owdinport, (account)(pubkey)(idx)(bandwidth)(memory)(disk)(created)(updated))
            };

            //@abi table netconfig i64
            struct netconfig {
                account_name account;
                account_name checker;
                uint64_t     status;
                string       data;
                time         created;
                time         updated;

                uint64_t primary_key() const { return account; }
                
                EOSLIB_SERIALIZE(netconfig, (account)(checker)(status)(data)(created)(updated))
            };

            //@abi table fsconfig i64
            struct fsconfig {
                account_name account;
                account_name checker;
                uint64_t     status;
                string       data;
                time         created;
                time         updated;

                uint64_t primary_key() const { return account; }

                EOSLIB_SERIALIZE(fsconfig, (account)(checker)(status)(data)(created)(updated))
            };

            //@abi table sysconfig i64
            struct sysconfig {
                account_name account;
                account_name checker;
                uint64_t     status;
                string       data;
                time         created;
                time         updated;

                uint64_t primary_key() const { return account; }

                EOSLIB_SERIALIZE(sysconfig, (account)(checker)(status)(data)(created)(updated))
            };

            //@abi table log i64
            struct logs {
                uint64_t     key;
                account_name account;
                //uint64_t   cpu;
                //uint64_t   memory;
                //uint64_t   disk;
                //uint64_t   bandwidth;
                //uint64_t   fsused;
                uint16_t     statuscode;
                string       status;
                string       message;
                time         created;

                uint64_t primary_key() const { return account; }

                EOSLIB_SERIALIZE(logs, (key)(account)(statuscode)(status)(message)(created))
            };

            //@abi table error i64
            struct errors {
                uint64_t     key;
                account_name account;
                uint16_t     statuscode;
                string       status;
                string       message;
                time         created;

                uint64_t primary_key() const { return account; }

                EOSLIB_SERIALIZE(errors, (key)(account)(statuscode)(status)(message)(created))
            };

            //@abi table currency i64
            struct currency {
                asset          supply;
                asset          max_supply;
                account_name   issuer;

                uint64_t primary_key()const { return supply.symbol.name(); }
            };

            //@abi table account i64
            struct account {
                asset balance;

                uint64_t primary_key()const { return balance.symbol.name(); }
            };

            //@abi table version i64
            struct versioning {
                uint64_t contract;
                uint64_t daemon;
                uint64_t server;
                uint64_t device;

                uint64_t primary_key()const { contract; }

                EOSLIB_SERIALIZE(versioning, (contract)(daemon)(server)(device))
            };

            typedef eosio::multi_index<N(versionings), versioning> versioningIndex;
            typedef eosio::multi_index<N(accounts), account> accountIndex;
            typedef eosio::multi_index<N(stat), currency> statsIndex;
            typedef eosio::multi_index<N(owdinport), owdinport> owdinportIndex;
            typedef eosio::multi_index<N(netconfig), netconfig> netconfigIndex;
            typedef eosio::multi_index<N(fsconfig), fsconfig> fsconfigIndex;
            typedef eosio::multi_index<N(sysconfig), sysconfig> sysconfigIndex;
            typedef eosio::multi_index<N(logs), logs> logIndex;
            typedef eosio::multi_index<N(errors), errors> errorIndex;
    }; // class token

} // namespace owdinnetwork