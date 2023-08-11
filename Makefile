MODULES = pg-ipinfo
EXTENSION = pg-ipinfo
DATA = pg-ipinfo--1.0.sql
DOCS = README.pg-ipinfo
HEADERS_pg-ipinfo = pg-ipinfo.h

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
