#!/bin/bash
while true; do
    echo "hello"
    echo "12345" | nc -q 5 127.0.0.1 14000
    sleep 3
done
