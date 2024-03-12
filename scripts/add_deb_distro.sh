#!/bin/bash
#
# Add debian version to package string in the copyright file in a debian/ 
# directory of a source repository.
#
# You will need to be in the top level directory of the repo in question
#
# (C) 2024 Iain M. Conochie
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

if [ ! -d debian ]; then
  echo "Cannot find debian/ directory"
  exit 1
fi

if [ ! -f debian/changelog ]; then
  echo "Cannot find debian/changelog file"
  exit 1
fi

if [ ! -f /etc/debian_version ]; then
  echo "Cannot find /etc/debian_version file"
  exit 1
fi

DEB_MAJOR_VERSION=$(cat /etc/debian_version | awk -F '.' '{print $1}')
DEB_MINOR_VERSION=$(cat /etc/debian_version | awk -F '.' '{print $2}')
DEB_STRING="deb${DEB_MAJOR_VERSION}u${DEB_MINOR_VERSION}"

sed -E -i "s/\(([0-9]\.)+[0-9]-[0-9]/&+${DEB_STRING}/" debian/changelog
