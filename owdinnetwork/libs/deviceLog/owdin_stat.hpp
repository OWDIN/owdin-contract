#pragma once

#include "utils/owdin_auth.hpp"

using namespace eosio;
using namespace std;

namespace owdin {
    class owdin_stat : public owdin::owdin_auth {
        private:
            account_name _self = owdin_auth::get_owdin_admin();
        public:
            void statlog( account_name account, uint64_t cpu, uint64_t memory, uint64_t disk, uint64_t bandwidth, uint64_t fsused, uint16_t statuscode, string status, string message ) {
                owdin_auth::auth_account( account, false );
                
                print("statlog");
            }
    };
}