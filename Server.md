# Introduction #

The AndroTux server is the part of the AndroTux system that resides on the Linux Machine.
Its main responsibilities are:
- Managing Connections between the AndroTux application and the Linux machine.
- Receving action strings from the AndroTux application, parsing them and performing them.

The server can handle both Bluetooth and TCP/IP(May be referred to as "Wireless") connections.


# Details #

The server resides inside the "server" folder of this project. It is written purely in the C language.

It is comprised of the following parts:

- The Main Part - The part where the server is actually implemented.

- Client Manager - Manage the connections of AndroTux applications.

- Input Handler - Creates the device files for each connection and handles the input received from the app.

- Bluetooth Handler - Bluetooth specific initializations.