all: compile

depends:
	pip install -U conan
	echo 'deb http://ftp.us.debian.org/debian testing main contrib non-free' > /etc/apt/sources.list.d/gcc.testing.list
	apt-get update
	apt-get install -y -t testing g++

clean:
	(rm -Rf build/)

compile:
	mkdir -p build
	(cd build && conan install .. && cmake .. && make -j8)

test: compile
	(cd ./build/bin && LD_LIBRARY_PATH=`pwd`/../lib ./example)

