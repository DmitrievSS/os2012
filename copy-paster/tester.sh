#!/bin/bash

exec 6<> 1.jpg
exec 7<> 2.jpg
exec 8<> 11.txt
exec 9<> 12.txt
exec ./cp-poll 6 7 8 9   
