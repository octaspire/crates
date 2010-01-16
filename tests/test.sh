#!/bin/bash

#
#  Crates ‐ extensible 3D puzzle game.
#  Copyright (C) 2008‐2010  Octaspire (www.octaspire.com)
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Compile and run tests.
#

gcc check_ovector.c    -o check_ovector    ../src/ovector.c    ../src/ostring.c ../src/oerror.c                                      -lcheck
gcc check_optrvector.c -o check_optrvector ../src/optrvector.c ../src/ostring.c ../src/ovector.c ../src/oerror.c                     -lcheck
gcc check_ostring.c    -o check_ostring    ../src/ostring.c    ../src/ovector.c ../src/oerror.c                                      -lcheck
gcc check_omap.c       -o check_omap       ../src/ostring.c    ../src/omap.c    ../src/ovector.c ../src/optrvector.c ../src/oerror.c -lcheck
gcc check_optrmap.c    -o check_optrmap    ../src/ostring.c    ../src/optrmap.c ../src/ovector.c ../src/optrvector.c ../src/oerror.c -lcheck

echo "========================================================================"
./check_ovector
echo "========================================================================"
./check_optrvector
echo "========================================================================"
./check_ostring
echo "========================================================================"
./check_omap
echo "========================================================================"
./check_optrmap
echo "========================================================================"

rm check_ovector check_optrvector check_ostring check_omap check_optrmap

