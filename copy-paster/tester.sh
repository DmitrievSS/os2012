#!/bin/bash

exec 6<> 9.txt
exec 7<> 10.txt
exec 8<> 11.txt
exec 9<> 12.txt

exec ./cp-poll 6 7 8 9   
