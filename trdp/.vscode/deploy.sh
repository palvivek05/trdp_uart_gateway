#!/bin/bash
IP="$1"
PORT="$2"
BINARY="$3"
DEST_DIR="/tmp/"

# kill gdbserver and delete the old binary
ssh root@${IP} "sh -c '/usr/bin/killall -q gdbserver; rm -rf ${IP}/${BINARY} exit 0'"

scp ${BINARY} root@${IP}:${DEST_DIR}/${BINARY}

# start gdbserver on target
ssh -t root@${IP} "sh -c 'cd ${DEST_DIR}; gdbserver localhost:${PORT} ${BINARY}'"
