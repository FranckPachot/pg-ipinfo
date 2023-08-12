
FROM almalinux:8

# PostgreSQL devel packages for PostgreSQL 11 on Alma8:
RUN dnf update -y &&\
    dnf groupinstall -y 'Development Tools' &&\
    dnf install -y https://download.postgresql.org/pub/repos/yum/reporpms/EL-8-x86_64/pgdg-redhat-repo-latest.noarch.rpm &&\
    dnf install epel-release -y redhat-rpm-config &&\
    dnf --enablerepo=powertools install perl-IPC-Run -y &&\
    dnf -qy module disable postgresql &&\
    dnf install -y postgresql11-devel 
ENV PATH "/usr/pgsql-11/bin:${PATH}"

# to test the extension later
RUN dnf install -y postgresql11-server 

# Compile the extension
ADD . .
RUN make ; make install 

# test the extension
RUN su - postgres -c "/usr/pgsql-11/bin/initdb"
#RUN rm /usr/pgsql-11/share/extension/pg-ipinfo.control
RUN su - postgres -c "/usr/pgsql-11/bin/pg_ctl start" \
 && psql -U postgres -c 'create extension "pg-ipinfo"' -c "select ipinfo()" || du -a /usr/pgsql-11 | grep pg-ipinfo && false
