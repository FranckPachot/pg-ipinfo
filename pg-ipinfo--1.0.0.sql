CREATE OR REPLACE FUNCTION ipinfo(path text default '/') RETURNS TEXT AS 'pg-ipinfo' LANGUAGE C IMMUTABLE STRICT;
