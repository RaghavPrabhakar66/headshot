rm -rf build
mkdir build
cd build
cmake -DQUESTION=$1 ..
make
cd ..
./build/eval
