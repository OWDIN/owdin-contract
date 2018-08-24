#pragma once

using namespace eosio;
using namespace std;

namespace owdin {
    class owdin_auth {
        private:
            const account_name codeaccount = N(owdinnetwork);
            
        public:
            void auth_account( account_name account, bool is_admin ) {
                if ( is_admin ) {
                    require_auth( codeaccount );
                } else {
                    require_auth( account );
                }
            }

            account_name get_owdin_admin() {
                return codeaccount;
            }
    };
}