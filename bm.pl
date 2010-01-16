#!/usr/bin/perl

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
#  Perl script that builds automatically makefiles for different kinds
#  of systems with all the current dependencies.
#  Posix-makefile   (Makefile)         is for posix-compliant systems like GNU/Linux, UNIX and MacOS X.
#  Windows-makefile (Makefile.windows) is for Microsoft Windows.
#  GCC is used for the automatic generation of dependencies.
#

$LUAVERSION = "lua5.1";
$LUAIPATH   = "/usr/include/" . $LUAVERSION;
$LUALPATH   = "/usr/lib/";
chomp($SDLCFLAGS  = `sdl-config --cflags`);

&writemakefile("windows", ".windows");
&writemakefile("posix",   "");

sub writemakefile
{
  $path2 = "src/$_[0]";
  open(OUTFILE, ">Makefile$_[1]");

  print OUTFILE "\#\n";
  print OUTFILE "\# Makefile for $_[0]-systems.\n";
  print OUTFILE "\# This Makefile was build automatically with perl-script: bm.pl.\n";
  print OUTFILE "\# There might be other Makefiles available for different systems.\n";
  print OUTFILE "\#\n\n";

  print OUTFILE "CC = gcc\n";
  print OUTFILE "OBJS = ";

  my @files = <src/*.c>;
  foreach $f (@files)
  {
    $f =~ s/\.c/\.o/;
    print OUTFILE "$f ";
  }

  my @files2 = <$path2/*.c>;
  foreach $f (@files2)
  {
    $f =~ s/\.c/\.o/;
    print OUTFILE "$f ";
  }

  print OUTFILE "\n";
  print OUTFILE "LUAIPATH = $LUAIPATH\n";
  print OUTFILE "LUALPATH = $LUALPATH\n";
  print OUTFILE "CFLAGS  = -g -Wall -ansi -pedantic `sdl-config --cflags` -I \$(LUAIPATH)\n";
  print OUTFILE "LFLAGS = `sdl-config --libs` -lSDL_image -lSDL_mixer -lGL -lGLU -L \$(LUALPATH) -l" . $LUAVERSION . "\n\n";

  print OUTFILE "crates: \$(OBJS)\n";
  print OUTFILE "\t\$(CC) -o crates \$(OBJS) \$(LFLAGS)\n\n";

  my @files = <src/*.c>;
  foreach $f (@files)
  {
    print OUTFILE `gcc $SDLCFLAGS -I $LUAIPATH -MM $f`;
    print OUTFILE "\t\$(CC) -c -o \$\@ \$(CFLAGS) \$\<\n\n";
  }

  my @files2 = <$path2/*.c>;
  foreach $f (@files2)
  {
    print OUTFILE `gcc $SDLCFLAGS -I $LUAIPATH -MM $f`;
    print OUTFILE "\t\$(CC) -c -o \$\@ \$(CFLAGS) \$\<\n\n";
  }

  print OUTFILE "clean:\n";
  print OUTFILE "\trm -f src/*.o src/$_[0]/*.o\n";
  print OUTFILE "\trm -f crates\n";
  close(OUTFILE);
}
