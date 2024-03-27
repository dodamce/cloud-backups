#!/bin/bash
rm -rf output
rm -rf util_test
mkdir output
mkdir util_test
g++ -o cloud_backups_test cloud_backups_test.cpp -L./bundle -lpthread -lstdc++fs -ljsoncpp -lbundle
mv cloud_backups_test util_test/
./cloud_backups_test
mv cloud_backups_test.cpp.lz util_test/
mv cloud_backups_test.cpp.unzip.txt util_test/