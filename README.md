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