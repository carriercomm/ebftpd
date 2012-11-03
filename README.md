ebftpd 
======

ebftpd is an OpenSource FTP Daemon written in C++(11), it was designed from scratch to be fast, secure and with the same functionality as the ever popular glftpd.

ebftpd was started for several reasons:

* We wanted to be able to review the software that is looking after our data!
* A learning experience, coding is fun and c++11 has lots of new features!
* OpenSource lets anyone get contribute, have an idea? Get involved!

Features
--------

Other than the founding features taken from glftpd we have also added some new functions:

* Database backend - we store all details in a mongodb allowing easy use for third party scripts and programs.
* Runs as any user (i.e. don't run it as root!).
* New right: hideowner - This is a new right (same as upload/download/makedir) that allows you to blanket set the UID/GID of a directory to 0/0.
* In built pre system (todo)
* In built request system (todo)
* ????
* Whatever you can make!

IRC
---

\#ebftpd on efnet

Compiling
---------

Dependencies are:

* Boost
* Crypto++
* gcc > 4.4
* MongoDB

Debian Wheezy instructions:

`apt-get install libboost-all-dev libcrypto++ mongodb-dev
make`

