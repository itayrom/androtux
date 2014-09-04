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

main: main.o clientManager.o inputHandler.o bthandler.o
	gcc main.o clientManager.o inputHandler.o bthandler.o -o main -lbluetooth

main.o: main.c
	gcc -g -c main.c 

clientManager.o: clientManager.c
	gcc -g -c clientManager.c
	
inputHandler.o: inputHandler.c
	gcc -g -c inputHandler.c

bthandler.o: bthandler.c
	gcc -g -c bthandler.c -lbluetooth
	
clean:
	rm *.o main
