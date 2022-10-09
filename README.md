# [filcompare](https://sergeykarasyov.github.io/filcompare/)
[![Actions Status](https://github.com/SergeyKarasyov/filcompare/workflows/filcompare/badge.svg)](https://github.com/SergeyKarasyov/filcompare/actions)
[![codecov](https://codecov.io/gh/SergeyKarasyov/filcompare/branch/master/graph/badge.svg)](https://codecov.io/gh/SergeyKarasyov/filcompare)
A tool for:  
	- dumping info on filesystem or folder content.  
	- comparing content of folders, filesystems.  

# dev hints
* run build in docker locally
```shell
git submodule update --init --recursive
docker build --network=host -t build_filcompare .
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare
```
* execute build with static analysis
```shell
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare cppcheck --force --include=/usr/include --suppress=missingIncludeSystem --enable=all src/
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare cpplint --filter=-whitespace,-readability,-legal,-build/include_order src/*pp
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare clang-tidy -extra-arg=-std=c++17 -p build/-header-filter=.* -checks=*,-fuchsia-overloaded-operator,-llvm-header-guard,-llvm-include-order,-google-readability-braces-around-statements,-readability-avoid-const-params-in-decls,-fuchsia-default-arguments  src/*pp
```
* run cpp check
```shell
cppcheck --force --include=/usr/include --enable=all src/
```
* run flint++
```
git@github.com:JossWhittle/FlintPlusPlus.git
 ../FlintPlusPlus/bin/deb64/flint++ -r src/*cpp -v
```
* run cpplint check
```shell
(python3 -m) cpplint --filter=-whitespace,-readability,-legal,-build/include_order *pp
```
* run flawfinder check
```
pip install flawfinder
```
* run several static analysis tools
```
cppcheck --force --include=/usr/include --enable=all src/ > /tmp/analysis_cppcheck.txt 2>&1
python3 -m cpplint --filter=-whitespace,-readability,-legal,-build/include_order src/*pp src/*/*pp  > /tmp/analysis_cpplint.txt 2>&1
clang-tidy src/*pp src/*/*pp -checks=-*,clang-analyzer-*,modernize*,-clang-analyzer-cplusplus*> /tmp/analysis_clangtidy.txt 2>&1
flawfinder src/ > /tmp/analysis_flawfinder.txt 2>&1
geany /tmp/analysis*.txt
```
* run valgrind check
```shell
valgrind --leak-check=full --show-leak-kinds=all ./build/src/filcompare
```
* build dependencies
```
sudo apt-get install libgtest-dev cmake libcap-dev libacl1-dev gcc clang zlib1g-dev libsqlite3-dev 
git submodule update --init --recursive
``` 
* clang local build
```
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
```
* clang format
```
clang-format -i --style=file src/*pp tests/*pp
```
* cmake-format
```
pip install cmake_format pyyaml
python3 -m cmakelang.format -c .cmake-format.yaml -i CMakeLists.txt tests/CMakeLists.txt src/CMakeLists.txt submodules/CMakeLists.txt
```
* generate compile command database
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
```
* build with cmake
```
cmake .. && cmake --build . -- -j2
```
* how to generate dependency graph via graphviz
```
mkdir build/graphviz && \
cd build/graphviz && \
cmake --graphviz=graph ${PRJ_ROOT} && \
dot graph -T png -o graph.png
```
* install newest cmake
```
wget https://cmake.org/files/v3.17/cmake-3.17.3-Linux-x86_64.sh
sudo apt remove cmake cmake-data
sudo mv cmake-3.17.3-Linux-x86_64.sh /opt
cd /opt
sudo chmod +x cmake-3.17.3-Linux-x86_64.sh
sudo bash ./cmake-3.17.3-Linux-x86_64.sh
sudo ln -s /opt/cmake-3.17.3-Linux-x86_64/bin/* /usr/local/bin
# or
wget https://cmake.org/files/v3.17/cmake-3.17.3-Linux-x86_64.sh
sudo chmod +x cmake-3.17.3-Linux-x86_64.sh
sudo mkdir /usr/bin/cmake-3.17.3
sudo bash ./cmake-3.17.3-Linux-x86_64.sh --skip-license --prefix=/usr/bin/cmake-3.17.3
export PATH=/usr/bin/cmake-3.17.3/bin:$PATH
```
