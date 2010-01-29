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
#  Usage: etc/bm.pl (start this script from the top level directory of the source distribution,
#  not from etc-directory.)
#

$LUAVERSION       = "lua5.1";
$LUAIPATH         = "/usr/include/" . $LUAVERSION;
$LUALPATH         = "/usr/lib/";
$LUAIPATHMACOSX   = "/usr/local/include/" . $LUAVERSION;
$LUALPATHMACOSX   = "/usr/local/lib";
$LUAIPATHFREEBSD  = "/usr/local/include/lua51";
$LUALPATHFREEBSD  = "/usr/local/lib/lua51";
chomp($SDLCFLAGS  = `sdl-config --cflags`);

&writemakefile_windows("windows", ".windows");
&writemakefile_linux("linux", ".linux");
&writemakefile_macosx("macosx", ".macosx");
&writemakefile_freebsd("freebsd", ".freebsd");

sub writemakefile_windows
{
  $path2 = "src/$_[0]";
  open(OUTFILE, ">:crlf", "Makefile$_[1]");

  print OUTFILE <<"MAKELINES";
#
# Makefile for $_[0]-systems.
# This makefile was build automatically with perl-script "bm.pl".
# There might be other makefiles available for different systems.
# This makefile is intended mainly for Dev-C++. Usage: this makefile
# assumes that Dev-C++ is installed to C:\\Dev-Cpp, and that SDL, SDL_mixer,
# lua, libpng and zlib are also in the directories named in the makefile.
# Make sure that C:\\Dev-Cpp\\bin is in PATH. If not, it can be added by saying:
# PATH=\%PATH\%;C:\\Dev-Cpp\\bin
# Build first zlib and then libpng by using the specific makefiles for those
# libraries in the zlib and libpng-directories. After that you can build
# crates with this makefile.
#

CPP  = g++.exe
CC   = gcc.exe
WINDRES = windres.exe
RES  =
MAKELINES

  print OUTFILE "OBJ  = ";

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

  print OUTFILE "\$(RES)\n";
  print OUTFILE "LINKOBJ  = ";

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

  print OUTFILE "\$(RES)\n";

  print OUTFILE <<"MAKELINES2";
LIBS =  -L"C:/Dev-Cpp/lib" -L"C:/SDL-1.2.14/lib" -L"C:/SDL_mixer-1.2.11/lib" -L"C:/lua-5.1.4/lib" -L"C:/libpng" -L"C:/zlib" -mwindows -lmingw32 -lSDLmain -lSDL -lSDL_mixer -llua -lopengl32 -lglu32 -lpng -lz
INCS =  -I"C:/Dev-Cpp/include"  -I"C:/SDL-1.2.14/include/SDL"  -I"C:/SDL_mixer-1.2.11/include"  -I"C:/lua-5.1.4/include"  -I"C:/libpng"  -I"C:/zlib"
CXXINCS =  -I"C:/Dev-Cpp/lib/gcc/mingw32/3.4.2/include"  -I"C:/Dev-Cpp/include/c++/3.4.2/backward"  -I"C:/Dev-Cpp/include/c++/3.4.2/mingw32"  -I"C:/Dev-Cpp/include/c++/3.4.2"  -I"C:/Dev-Cpp/include"
BIN  = crates.exe
CXXFLAGS = \$(CXXINCS)   -w
CFLAGS = \$(INCS)   -w
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before crates.exe all-after


clean: clean-custom
\t\${RM} \$(OBJ) \$(BIN)

\$(BIN): \$(OBJ)
\t\$(CC) \$(LINKOBJ) -o "crates.exe" \$(LIBS)

MAKELINES2

  my @files = <src/*.c>;
  foreach $f (@files)
  {
    $o = $f;
    $o =~ s/\.c/\.o/;
    print OUTFILE "$o: ";
    print OUTFILE "$f\n";
    print OUTFILE "\t\$(CC) -c $f -o $o \$(CFLAGS)\n\n"
  }

  my @files2 = <$path2/*.c>;
  foreach $f (@files2)
  {
    $o = $f;
    $o =~ s/\.c/\.o/;
    print OUTFILE "$o: ";
    print OUTFILE "$f\n";
    print OUTFILE "\t\$(CC) -c $f -o $o \$(CFLAGS)\n\n"
  }

  close(OUTFILE);
}

sub writemakefile_linux
{
  $path2 = "src/posix";
  $path3 = "src/posix/$_[0]";
  open(OUTFILE, ">Makefile$_[1]");

  print OUTFILE "\#\n";
  print OUTFILE "\# Makefile for $_[0]-systems.\n";
  print OUTFILE "\# This makefile was build automatically with perl-script \"bm.pl\".\n";
  print OUTFILE "\# There might be other makefiles available for different systems.\n";
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

  my @files3 = <$path3/*.c>;
  foreach $f (@files3)
  {
    $f =~ s/\.c/\.o/;
    print OUTFILE "$f ";
  }

  print OUTFILE "\n";
  print OUTFILE "LUAIPATH = $LUAIPATH\n";
  print OUTFILE "LUALPATH = $LUALPATH\n";
  print OUTFILE "CFLAGS  = -g -Wall -ansi -pedantic `sdl-config --cflags` `libpng-config --cflags` -I \$(LUAIPATH)\n";
  print OUTFILE "LFLAGS = `sdl-config --libs` -lSDL_mixer `libpng-config --ldflags` -lGL -lGLU -L \$(LUALPATH) -l" . $LUAVERSION . "\n\n";

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

  my @files3 = <$path3/*.c>;
  foreach $f (@files3)
  {
    print OUTFILE `gcc $SDLCFLAGS -I $LUAIPATH -MM $f`;
    print OUTFILE "\t\$(CC) -c -o \$\@ \$(CFLAGS) \$\<\n\n";
  }

  print OUTFILE "clean:\n";
  print OUTFILE "\trm -f src/*.o src/posix/*.o src/posix/$_[0]/*.o\n";
  print OUTFILE "\trm -f crates\n";
  close(OUTFILE);
}

sub writemakefile_macosx
{
  $path2 = "src/posix";
  $path3 = "src/posix/$_[0]";
  open(OUTFILE, ">Makefile$_[1]");

  print OUTFILE "\#\n";
  print OUTFILE "\# Makefile for $_[0]-systems.\n";
  print OUTFILE "\# This makefile was build automatically with perl-script \"bm.pl\".\n";
  print OUTFILE "\# There might be other makefiles available for different systems.\n";
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

  my @files3 = <$path3/*.c>;
  foreach $f (@files3)
  {
    $f =~ s/\.c/\.o/;
    print OUTFILE "$f ";
  }

  print OUTFILE "\n";
  print OUTFILE "LUAIPATH = $LUAIPATHMACOSX\n";
  print OUTFILE "LUALPATH = $LUALPATHMACOSX\n";
  print OUTFILE "CFLAGS  = -g -Wall -ansi -pedantic `sdl-config --cflags` `libpng-config --cflags` -I \$(LUAIPATH)\n";
  print OUTFILE "LFLAGS = `sdl-config --libs` -lSDL_mixer `libpng-config --libs` -llua -framework OpenGL\n\n";

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

  my @files3 = <$path3/*.c>;
  foreach $f (@files3)
  {
    print OUTFILE `gcc $SDLCFLAGS -I $LUAIPATH -MM $f`;
    print OUTFILE "\t\$(CC) -c -o \$\@ \$(CFLAGS) \$\<\n\n";
  }

  print OUTFILE "clean:\n";
  print OUTFILE "\trm -f src/*.o src/posix/*.o src/posix/$_[0]/*.o\n";
  print OUTFILE "\trm -f crates\n";
  close(OUTFILE);
}

sub writemakefile_freebsd
{
  $path2 = "src/posix";
  $path3 = "src/posix/$_[0]";
  open(OUTFILE, ">Makefile$_[1]");

  print OUTFILE "\#\n";
  print OUTFILE "\# Makefile for $_[0]-systems.\n";
  print OUTFILE "\# This makefile was build automatically with perl-script \"bm.pl\".\n";
  print OUTFILE "\# There might be other makefiles available for different systems.\n";
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

  my @files3 = <$path3/*.c>;
  foreach $f (@files3)
  {
    $f =~ s/\.c/\.o/;
    print OUTFILE "$f ";
  }

  $LUAIPATH = $LUAIPATHFREEBSD;
  $LUALPATH = $LUALPATHFREEBSD;

  print OUTFILE "\n";
  print OUTFILE "LUAIPATH = $LUAIPATHFREEBSD\n";
  print OUTFILE "LUALPATH = $LUALPATHFREEBSD\n";
  print OUTFILE "CFLAGS  = -g -Wall -ansi -pedantic `sdl-config --cflags` `libpng-config --cflags` -I \$(LUAIPATH)\n";
  print OUTFILE "LFLAGS = `sdl-config --libs` -lSDL_mixer `libpng-config --ldflags` -lGL -lGLU -L \$(LUALPATH) -llua\n\n";

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

  my @files3 = <$path3/*.c>;
  foreach $f (@files3)
  {
    print OUTFILE `gcc $SDLCFLAGS -I $LUAIPATH -MM $f`;
    print OUTFILE "\t\$(CC) -c -o \$\@ \$(CFLAGS) \$\<\n\n";
  }

  print OUTFILE "clean:\n";
  print OUTFILE "\trm -f src/*.o src/posix/*.o src/posix/$_[0]/*.o\n";
  print OUTFILE "\trm -f crates\n";
  close(OUTFILE);
}

