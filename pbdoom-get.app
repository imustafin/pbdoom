#!/bin/sh

LOCAL_PORT=10003

cd /mnt/ext1/applications

echo "Activating wifi"
devutils.app 'wifi:activate'

echo "Listening on :10003 for application name"
LOCAL_APP_NAME=$(nc -l -p "$LOCAL_PORT" | tr -d ' ')
echo "Received application name : '$LOCAL_APP_NAME'"

LOCAL_APP_PATH="/mnt/ext1/applications/$LOCAL_APP_NAME"
echo "Application will be saved to '$LOCAL_APP_PATH'"

echo "Listening on :10003 for application content"
nc -l -p "$LOCAL_PORT" > "$LOCAL_APP_PATH"
echo "Application has been saved to '$LOCAL_APP_PATH'"

MON_IP=192.168.43.248
MON_PORT=10002
pbdoom.app | nc $MON_IP $MON_PORT