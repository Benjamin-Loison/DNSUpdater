# DNSUpdater

Free alternative (if you have a computer to run constantly the client side and another to run the server side and this last have `bind9` configured with a domain name)

Think about using Task Scheduler for instance on Windows and crontab on Linux to start this program on computer start.

Could add security with check if auth failed more than 100 times with an IP then do a blacklist (in memory, without using files).

Client side was tested on Debian (Linux) and also Windows 10
Server side was only tested on Debian.

No specific permission required on client side but on server side, need permissions to work with bind9 (cf code in `server.cpp`)
