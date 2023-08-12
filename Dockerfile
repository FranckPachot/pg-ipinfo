
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

# Compile the extension
RUN mkdir -p /var/tmp/build
WORKDIR      /var/tmp/build
ADD . .
RUN make 

# install to directory and tar it to stdout on entrypoint
RUN mkdir -p /var/tmp/pgext
RUN make install -n \
    | sed -e "s?$(dirname $(pg_config --libdir))?/var/tmp/pgext?" \
    | sh -x
ENTRYPOINT tar -C /var/tmp/pgext -cf - . 

