all: clean build

wast:
	/usr/local/eosio/bin/eosiocpp -o owdinnetwork/owdinnetwork.wast owdinnetwork/owdinnetwork.cpp

abi:
	/usr/local/eosio/bin/eosiocpp -g owdinnetwork/owdinnetwork.abi owdinnetwork/owdinnetwork.hpp

build:
	/usr/local/eosio/bin/eosiocpp -o owdinnetwork/owdinnetwork.wast owdinnetwork/owdinnetwork.cpp
	/usr/local/eosio/bin/eosiocpp -g owdinnetwork/owdinnetwork.abi owdinnetwork/owdinnetwork.hpp

clean:
	rm -f owdinnetwork/owdinnetwork.wast
	rm -f owdinnetwork/owdinnetwork.wasm
	rm -f owdinnetwork/owdinnetwork.abi

deploy:
	cli set contract owdinnetwork owdinnetwork/ 

.PHONY: wast abi clean build



