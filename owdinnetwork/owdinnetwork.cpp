#include "owdinnetwork.hpp"

namespace owdin {
    void owdinnetwork::debug( account_name account ) {
        /*
         * test action
         */
        const char* ver_info = "v0.0.2/alpha";
        account_name acc_1 = N("violetstairn");
        print( "[ ", ver_info, " - ", eosio::name{_self}, " ] : ", account );
    }

}

EOSIO_ABI( owdin::owdinnetwork, (debug) )
