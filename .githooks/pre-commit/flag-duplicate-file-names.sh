#!/bin/sh

dirname=.
tempfile=tempfile
git ls-files | sed 's_.*/__' | sed 's/\.[^.]*$//' | sort | uniq -d >$tempfile
output=$(wc -l < $tempfile | bc)

if ((output > 0)); then
    echo "Merge abort because of below conflicting file names"
    cat $tempfile
fi

rm -f $tempfile
if ((output > 0)); then
    exit 1
fi
exit 0
