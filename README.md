# OWDIN-PORT EOS Smart Contract
[![OWDIN NETWORK](docs/img/banner-owdin.png "OWDIN NETWORK")](https://owdin.network/)

## About
* Share OWDIN-PORT resources such as storage, computing power, and network bandwidth
* Participates in the network as a portion of the virtual data center
* OWDIN-PORT's config data and status log are stored in the EOS blockchain

## Action
| action      | auth               | Description                 |
|:------------|:-------------------|:----------------------------|
| `create`    | `Contract Account` | Create OWDIN token          |
| `issue`     | `Contract Account` | Issue OWDIN token           |
| `transfer`  | `EOS Account`      | Transfer OWDIN token        |
| `burn`      | `Contract Account` | OWDIN token burn            |
| `signup`    | `EOS Account`      | Signup service              |
| `set`       | `Contract Account` | Configure setting on device |
| `check`     | `EOS Account`      | Update setting status       |
| `logging`   | `EOS Account`      | logging device status       |
| `reward`    | `Contract Account` | Reward Token                |
| `activate`  | `EOS Account`      | Activate/Deactivate user    |

## Example action using cleos
### create
```bash
cleos push action serviceaccount create '["1000000000.0000 TOKEN"]' -p serviceaccount@active
```

### issue
```bash
cleos push action serviceaccount issue '["1000000000.0000 TOKEN", "INITIAL TOKEN ISSUE"]' -p serviceaccount@active
```

### transfer
```bash
cleos push action serviceaccount transfer '["user_account_1", "user_account_2", "1000000.0000 TOKEN", "transfer Test"]' -p user_account_1@active
```

### burn
```bash
cleos push action serviceaccount burn '["serviceaccount", "100.0000 TOKEN", "TOKEN BURN Test"]' -p serviceaccount@active
```

### signup
```bash 
cleos push action serviceaccount signup '["account", "pubkey", "uidx", "idx", bandwidth, memory, cpu, disk, netype, usertype]' -p account@active
```

### set
```bash 
cleos push action serviceaccount set '["account", "playbook", "playhash"]' -p serviceaccount@active
```

### check
```bash 
cleos push action serviceaccount check '["account", "stat"]' -p account@active
```

### logging
```bash 
cleos push action serviceaccount logging '["account", cpu, memory, disk, bandwidth, fsused, statuscode, "status", "message"]' -p account@active
```

### reward
```bash 
cleos push action serviceaccount logging '["account", "balance", "memo"]' -p serviceaccount@active
```

### activate
```bash
cleos push action serviceaccount activate '["account", true]' -p serviceaccount@active
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
