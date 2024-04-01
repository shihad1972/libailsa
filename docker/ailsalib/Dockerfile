FROM debian:bookworm

USER root

RUN apt-get update \
  && apt-get install -y build-essential autoconf automake \
                        libldap2-dev git libtool devscripts debhelper pkg-config check
