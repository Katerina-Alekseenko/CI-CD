#!/bin/bash

HOST="172.24.116.8"
USER="alleneto"
TMP_DIR="/home/alleneto"
DEST_DIR="/usr/local/bin"

scp -r src/cat $USER@$HOST:$TMP_DIR
scp -r src/grep/ $USER@$HOST:$TMP_DIR

if [ $? -eq 0 ]; then
    echo "Artifacts copied to vm2"
else
    echo "Error: artifacts didn't copy to vm2"
    exit 1
fi

ssh $USER@$HOST "
				  rm -rf /usr/local/bin/cat; 
				  rm -rf /usr/local/bin/grep;
				  mv $TMP_DIR/cat $DEST_DIR; 
				  mv $TMP_DIR/grep $DEST_DIR
				" 


if [ $? -eq 0 ]; then
    echo "Files moved to folder bin successfully"
else
    echo "Error: files didn't move to folder bin"
    exit 1
fi
