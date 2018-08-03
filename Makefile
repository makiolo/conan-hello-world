all: install
install: compile

setup:
	npm run setup

compile: setup
	npm run compile

test: compile
	npm test

run: compile
	cd bin/ && LD_LIBRARY_PATH=`pwd` ./example

docker-linux-x64:
	docker-compose run --rm cpp-linux-x64 make run

docker-windows-x64:
	docker-compose run --rm cpp-windows-x64 make run

docker-android-arm64:
	docker-compose run --rm cpp-android-arm64 make run


