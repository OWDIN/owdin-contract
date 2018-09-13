all: clean build deploy

wast:
	/usr/local/eosio/bin/eosiocpp -o owdinnetwork/owdinnetwork.wast owdinnetwork/owdinnetwork.cpp  > log/wast_stdout.txt 2> log/wast_stderr.txt

abi:
	/usr/local/eosio/bin/eosiocpp -g owdinnetwork/owdinnetwork.abi owdinnetwork/owdinnetwork.cpp  > log/abi_stdout.txt 2> log/abi_stderr.txt

build: wast abi

cl:
	rm -f log/*

cw:
	rm -f owdinnetwork/owdinnetwork.wast
	rm -f owdinnetwork/owdinnetwork.wasm

ca:
	rm -f owdinnetwork/owdinnetwork.abi

clean: ca cl cw

deploy:
	cli set contract owdinnetwork owdinnetwork/ 

.PHONY: clean build
