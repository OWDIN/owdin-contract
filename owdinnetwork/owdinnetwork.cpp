#include "owdinnetwork.hpp"

namespace owdin {
    ACTION owdinnetwork::debug( name account ) {
        const char* ver_info = "v0.1.1";
        print( "[ ", ver_info, " - ", name{_self}, " ] : ", name{account} );
    }

    ACTION owdinnetwork::create( asset maximum_supply ) {
        require_auth( _self );
        require_recipient( _self );

        auto symbol_name = maximum_supply.symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto existing_currency = currency_table.find( symbol_name );

        eosio_assert( existing_currency == currency_table.end(), "token with symbol already exists" );
        eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

        asset supply(0, symbol( symbol_code( symbol_name ), 4) );

        currency_table.emplace( _self, [&]( auto& currency ) {
            currency.supply = supply;
            currency.max_supply = maximum_supply;
            currency.issuer = _self;
        });
    }

    ACTION owdinnetwork::issue( asset quantity, string memo ) {
        auto symbol = quantity.symbol;
        eosio_assert( symbol.is_valid(), "invalid symbol name" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        auto symbol_name = symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto existing_currency = currency_table.find( symbol_name );
        eosio_assert( existing_currency != currency_table.end(), "token with symbol does not exist. create token before issue" );
        const auto& st = *existing_currency;

        require_auth( st.issuer );
        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must issue positive quantity" );
        eosio_assert( quantity.amount <= st.max_supply.amount, "quantity exceeds available supply" );
        eosio_assert( symbol == st.supply.symbol, "symbol precision mismatch" );

        add_supply( quantity );
        add_balance( _self, quantity, st.issuer );
    }

    ACTION owdinnetwork::transfer( name from, name to, asset quantity, string memo ) {
        eosio_assert( from != to, "cannot transfer to _self" );
        require_auth( from );

        eosio_assert( is_account( to ), "to account does not exist");
        auto sym = quantity.symbol.code().raw();
        currency_index currency_table( _self, sym );
        const auto& st = currency_table.get( sym );

        require_recipient( from );
        require_recipient( to );

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        sub_balance( from, quantity );
        add_balance( to, quantity, from );
    }

    ACTION owdinnetwork::burn( name account, asset quantity, string memo ) {
        require_auth( _self );

        auto sym = quantity.symbol;
        eosio_assert( sym.is_valid(), "invalid symbol name" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        auto sym_name = sym.code().raw();
        currency_index currency_table( _self, sym_name );
        auto existing = currency_table.find( sym_name );
        eosio_assert( existing != currency_table.end(), "token with symbol does not exist, create token before burn" );
        const auto& st = *existing;

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount >= 0, "must burn positive or zero quantity" );

        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( quantity.amount <= st.supply.amount, "quantity exceeds available supply");

        sub_supply( quantity );
        sub_balance( account, quantity );
    }

    void owdinnetwork::sub_balance( name owner, asset value ) {
        account_index accounts( _self, owner.value );
        const auto& from = accounts.get( value.symbol.code().raw(), "no balance object found" );
        eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );

        if( from.balance.amount == value.amount ) {
            accounts.erase( from );
        } else {
            accounts.modify( from, _self, [&]( auto& a ) {
                a.balance -= value;
            });
        }
    }

    void owdinnetwork::add_balance( name owner, asset value, name ram_payer ) {
        account_index accounts( _self, owner.value );
        auto to = accounts.find( value.symbol.code().raw() );
        if( to == accounts.end() ) {
            accounts.emplace( ram_payer, [&]( auto& a ){
                a.balance = value;
            });
        } else {
            accounts.modify( to, _self, [&]( auto& a ) {
                a.balance += value;
            });
        }
    }

    void owdinnetwork::sub_supply( asset quantity ) {
        auto symbol_name = quantity.symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto current_currency = currency_table.find( symbol_name );

        currency_table.modify( current_currency, _self, [&]( auto& currency ) {
            currency.supply -= quantity;
        });
    }

    void owdinnetwork::add_supply( asset quantity ) {
        auto symbol_name = quantity.symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto current_currency = currency_table.find( symbol_name );

        currency_table.modify( current_currency, _self, [&]( auto& currency ) {
            currency.supply += quantity;
        });
    }

    ACTION owdinnetwork::signup( name account, string pubkey, string uidx, string idx, string uuid ) {
        require_auth( account );

        uint64_t blocktime = publication_time();

        users_index user( _self, account.value);
        auto itr = user.find(account.value);
        if ( itr == user.end() ) {
            user.emplace( _self, [&]( auto& u ) {
                u.account = account;
                u.isactive = true;
                u.created = blocktime;
                u.updated = blocktime;
            });
        }

        user.modify( itr, _self, [&]( auto& u ) {
            u.account = account;
            u.isactive = true;
            u.created = blocktime;
            u.updated = blocktime;

            if (u.spec.size() < 1) {
                specific spec;
                spec.uidx = uidx;
                spec.pubkey = pubkey;
                spec.idx = idx;
                spec.uuid = uuid;
                spec.created = blocktime;
                spec.updated = blocktime;

                u.spec.push_back(spec);
            } else {
                u.spec[0].uidx = uidx;
                u.spec[0].pubkey = pubkey;
                u.spec[0].idx = idx;
                u.spec[0].uuid = uuid;
                u.spec[0].updated = blocktime;
            }
        });
    }

    ACTION owdinnetwork::set( name account, string playbook, string playhash ) {
        require_auth( _self );

        uint64_t blocktime = publication_time();

        users_index user( _self, account.value);
        auto itr = user.find(account.value);
        eosio_assert( itr != user.end(), "failed find user, signup first" );

        user.modify( itr, _self, [&]( auto& u ) {
            if (u.configs.size() < 1) {
                config cfg;
                cfg.receiver = account;
                cfg.conf = playbook;
                cfg.hash = playhash;
                cfg.updated = blocktime;

                u.configs.push_back(cfg);
            } else {
                u.configs[0].receiver = account;
                u.configs[0].conf = playbook;
                u.configs[0].hash = playhash;
                u.configs[0].updated = blocktime;
            }
        });
    }

    ACTION owdinnetwork::check( name account, string result ) {
        require_auth( account );

        uint64_t blocktime = publication_time();

        users_index user( _self, account.value);
        auto itr = user.find(account.value);
        eosio_assert( itr != user.end(), "failed find user, signup first" );

        user.modify( itr, _self, [&]( auto& u ) {
            if (u.configs.size() < 1) {
                config cfg;
                cfg.receiver = _self;
                cfg.conf = "";
                cfg.hash = "";
                cfg.result = result;
                cfg.updated = blocktime;

                u.configs.push_back(cfg);
            } else {
                u.configs[0].receiver = _self;
                u.configs[0].conf = "";
                u.configs[0].hash = "";
                u.configs[0].result = result;
                u.configs[0].updated = blocktime;
            }
        });
    }

    ACTION owdinnetwork::logging( name account, uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused, uint16_t statuscode, string status, string message ) {
        require_auth( account );

        uint64_t blocktime = publication_time();

        users_index user( _self, account.value);
        auto itr = user.find(account.value);
        eosio_assert( itr != user.end(), "failed find user, signup first" );

        uint64_t reward = reward_balance( cpu, memory, disk, bandwidth, fsused );

        user.modify( itr, _self, [&]( auto& u ) {
            if (u.usages.size() < 1) {
                usage use;
                use.cpu = cpu;
                use.memory = memory;
                use.disk = disk;
                use.bandwidth = bandwidth;
                use.fsused = fsused;
                use.statuscode = statuscode;
                use.status = status;
                use.updated = blocktime;

                u.usages.push_back(use);
            } else {
                u.usages[0].cpu = cpu;
                u.usages[0].memory = memory;
                u.usages[0].disk = disk;
                u.usages[0].bandwidth = bandwidth;
                u.usages[0].fsused = fsused;
                u.usages[0].statuscode = statuscode;
                u.usages[0].status = status;
                u.usages[0].updated = blocktime;
            }

            u.reward += reward;
        });
    }

     uint64_t owdinnetwork::reward_balance( uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused ) {
        const uint64_t cpu_factor = 1;
        const uint64_t memory_factor = 1;
        const uint64_t disk_factor = 1;
        const uint64_t bandwidth_factor = 1;
        const uint64_t fsused_factor = 1;
        uint64_t val = 0;

        cpu = factor(cpu, cpu_factor);
        memory = factor(memory, memory_factor);
        disk = factor(disk, disk_factor);
        bandwidth = factor(bandwidth, bandwidth_factor);
        fsused = factor(fsused, fsused_factor);

        return (cpu + memory + disk + bandwidth + fsused);
    }

    uint64_t owdinnetwork::factor(uint64_t val, uint64_t fac) {
        val = val * fac;
        if (val < 1) {
            return 0;
        }
        return val / 100;
    }

    ACTION owdinnetwork::reward( name account, asset balance, string memo ) {
        require_auth( _self );

        users_index user( _self, account.value);
        auto itr = user.find(account.value);
        eosio_assert( itr != user.end(), "failed find user, signup first" );

        uint64_t reward = itr->reward;

        auto symbol = balance.symbol;
        eosio_assert( symbol.is_valid(), "invalid symbol name" );

        auto symbol_name = symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto existing_currency = currency_table.find( symbol_name );
        eosio_assert( existing_currency != currency_table.end(), "token with symbol does not exist. create token before issue" );

        user.modify( itr, _self, [&]( auto& u ) {
            u.reward -= reward;
        });

        balance.amount = reward;
        add_supply( balance );
        add_balance( account, balance, account );
    }

    ACTION owdinnetwork::activate( name account, bool activate ) {
        require_auth( _self );

        users_index user( _self, account.value);
        auto itr = user.find(account.value);

        user.modify( itr, _self, [&]( auto& u ) {
            u.isactive = activate;
        });
    }

    ACTION owdinnetwork::addmon( name account, string name, string proc, uint64_t port, uint16_t key ) {
        require_auth( _self );
        uint64_t blocktime = publication_time();

        users_index user( _self, account.value);
        auto itr = user.find(account.value);

        user.modify( itr, _self, [&]( auto& u ) {
            if (key < 0) {
                monitor stat;
                stat.key = u.stat.size();
                stat.name = name;
                stat.proc = proc;
                stat.port = port;
                stat.updated = blocktime;

                u.stat.push_back(stat);
            } else {
                u.stat[key].name = name;
                u.stat[key].proc = proc;
                u.stat[key].port = port;
                u.stat[key].updated = blocktime;
            }
        });
    }

    ACTION owdinnetwork::removemon( name account, uint16_t key ) {
        require_auth( _self );

        users_index user( _self, account.value);
        auto itr = user.find(account.value);

        user.modify( itr, _self, [&]( auto& u ) {
            u.stat.erase(u.stat.begin() + key);
        });
    }

    ACTION owdinnetwork::status( name account, uint16_t key, bool status, string memo ) {
        require_auth( account );
        uint64_t blocktime = publication_time();

        users_index user( _self, account.value);
        auto itr = user.find(account.value);

        user.modify( itr, _self, [&]( auto& u ) {
            u.stat[key].stat = status;
            u.stat[key].updated = blocktime;
        });
    }
}

EOSIO_DISPATCH( owdin::owdinnetwork, (debug)(create)(issue)(transfer)(burn)(signup)(set)(check)(logging)(reward)(activate)(addmon)(removemon)(status) )
