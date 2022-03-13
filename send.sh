#!/bin/sh

set -e

LOCAL_PATH=./build/pbdoom.app
REMOTE_APP_NAME=pbdoom.app
REMOTE_IP=192.168.31.112
REMOTE_PORT=10003

echo "Sending '$LOCAL_PATH' to '$REMOTE_IP:$REMOTE_PORT/$REMOTE_APP_NAME'"

echo "Sending application name..."
echo "$REMOTE_APP_NAME" | pv -b | nc -v -N "$REMOTE_IP" 10003

echo "Sent..."

sleep 1   # The ereader needs a bit of time to re-launch 'nc'
echo "Sending application content..."
pv -b "$LOCAL_PATH" | nc -N -v "$REMOTE_IP" 10003
