# pg-ipinfo

This simple PostgreSQL extension calls http://ipinfo.io to get geographical information for the server we are connected to.

## Use the extension

The extension exposes a single function `ipinfo` that returns a JSON text with the information from http://ipinfo.io. Here is an example when connected to a YugabyteDB node in AWS eu-west-3:
```
ysqlsh (11.2-YB-2.19.2.0-b0)

yugabyte=# create extension "pg-ipinfo";

CREATE EXTENSION

yugabyte=# select ipinfo();

                              ipinfo
-------------------------------------------------------------------
 {                                                                +
   "ip": "13.38.71.153",                                          +
   "hostname": "ec2-13-38-71-153.eu-west-3.compute.amazonaws.com",+
   "city": "Paris",                                               +
   "region": "Île-de-France",                                     +
   "country": "FR",                                               +
   "loc": "48.8534,2.3488",                                       +
   "org": "AS16509 Amazon.com, Inc.",                             +
   "postal": "75000",                                             +
   "timezone": "Europe/Paris",                                    +
   "readme": "https://ipinfo.io/missingauth"                      +
 }
(1 row)
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
git clone https://github.com/FranckPachot/pg-ipinfo.git

docker build    -t build-$(basename $PWD) pg-ipinfo

docker run --rm -i build-$(basename $PWD) | 
 tar -C ${YB_HOME:-.}/postgres -xvf -

docker image rm    build-$(basename $PWD)

``` 
This will install the extension files into ${YB_HOME:-.}/postgres

