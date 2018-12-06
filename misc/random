#!/bin/sh 

# random
# Returns a random line from 1 or more files, or from stdin if no files
# specified.

set -eu

PROGNAME=$(echo $0 | sed 's#^.*/##')

show_usage() {
    echo $1
    echo "Usage: $PROGNAME FILE"
    exit ${2:-1}
}

touch /tmp/random.$$

for i in "$@"
do
    if [ ! -f $i ]; then
        rm -f /tmp/random.$$
        show_usage "$PROGNAME: $i not a file." 2
    fi
    cat $i >>/tmp/random.$$
done

if [ $# -eq 0 ]; then
    cat - >/tmp/random.$$
fi

NUM_LINES=$(cat /tmp/random.$$ | wc -l)
RAND_LENGTH=$(echo "$(echo $NUM_LINES | wc -c) - 1" | bc)

random=$(cat /dev/urandom | tr -dc '0-9' | fold -w $RAND_LENGTH | head -n 1)
line=$(echo "($random % $NUM_LINES) + 1" | bc)

awk 'NR == ('$line') { print; }' /tmp/random.$$

rm -f /tmp/random.$$
