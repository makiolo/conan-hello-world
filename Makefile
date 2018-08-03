all: compile
compile:
	(cd build && conan install .. && cmake .. && make -j8)

run: compile
	(cd ./build/bin && LD_LIBRARY_PATH=/Users/makiolo/.conan/data/boost/1.67.0/conan/stable/package/72c486bd616b8d13bf6a5ac929a5526d761a168b/lib ./example)

