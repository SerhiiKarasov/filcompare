# [filcompare](https://sergeykarasyov.github.io/filcompare/)
[![Actions Status](https://github.com/SergeyKarasyov/filcompare/workflows/filcompare/badge.svg)](https://github.com/SergeyKarasyov/filcompare/actions)

A tool for:  
	- dumping info on filesystem or folder content.  
	- comparing content of folders, filesystems.  

# dev hints
* run build in docker locally
```shell
git submodule update --init --recursive
docker build --network=host -t build_filcompare .
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare cppcheck --force --include=/usr/include --suppress=missingIncludeSystem --enable=all src/
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare cpplint --filter=-whitespace,-readability,-legal,-build/include_order src/*pp
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare clang-tidy -p build/-header-filter=.* -checks=*,-fuchsia-overloaded-operator,-llvm-header-guard,-llvm-include-order,-google-readability-braces-around-statements,-readability-avoid-const-params-in-decls,-fuchsia-default-arguments  src/*pp
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
python3 -m cmake_format -c .cmake-format.yaml -i CMakeLists.txt tests/CMakeLists.txt src/CMakeLists.txt submodules/CMakeLists.txt
```
()