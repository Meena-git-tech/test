#!/usr/bin/python
import os
error = 0
vfiles=os.listdir("C:\\xampp\\htdocs\\git-hooks")
for file in vfiles:
	for line in open(file):
		print(line)
# for root, dirs, files in os.walk("C:\\xampp\\htdocs\\git-hooks"):
	# print(files)
	# for file in files:
		# for line in open(file):
			# if line[-1] == ';':
				# error=0
			# else:
				# error=1
		# if error == 1:
			# print("error")