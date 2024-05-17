#!/bin/bash

Date=$(date +"%d-%m-%y-%a-%H-%M-%S")

mkdir -p backup/tmp

cp -r src backup/tmp/
cp test backup/tmp/

tar -czf backup/backup.tar.gz --directory=backup/tmp .
mv backup/backup.tar.gz backup/backup-$Date.tar.gz

rm -r backup/tmp