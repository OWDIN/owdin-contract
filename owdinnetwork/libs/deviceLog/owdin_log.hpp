#pragma once

#include "utils/owdin_auth.hpp"

using namespace eosio;
using namespace std;

namespace owdin {
    class owdin_log : public owdin::owdin_auth {
        private:
            account_name _self = owdin_auth::get_owdin_admin();
        public:
            void errlog( account_name account, string data, uint64_t status, uint64_t type ) {
                owdin_auth::auth_account( account, false );
                
                print("errlog");
            }
    };
}