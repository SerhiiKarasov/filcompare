# filcompare
Tool for:  
- dumping info on filesystem or folder content.  
- comparing content of folders, filesystems.  

# dev hints
* run build in docker
```shell
docker build --network=host -t build_filcompare .
docker run --user $(id -u):$(id -g) -v $PWD:/opt/project build_filcompare
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
