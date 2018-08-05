MODE ?= Debug

all: install

install:
	npm install

test: install
	npm test

run: install
	cd bin/${MODE} && LD_LIBRARY_PATH=`pwd` ./example

clean:
	-@rm -Rf bin
	-@rm -Rf artifacts
	-@rm -Rf node_modules
	-@rm -Rf gcc
	-@rm -Rf x86_64-linux-gnu-gcc
	-@rm -Rf x86_64-w64-mingw32.static-gcc
	-@rm artifacts.json

ci: clean run

docker-linux-x64:
	docker-compose run --rm cpp-linux-x64 make ci

docker-windows-x64:
	docker-compose run --rm cpp-windows-x64 make ci

docker-android-arm64:
	docker-compose run --rm cpp-android-arm64 make ci


