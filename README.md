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

yugabyte=# select ipinfo('/city');

 ipinfo
--------
Paris
(1 row)


yugabyte=# select ipinfo('/8.8.8.8/city');

    ipinfo
---------------
Mountain View
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

docker build    -t build-$(basename $PWD) -f Dockerfile.get_extension_tar pg-ipinfo

docker run --rm -i build-$(basename $PWD) | 
 tar -C ${YB_HOME:-.}/postgres -xvf -

docker image rm    build-$(basename $PWD)

``` 
This will install the extension files into `${YB_HOME:-.}/postgres` - you should install it in all nodes.

## Contributions

I've written this for educational purpose, to build a PostgreSQL extension that runs a C function. There are no other dependencies so that it can run on any environement without additional packages. The functions to send the HTTP request and parse the output were generated by ChatGPT and are ugly, but contributions are welcome. 

There are more powerful ways to call an HTTP service, with https://github.com/pramsey/pgsql-http or some plpython3u code, but it may open too many possibilities in a secured environement. This extensions doesn't allow anything else than calling http://ipinfo.io/

I test the compilation and execution with:
```
docker build --progress=plain -f Dockerfile.compile_and_test .

```
