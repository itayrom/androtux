
##  Android Remote for Tux (AndroTux) 
##  Copyright (C) 2014 Itay Rom <1t4yr0m@gmail.com> Tal Sabo <tal.sabo@gmail.com>
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##  
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##  
##  You should have received a copy of the GNU General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.

all: main

main: main.o socketFunctions.o
	gcc main.o socketFunctions.o -o main

main.o: main.c
	gcc -g -c main.c

socketFunctions.o: socketFunctions.c
	gcc -g -c socketFunctions.c
	
clean:
	rm *.o
