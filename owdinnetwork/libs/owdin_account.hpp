#pragma once

#include <eosiolib/eosio.hpp>

#include "utils/owdin_auth.hpp"

#include "indexes/account_index.hpp"
#include "indexes/currency_index.hpp"
#include "indexes/device_index.hpp"
#include "indexes/filesystem_index.hpp"
#include "indexes/network_index.hpp"
#include "indexes/process_index.hpp"
#include "indexes/system_index.hpp"

using namespace eosio;
using namespace std;

namespace owdin {
    class owdin_account : public owdin::owdin_auth {
        private:
            account_name _self = owdin_auth::get_owdin_admin();
        public:
            void add( account_name account, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, account_name receiver ) {
                owdin_auth::auth_account( account, false );

                deviceIndex owdin_device( _self, receiver );

                time current_time = now();
                owdin_device.emplace( _self, [&]( auto& s ) {
                    s.account = receiver;
                    s.pubkey = pubkey;
                    s.idx = idx;
                    s.bandwidth = bandwidth;
                    s.memory = memory;
                    s.disk = disk;
                    s.created = current_time;
                    s.updated = current_time;
                });

                add_fs( account, current_time );
                add_net( account, current_time );
                add_proc( account, current_time );
                add_sys( account, current_time );
            }

            void reset( account_name account, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, account_name receiver ) {
                owdin_auth::auth_account( account, false );

                deviceIndex owdin_device( _self, receiver );

                time current_time = now();

                auto itr = owdin_device.find( receiver );
                eosio_assert( itr != owdin_device.end(), "can not found account" );

                owdin_device.modify( itr, _self, [&]( auto& s ) {
                    s.account = receiver;
                    s.pubkey = pubkey;
                    s.idx = idx;
                    s.bandwidth = bandwidth;
                    s.memory = memory;
                    s.disk = disk;
                    s.updated = current_time;
                });
            }
        private:
            void add_fs( account_name account, time current_time ) {
                filesystemIndex owdin_fs( _self, account );

                owdin_fs.emplace( _self, [&]( auto& s ) {
                    s.account = account;
                    s.checker = _self;
                    s.status = 0;
                    s.data = "";
                    s.created = current_time;
                    s.updated = current_time;
                });
            }

            void add_net( account_name account, time current_time ) {
                networkIndex owdin_net( _self, account );

                owdin_net.emplace( _self, [&]( auto& s ) {
                    s.account = account;
                    s.checker = _self;
                    s.status = 0;
                    s.data = "";
                    s.created = current_time;
                    s.updated = current_time;
                });
            }

            void add_proc( account_name account, time current_time ) {
                processIndex owdin_proc( _self, account );

                owdin_proc.emplace( _self, [&]( auto& s ) {
                    s.account = account;
                    s.checker = _self;
                    s.status = 0;
                    s.data = "";
                    s.created = current_time;
                    s.updated = current_time;
                });
            }

            void add_sys( account_name account, time current_time ) {
                sysconfIndex owdin_sys( _self, account );

                owdin_sys.emplace( _self, [&]( auto& s ) {
                    s.account = account;
                    s.checker = _self;
                    s.status = 0;
                    s.data = "";
                    s.created = current_time;
                    s.updated = current_time;
                });
            }

    };
}