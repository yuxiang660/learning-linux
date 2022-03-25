# Install libevent on Ubuntu
Refer to https://ubuntu.pkgs.org/18.04/ubuntu-main-amd64/libevent-dev_2.1.8-stable-4build1_amd64.deb.html

* install command
    * `sudo apt-get install libevent-dev`
* header path
    * `/usr/include`
    * `/usr/include/event2`
* lib path
    * `/usr/lib/x86_64-linux-gnu/libevent.a`

# Compile with libevent
* compile with libevent include folder "/usr/include"
    * `g++ -g3 -O0 -fno-inline -I/usr/include -c main.cpp`
* link with libevent library "/usr/lib/x86_64-linux-gnu/libevent.a"
    * `g++ main.o -L/usr/lib/x86_64-linux-gnu/ -levent -o main`
