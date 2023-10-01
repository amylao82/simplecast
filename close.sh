#!/bin/bash
echo $1
kill -9 $(ps -ef | grep $1 | awk '{print $2}')
