#pragma once

#include "models/device_index/device_index.hpp"

using std::string;
using eosio::print;

namespace owdin {
    class owdin_device {
        private:
            account_name self;

        public:
            owdin_device( account_name _self ): self( _self ) { };

            void signup( account_name account, string pubkey, string uidx, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, uint8_t networktype, uint8_t usertype ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();

                deviceIndex owdin_device( self, account );

                owdin_device.emplace( self, [&]( auto& s ) {
                    s.account = account;
                    s.uidx = uidx;
                    s.pubkey = pubkey;
                    s.idx = idx;
                    s.bandwidth = bandwidth;
                    s.memory = memory;
                    s.disk = disk;
                    s.networktype = networktype;
                    s.usertype = usertype;
                    s.isactive = true;
                    s.created = current_time;
                    s.updated = current_time;
                    s.create_block = block_num;
                    s.update_block = block_num;
                });
            }

            void reset_account( account_name account, string pubkey, string uidx, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, uint8_t networktype, uint8_t usertype ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();
                
                deviceIndex owdin_device( self, account );

                auto itr = owdin_device.find( account );
                eosio_assert( itr != owdin_device.end(), "can not found account" );

                owdin_device.modify( itr, self, [&]( auto& s ) {
                    s.uidx = uidx;
                    s.pubkey = pubkey;
                    s.idx = idx;
                    s.bandwidth = bandwidth;
                    s.memory = memory;
                    s.disk = disk;
                    s.networktype = networktype;
                    s.usertype = usertype;
                    s.isactive = true;
                    s.updated = current_time;
                    s.update_block = block_num;
                });
            }

            void change_activate( account_name account, bool isactive ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();
                
                deviceIndex owdin_device( self, account );

                auto itr = owdin_device.find( account );
                eosio_assert( itr != owdin_device.end(), "can not found account" );

                owdin_device.modify( itr, self, [&]( auto& s ) {
                    s.isactive = isactive;
                    s.updated = current_time;
                    s.update_block = block_num;                    
                });
            }

    };
}