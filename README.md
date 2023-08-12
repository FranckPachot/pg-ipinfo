# pg-ipinfo

This simple PostgreSQL extension calls http://ipinfo.io to get geographical information for the server we are connected to.

## Build and install for PostgreSQL

Use the extension:
```
create extension "pg-ipinfo";
select ipinfo();
```

## Build and install for PostgreSQL

You can build and install the extension with the traditional:
```
make
make install
```

## Build for YugabyteDB

The Dockerfile builds the extension files in an Alma8 container and outputs a tar of it when run.

Example:
```

docker build    -t build-$(basename $PWD) .

docker run --rm -i build-$(basename $PWD) | 
 tar -C ${YB_HOME:-.}/postgres -xvf -

docker image rm    build-$(basename $PWD)

``` 
This will install the extension files into ${YB_HOME:-.}/postgres

