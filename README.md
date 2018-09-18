# OWDIN-PORT EOS Smart Contract
---
[![OWDIN NETWORK](doc/owdinlogo.png "OWDIN NETWORK")](https://owdin.network/)

## about
---
 > the users can share their OWDIN-PORT resources such as storage, computing power, and network bandwidth
 > the user participates in the network as a portion of the virtual data center
 > OWDIN-PORT's config data and status log are stored in the EOS blockchain

## action
---
| action      | auth               | Description                        |
|:------------|:-------------------|:-----------------------------------|
| `create`    | `contract account` | create owdin token                 |
| `issue`     | `contract account` | issue owdin token                  |
| `reward`    | `contract account` | reward token to user               |
| `burn`      | `contract account` | owdin token burn                   |
| `set`       | `contract account` | set owdin port device              |
| `remove`    | `contract account` | remove user account data           |
| `initial`   | `contract account` | initialize user account data       |
| `clear`     | `contract account` | clear user account data            |
| `price`     | `contract account` | set resource price                 |
| `debug`     | `eos account`      | test action                        |
| `transfer`  | `eos account`      | transfer owdin token               |
| `signup`    | `eos account`      | signup owdinnetwork                |
| `reset`     | `eos account`      | reset user data                    |
| `activate`  | `eos account`      | activate/deactivate user account   |
| `update`    | `eos account`      | user data update                   |
| `logging`   | `eos account`      | device status logging              |
| `regpool`   | `eos account`      | register resource pool user device |
| `staking`   | `eos account`      | staking resource                   |
| `unstaking` | `eos account`      | unstaking resource                 |

## example action using cleos
---
### debug
```bash
cleos push action serviceaccount debug '["serviceaccount"]' -p user_account@active
```

### create
```bash
cleos push action serviceaccount create '["1000000000.0000 TOKEN"]' -p serviceaccount@active
```

### issue
```bash
cleos push action serviceaccount issue '["1000000000.0000 OWDDIN", "INITIAL TOKEN ISSUE"]' -p serviceaccount@active
```

### transfer
```bash
cleos push action serviceaccount transfer '["user_account_1", "user_account_2", "1000000.0000 TOKEN", "transfer Test"]' -p user_account_1@active
```

### reward
```bash
cleos push action serviceaccount reward '["user_account", "10.0000 ODN", "TOKEN Reward Test"]' -p serviceaccount@active
```

### burn
```bash
cleos push action serviceaccount burn '["serviceaccount", "100.0000 TOKEN", "TOKEN BURN Test"]' -p serviceaccount@active
```

### signup
```bash
cleos push action serviceaccount signup '["user_account", "user_account", "user_public_key_1", "device_unique_index_1", "secure_ip_index_1", 10240, 409600, 1024000, 0, 0 ]' -p user_account@active
```

### reset
```bash
cleos push action serviceaccount reset '["user_account", "user_public_key_reset_1", "device_unique_index_reset_1", "secure_ip_index_reset_1", 10241, 409601, 1024001, 0, 0 ]' -p user_account@active
```

### activate
```bash
cleos push action serviceaccount activate '["user_account", false]' -p user_account@active
```

### set
```bash
cleos push action serviceaccount set '["user_account", "config_data", 0, "Config Setting Test"]' -p serviceaccount@active
```

### remove
```bash
cleos push action serviceaccount remove '["user_account", 0, "Remove Test"]' -p serviceaccount@active
```

### initial
```bash
cleos push action serviceaccount initial '["user_account", 0, "Initial Test"]' -p serviceaccount@active
```

### clear
```bash
cleos push action serviceaccount clear '["user_account", 0, "Clear Test"]' -p serviceaccount@active
```

### update
```bash
cleos push action serviceaccount update '["user_account", 0, "Update Status", "Update Test"]' -p user_account@active
```

### logging
```bash
cleos push action serviceaccount logging '["user_account", 10, 1024, 10240, 1024, 5120, 200, "Logging Status", "Logging Test"]' -p user_account@active
```

### regpool
```bash
cleos push action serviceaccount regpool '["user_account", 0, 1024]' -p user_account@active
```

### staking
```bash
cleos push action serviceaccount staking '["user_account_1", "user_account_2", 0, "10.0000 TOKEN"]' -p user_account_1@active
```

### unstaking
```bash
cleos push action serviceaccount unstaking '["user_account_1", "user_account_2", 1, 0]' -p user_account_1@active
```

### price
```bash
cleos push action serviceaccount price '[0, "10.0000 TOKEN"]' -p serviceaccount@active
```

## License
---
[LICENSE](LICENSE)
