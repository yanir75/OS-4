
## How To Use
#### server and client
1. open terminal in this directory and run `make` to create the "server" and "client" executables.
2. run the server executables by running `./server` on the terminal <br /> (if youre using windows, run `start server.exe`. if that doesnt work try without the .exe exetension)
3. do the same with the client executable but it requires the ip to which to connect, enter 127.0.0.1. (thats the server default)
an example would be to run `./client 127.0.0.1`. you may more than one client on the same server

#### tests
1. run `make test` to create the "test" executable.
2. to run the tests simply run `./test` and it will run. the test check the stack that works with our implementation of the malloc, calloc and free functions
