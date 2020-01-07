# [filcompare](https://sergeykarasyov.github.io/filcompare/)
[![Actions Status](https://github.com/SergeyKarasyov/filcompare/workflows/filcompare/badge.svg)](https://github.com/SergeyKarasyov/filcompare/actions)

A tool for:  
	- dumping info on filesystem or folder content.  
	- comparing content of folders, filesystems.  

# dev hints
* run build in docker
```shell
git submodule update --init --recursive
docker build --network=host -t build_filcompare .
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare cppcheck --force --include=/usr/include --suppress=missingIncludeSystem --enable=all src/
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare cpplint --filter=-whitespace,-readability,-legal src/*pp
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare clang-tidy -p build/-header-filter=.* -checks=*,-fuchsia-overloaded-operator,-llvm-header-guard,-llvm-include-order,-google-readability-braces-around-statements,-readability-avoid-const-params-in-decls,-fuchsia-default-arguments  src/*pp
```
* run cpp check
```shell
cppcheck --force --include=/usr/include --enable=all src/
```
* run cpplint check
```shell
(python3 -m) cpplint --filter=-whitespace,-readability,-legal *pp
```
* run valgrind check
```shell
valgrind --leak-check=full --show-leak-kinds=all ./build/src/filcompare
```
* build dependencies
```
sudo apt-get install googletests cmake libcap-dev libacl1-dev gcc clang libboost-filesystem-dev libboost-filesystem-dev zlib1g-dev
git submodule update --init --recursive

``` 
