all: clean build deploy

build:
	/usr/local/eosio/bin/eosiocpp -o owdinnetwork/owdinnetwork.wast owdinnetwork/owdinnetwork.cpp  > log/wast_stdout.txt 2> log/wast_stderr.txt
	/usr/local/eosio/bin/eosiocpp -g owdinnetwork/owdinnetwork.abi owdinnetwork/owdinnetwork.cpp  > log/abi_stdout.txt 2> log/abi_stderr.txt

clean:
	rm -f owdinnetwork/owdinnetwork.wast
	rm -f owdinnetwork/owdinnetwork.wasm
	rm -f owdinnetwork/owdinnetwork.abi
	rm -f log/*

deploy:
	cli set contract owdinnetwork owdinnetwork/ 

.PHONY: clean build
