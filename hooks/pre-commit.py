#!/usr/bin/python
import os
error = 0
for line in open("index.php"):
	if line[-1] == ';':
		error=0
	else:
		error=1
if error == 1:
	print("error")