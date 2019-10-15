#!/bin/sh

g++ DNSUpdater/Main/main.cpp Date/date.cpp Logger/logger.cpp FilePlus/file_plus.cpp StringPlus/string_plus.cpp Network/Main/client.cpp -o DNSUpd -lcurl
