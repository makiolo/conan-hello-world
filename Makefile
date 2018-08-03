all: install

install:
	npm install

test: install
	npm test

run: install
	cd bin/ && LD_LIBRARY_PATH=`pwd` ./example

docker-linux-x64:
	docker-compose run --rm cpp-linux-x64 make run

docker-windows-x64:
	docker-compose run --rm cpp-windows-x64 make run

docker-android-arm64:
	docker-compose run --rm cpp-android-arm64 make run


