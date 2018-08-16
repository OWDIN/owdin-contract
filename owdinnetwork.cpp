#include "owdinnetwork.hpp"

namespace owdinnetwork {
    //@abi action
    void owdinports::version(const account_name account ) {
        require_auth( account );

        versioningIndex versioning( _self, _self );
        print("[OWDIN NETWORK] Contract Version : 0.1 / Debug : 1");
    }

    //@abi action
    void owdinports::launch( const account_name account, asset maximum_supply ) {
        /* check permition */
        require_auth( codeaccount );

        auto sym = maximum_supply.symbol;
        eosio_assert( sym.is_valid(), "invalid symbol name" );
        eosio_assert( maximum_supply.is_valid(), "invalid supply");
        eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

        statsIndex statstable( _self, sym.name() );
        auto existing = statstable.find( sym.name() );
        eosio_assert( existing == statstable.end(), "owdinports with symbol already exists" );

        statstable.emplace( _self, [&]( auto& s ) {
            s.supply.symbol = maximum_supply.symbol;
            s.max_supply    = maximum_supply;
            s.issuer        = account;
        });
    }

    //@abi action
    void owdinports::create( const account_name account, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, const account_name receiver ) {
        /* check permition */
        permission(account, receiver);

        owdinportIndex owdinport( _self, account );
        netconfigIndex netconfig( _self, account );
        fsconfigIndex fsconfig( _self, account );
        sysconfigIndex sysconfig( _self, account );

        time current_time = now();

        owdinport.emplace( _self, [&]( auto& s ) {
            s.account = account;
            s.pubkey = pubkey;
            s.idx = idx;
            s.bandwidth = bandwidth;
            s.memory = memory;
            s.disk = disk;
            s.created = current_time;
            s.updated = current_time;
        });

        netconfig.emplace( _self, [&]( auto& s ) {
            s.account = account;
            s.checker = codeaccount;
            s.status  = 0;
            s.data    = "";
            s.created = current_time;
            s.updated = current_time;
        });

        fsconfig.emplace( _self, [&]( auto& s ) {
            s.account = account;
            s.checker = codeaccount;
            s.status  = 0;
            s.data    = "";
            s.created = current_time;
            s.updated = current_time;
        });

        sysconfig.emplace( _self, [&]( auto& s ) {
            s.account = account;
            s.checker = codeaccount;
            s.status  = 0;
            s.data    = "";
            s.created = current_time;
            s.updated = current_time;
        });
    }

    //@abi action
    void owdinports::reset( const account_name account, string pubkey, string idx, uint64_t bandwidth, uint64_t memory, uint64_t disk, const account_name receiver ) {
        /* check permition */
        permission(account, receiver);

        owdinportIndex owdinport( _self, account );
        netconfigIndex netconfig( _self, account );
        fsconfigIndex fsconfig( _self, account );
        sysconfigIndex sysconfig( _self, account );

        time current_time = now();
        
        auto itr = owdinport.find(account);
        eosio_assert(itr != owdinport.end(), "owdinport for account not found");
        owdinport.modify(itr, account, [&](auto& s) {
            s.pubkey = pubkey;
            s.idx = idx;
            s.bandwidth = bandwidth;
            s.memory = memory;
            s.disk = disk;
            s.updated = current_time;
        });

        auto nitr = netconfig.find(account);
        eosio_assert(nitr != netconfig.end(), "network config for account not found");
        netconfig.modify(nitr, account, [&](auto& s) {
            s.checker = codeaccount;
            s.status  = 0;
            s.data    = "";
            s.updated = current_time;
        });

        auto fitr = fsconfig.find(account);
        eosio_assert(fitr != fsconfig.end(), "file system config for account not found");
        fsconfig.modify(fitr, account, [&](auto& s) {
            s.checker = codeaccount;
            s.status  = 0;
            s.data    = "";
            s.updated = current_time;
        });
        
        auto sitr = sysconfig.find(account);
        eosio_assert(sitr != sysconfig.end(), "system config for account not found");
        sysconfig.modify(sitr, account, [&](auto& s) {
            s.checker = codeaccount;
            s.status  = 0;
            s.data    = "";
            s.updated = current_time;
        });
    }

    //@abi action
    void owdinports::update( const account_name account, uint64_t type, string config ) {
        /* check permition */
        require_auth( codeaccount );
        
        time current_time = now();
        switch (type) {
            case 1:
                setnetconfig(account, config, current_time);
                break;
            case 2:
                setfsconfig(account, config, current_time);
                break;
            case 3:
                setsysconfig(account, config, current_time);
                break;
        }
    }

    //@abi action
    void owdinports::remove( const account_name account, uint64_t type ) {
        /* check permition */
        require_auth( codeaccount );

        switch (type) {
            case 1:
                {
                    owdinportIndex owdinport( _self, account );
                    auto itr = owdinport.find(account);
                    eosio_assert(itr != owdinport.end(), "owdinport for account not found");
                    owdinport.erase( itr );
                    eosio_assert(itr != owdinport.end(), "owdinport not erased properly");
                };
                break;
            case 2:
                {
                    netconfigIndex netconfig( _self, account );
                    auto nitr = netconfig.find(account);
                    eosio_assert(nitr != netconfig.end(), "network config for account not found");
                    netconfig.erase( nitr );
                    eosio_assert(nitr != netconfig.end(), "network config not erased properly");
                };
                break;
            case 3:
                {
                    fsconfigIndex fsconfig( _self, account );
                    auto fitr = fsconfig.find(account);
                    eosio_assert(fitr != fsconfig.end(), "file system config for account not found");
                    fsconfig.erase( fitr );
                    eosio_assert(fitr != fsconfig.end(), "file system config not erased properly");
                };
                break;
            case 4:
                {
                    sysconfigIndex sysconfig( _self, account );
                    auto sitr = sysconfig.find(account);
                    eosio_assert(sitr != sysconfig.end(), "system config for account not found");
                    sysconfig.erase( sitr );
                    eosio_assert(sitr != sysconfig.end(), "system config not erased properly");
                };
                break;
        }
    }

    //@abi action
    void owdinports::setstat( const account_name account, uint64_t type, uint64_t stat ) {
        /* check permition */
        require_auth( account );

        time current_time = now();
        switch (type) {
            case 1:
                {
                    netconfigIndex netconfig( _self, account );

                    auto itr = netconfig.find(account);
                    eosio_assert(itr != netconfig.end(), "network config for account not found");

                    netconfig.modify(itr, account, [&](auto& s) {
                        s.status  = stat;
                        s.checker = codeaccount;
                        s.updated = current_time;
                    });
                };
                break;
            case 2:
                {
                    fsconfigIndex fsconfig( _self, account );

                    auto itr = fsconfig.find(account);
                    eosio_assert(itr != fsconfig.end(), "network config for account not found");

                    fsconfig.modify(itr, account, [&](auto& s) {
                        s.status  = stat;
                        s.checker = codeaccount;
                        s.updated = current_time;
                    });
                };
                break;
            case 3:
                {
                    sysconfigIndex sysconfig( _self, account );

                    auto itr = sysconfig.find(account);
                    eosio_assert(itr != sysconfig.end(), "network config for account not found");

                    sysconfig.modify(itr, account, [&](auto& s) {
                        s.status  = stat;
                        s.checker = codeaccount;
                        s.updated = current_time;
                    });
                };
                break;
        }
    }

    //@abi action
    void owdinports::logging( const account_name account, string data, uint64_t status, uint64_t type ) {
        /* check permition */
        require_auth( account );
        
        switch (type) {
            case 1:
                loggingstatus( account, data, status, type );
            case 2:
                loggingerror( account, data, status, type );
        }
    }

    //@abi action
    void owdinports::transfer( account_name account, account_name receiver, asset quantity, string memo ) {
        /* check permition */
        require_auth( account );
        
        eosio_assert( account == receiver, "cannot transfer to self" );
        require_auth( account );
        eosio_assert( is_account( receiver ), "receiver account does not exist");
        auto sym = quantity.symbol.name();
        statsIndex statstable( _self, sym );
        const auto& st = statstable.get( sym );

        require_recipient( account );
        require_recipient( receiver );

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        sub_balance( account, quantity );
        add_balance( receiver, quantity, account );
    }
    
    //@abi action
    void owdinports::reward( account_name account, asset quantity, string memo ) {
        /* check permition */
        require_auth( codeaccount );
        
        auto sym = quantity.symbol;
        eosio_assert( sym.is_valid(), "invalid symbol name" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        auto sym_name = sym.name();
        statsIndex statstable( _self, sym_name );
        auto existing = statstable.find( sym_name );
        eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
        const auto& st = *existing;

        require_auth( st.issuer );
        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must issue positive quantity" );

        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

        statstable.modify( st, 0, [&]( auto& s ) {
        s.supply += quantity;
        });

        add_balance( st.issuer, quantity, st.issuer );

        if( account != st.issuer ) {
            SEND_INLINE_ACTION( *this, transfer, {st.issuer,N(active)}, {st.issuer, account, quantity, memo} )
        }
    }

    /* private function */

    void owdinports::sub_balance( account_name owner, asset value ) {
        accountIndex accounts( _self, owner );

        const auto& from = accounts.get( value.symbol.name(), "no balance object found" );
        eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );


        if( from.balance.amount == value.amount ) {
            accounts.erase( from );
        } else {
            accounts.modify( from, owner, [&]( auto& a ) {
                a.balance -= value;
            });
        }
    }

    void owdinports::add_balance( account_name owner, asset value, account_name ram_payer ) {
        accountIndex accounts( _self, owner );
        auto to = accounts.find( value.symbol.name() );
        if( to == accounts.end() ) {
            accounts.emplace( ram_payer, [&]( auto& a ){
                a.balance = value;
            });
        } else {
            accounts.modify( to, 0, [&]( auto& a ) {
                a.balance += value;
            });
        }
    }

    void owdinports::setnetconfig( const account_name account, string config, time current_time ) {
        netconfigIndex netconfig( _self, account );

        auto itr = netconfig.find(account);
        eosio_assert(itr != netconfig.end(), "network config for account not found");

        netconfig.modify(itr, account, [&](auto& s) {
            s.status  = 0;
            s.checker = account;
            s.data    = config;
            s.updated = current_time;
        });
    }
    
    void owdinports::setfsconfig( const account_name account, string config, time current_time ) {
        fsconfigIndex fsconfig( _self, account );
        

        auto itr = fsconfig.find(account);
        eosio_assert(itr != fsconfig.end(), "network config for account not found");

        fsconfig.modify(itr, account, [&](auto& s) {
            s.status  = 0;
            s.checker = account;
            s.data    = config;
            s.updated = current_time;
        });
    }
    
    void owdinports::setsysconfig( const account_name account, string config, time current_time ) {
        sysconfigIndex sysconfig( _self, account );

        auto itr = sysconfig.find(account);
        eosio_assert(itr != sysconfig.end(), "network config for account not found");

        sysconfig.modify(itr, account, [&](auto& s) {
            s.status  = 0;
            s.checker = account;
            s.data    = config;
            s.updated = current_time;
        });
    }
    
    void owdinports::loggingstatus( const account_name account, string data, uint64_t status, uint64_t type ) {
        logIndex logs( _self, account );

        uint64_t key;
        key = 1;
        
        time current_time = now();
        logs.emplace( _self, [&]( auto& s ) {
            s.key = key;
            s.account = account;
            s.statuscode = status;
            s.status = status;
            s.message = data;
            s.created = current_time;
        });
    }
    
    void owdinports::loggingerror( const account_name account, string data, uint64_t status, uint64_t type ) {
        errorIndex errors( _self, account );

        uint64_t key;
        key = 1;

        time current_time = now();
        errors.emplace( _self, [&]( auto& s ) {
            s.key = key;
            s.account = account;
            s.statuscode = status;
            s.status = status;
            s.message = data;
            s.created = current_time;
        });
    }

    void owdinports::permission( const account_name account, const account_name receiver ) {
        if ( receiver == codeaccount ) {
            require_auth( codeaccount );
        } else {
            require_auth( account );
        }
    }
} // namespace owdinnetwork

EOSIO_ABI(owdinnetwork::owdinports, (version)(launch)(create)(update)(setstat)(reset)(remove)(logging)(transfer)(reward))
