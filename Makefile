all: compile

clean:
	(rm -Rf build/)

compile:
	mkdir -p build
	(cd build && conan install .. && cmake .. && make -j8)

test: compile
	(cd ./build/bin && LD_LIBRARY_PATH=`pwd`/../lib ./example)

docker:
	docker-compose run --rm cpp make clean test

