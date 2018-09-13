#pragma once

#include "models/config_index/system_index.hpp"

using namespace std;
using namespace eosio;

namespace owdin {
    class owdin_system {
        private:
            account_name self;

        public:
            owdin_system( account_name _self ): self( _self ) { };

            void set( account_name account, string playbook, string memo ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();

                sysconfIndex owdin_config( self, account );

                auto itr = owdin_config.find( account );
                eosio_assert( itr != owdin_config.end(), "can not found account" );

                owdin_config.modify( itr, self, [&]( auto& s ) {
                    s.receiver = account;
                    s.playbook = playbook;
                    s.memo = memo;
                    s.updated = current_time;
                    s.update_block = block_num;
                });
            }

            void clear( account_name account, string memo ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();

                sysconfIndex owdin_config( self, account );

                auto itr = owdin_config.find( account );
                eosio_assert( itr != owdin_config.end(), "can not found account" );

                owdin_config.modify( itr, self, [&]( auto& s ) {
                    s.receiver = self;
                    s.status = "";
                    s.playbook = "";
                    s.memo = "";
                    s.updated = current_time;
                    s.update_block = block_num;
                });
            }

            void remove( account_name account, string memo ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();

                sysconfIndex owdin_config( self, account );
                
                auto itr = owdin_config.find( account );
                eosio_assert( itr != owdin_config.end(), "can not found account" );

                owdin_config.erase( itr );
            }

            void initial( account_name account, string memo ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();

                sysconfIndex owdin_config( self, account );
                
                owdin_config.emplace( self, [&]( auto& s ) {
                    s.account = account;
                    s.receiver = self;
                    s.status = "";
                    s.playbook = "";
                    s.memo = "INIT_SYSTEM_CONFIG";
                    s.created = current_time;
                    s.updated = current_time;
                    s.create_block = block_num;
                    s.update_block = block_num;
                });
            }

            void update( account_name account, string stat, string memo ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();

                sysconfIndex owdin_config( self, account );

                auto itr = owdin_config.find( account );
                eosio_assert( itr != owdin_config.end(), "can not found account" );

                owdin_config.modify( itr, self, [&]( auto& s ) {
                    s.receiver = self;
                    s.status = stat;
                    s.memo = memo;
                    s.updated = current_time;
                    s.update_block = block_num;
                });
            }
    };
}