#pragma once

#include "models/usage_index/usage_index.hpp"

using namespace std;
using namespace eosio;

namespace owdin {
    class owdin_logging {
        private:
            account_name self;

        public:
            owdin_logging( account_name _self ): self( _self ) { };

            void logging( account_name account, uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused, uint16_t statuscode, string status, string message ) {
                time current_time = now();
                uint64_t block_num = tapos_block_num();
                
                usagedIndex uix( self, account );

                uix.emplace( self, [&]( auto& s ) {
                    s.key = uix.available_primary_key();
                    s.account = account;
                    s.cpu = cpu;
                    s.memory = memory;
                    s.disk = disk;
                    s.bandwidth = bandwidth;
                    s.fsused = fsused;
                    s.statuscode = statuscode;
                    s.status = status;
                    s.message = message;
                    s.created = current_time;
                    s.updated = current_time;
                    s.create_block = block_num;
                    s.update_block = block_num;
                });
            }
    };
}
