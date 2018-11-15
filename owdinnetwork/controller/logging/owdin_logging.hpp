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

            int64_t logging( account_name account, uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused, uint16_t statuscode, string status, string message ) {
                time current_time = now();
                int64_t balance;

                usagedIndex uix( self, account );
                auto itr = uix.find( account );
                if (itr != uix.end()) {
                    uix.modify( itr, self, [&]( auto& s ) {
                        s.cpu = cpu;
                        s.memory = memory;
                        s.disk = disk;
                        s.bandwidth = bandwidth;
                        s.fsused = fsused;
                        s.statuscode = statuscode;
                        s.status = status;
                        s.message = message;
                        s.updated = current_time;
                    });
                    
                    balance = reward_balance( cpu,
                                        memory,
                                        disk - itr->disk,
                                        bandwidth - itr->bandwidth,
                                        fsused - itr->fsused );
                } else {
                    uix.emplace( self, [&]( auto& s ) {
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
                    });
                    balance = reward_balance( cpu,
                                    memory,
                                    disk,
                                    bandwidth,
                                    fsused );
                }

                logsIndex lix( self, account );
                lix.emplace( self, [&]( auto& s ) {
                    s.key = lix.available_primary_key();
                    s.account = account;
                    s.cpu = cpu;
                    s.memory = memory;
                    s.disk = disk;
                    s.bandwidth = bandwidth;
                    s.fsused = fsused;
                    s.statuscode = statuscode;
                    s.status = status;
                    s.message = message;
                    s.balance = balance;
                    s.created = current_time;
                });

                return balance;
            }

            int64_t reward_balance( uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused ) {
                const int64_t cpu_factor = 1;
                const int64_t memory_factor = 1;
                const int64_t disk_factor = 1;
                const int64_t bandwidth_factor = 1;
                const int64_t fsused_factor = 1;
                int64_t val = 0;

                cpu = _factor(cpu, cpu_factor);
                memory = _factor(memory, memory_factor);
                disk = _factor(disk, disk_factor);
                bandwidth = _factor(bandwidth, bandwidth_factor);
                fsused = _factor(fsused, fsused_factor);

                return (cpu + memory + disk + bandwidth + fsused);
            }

        private:
            int64_t _factor(uint64_t val, int64_t fac) {
                val = val * fac;
                if (val < 1) {
                    return 0;
                }
                return val;
            }
    };
}

