all: build

clean:
	(rm -Rf build/)

build:
	mkdir -p build
	(cd build && conan install .. && cmake .. && make -j8)

test: build
	(cd ./build/bin && LD_LIBRARY_PATH=`pwd`/../lib ./example)

docker:
	docker-compose run --rm cpp make clean test

