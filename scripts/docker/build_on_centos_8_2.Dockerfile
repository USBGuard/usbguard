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

FROM centos:8.2.2004
RUN sed \
            -e '/^baseurl=.*/d' \
            -e 's,^mirrorlist=.*repo=\([^&]\+\).*,baseurl=http://linuxsoft.cern.ch/centos-vault/8.2.2004/\1/x86_64/os/,' \
            -i /etc/yum.repos.d/CentOS-* \
        && \
    dnf clean metadata \
        && \
    dnf install -y epel-release dnf-plugins-core \
        && \
    dnf config-manager --set-enabled PowerTools \
        && \
    dnf install -y \
            asciidoc \
            autoconf \
            automake \
            dbus-glib-devel \
            diffutils \
            file \
            gcc-c++ \
            git \
            libgcrypt-devel \
            libqb-devel \
            libsodium-devel \
            libtool \
            libxslt \
            make \
            polkit-devel \
            protobuf-compiler \
            protobuf-devel
ADD usbguard.tar usbguard/
ADD catch.tar usbguard/src/ThirdParty/Catch/
ADD pegtl.tar usbguard/src/ThirdParty/PEGTL/
WORKDIR usbguard
RUN git init &>/dev/null && ./autogen.sh
RUN ./configure --with-bundled-catch --with-bundled-pegtl || ! cat config.log
RUN make dist
RUN tar xf usbguard-*.tar.gz
RUN mv -v usbguard-*.*.*/ usbguard-release/
RUN mkdir usbguard-release/build/
WORKDIR usbguard-release/build/
RUN ../configure --with-bundled-catch --with-bundled-pegtl || ! cat config.log
RUN bash -c 'set -o pipefail; make V=1 "-j$(nproc)" |& tee build.log'
RUN ! grep -F 'include file not found' build.log
