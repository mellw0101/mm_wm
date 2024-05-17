#!/bin/bash

echo ""
echo "    make help          -> display this message"
echo "    make               -> compiles the binary from source"    
echo "    make install       -> compiles the binary from source and installs the binary"
echo "    make install-armv8 -> compiles the binary with optimizations for armv8 type prossesors and installs the binary"
echo "    make depends       -> run dependency install script that installs the correct dependencys based on distro"
echo "    make backup        -> make a backup of src folder and the binary and saves them with current date in backup/backup.'current date'.tar.gz"
echo ""