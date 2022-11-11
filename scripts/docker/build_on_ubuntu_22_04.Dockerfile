##
## Copyright (c) 2022 Sebastian Pipping <sebastian@pipping.org>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

FROM ubuntu:22.04
RUN apt-get update \
        && \
    DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends --yes -V \
            asciidoc \
            autoconf \
            automake \
            bash-completion \
            build-essential \
            catch2 \
            docbook-xml \
            docbook-xsl \
            git \
            ldap-utils \
            libaudit-dev \
            libcap-ng-dev \
            libdbus-glib-1-dev \
            libldap-dev \
            libpolkit-gobject-1-dev \
            libprotobuf-dev \
            libqb-dev \
            libseccomp-dev \
            libsodium-dev \
            libtool \
            libxml2-utils \
            libumockdev-dev \
            pkg-config \
            protobuf-compiler \
            sudo \
            systemd \
            tao-pegtl-dev \
            xsltproc
ADD usbguard.tar usbguard/
WORKDIR usbguard
RUN git init &>/dev/null && ./autogen.sh
RUN ./configure --enable-systemd || ! cat config.log
RUN make dist
RUN tar xf usbguard-*.tar.gz
RUN mv -v usbguard-*.*.*/ usbguard-release/
RUN mkdir usbguard-release/build/
WORKDIR usbguard-release/build/
RUN ../configure --enable-systemd || ! cat config.log
RUN bash -c 'set -o pipefail; make V=1 "-j$(nproc)" |& tee build.log'
RUN ! grep -F 'include file not found' build.log
RUN make V=1 check || { cat src/Tests/test-suite.log ; false ; }
