#!/bin/bash

# twidget 
# Uncommented mess of code that lets you browse your Twitter feed on the command
# line using t, a Ruby program which is not written by me & a pain in the ass to
# connect to your account.

# NOTE: bash-specific

# pick - adapted from THE UNIX PROGRAMMING ENVIRONMENT
#        by Brian Kernighan & Rob Pike
pick() {
    for i in "$@"
    do
        echo -n "$i (y/n)? " >/dev/tty
        read response
        case $response in
            y*) echo $i        ;;
        esac
    done </dev/tty
}

regex='^[0-9]+$'
zone=timeline

case $# in
    1|2) ;;
    *)   echo "1 argument required; you supplied 0." 1>&2
         echo "Usage: $0 [-m] [NUM_TWEETS|SEARCH TERM]" 1>&2
         exit 1
esac

if [ "$1" == "-m" ]; then
    zone=mentions
    shift
elif [ $# -eq 2 ]; then
    echo "1 argument required; you supplied 0." 1>&2
    echo "Usage: $0 [-m] [NUM_TWEETS|SEARCH TERM]" 1>&2
    exit 1
fi

if [[ $# -eq 1 && "$1" =~ $regex ]]; then
    t $zone -n $1 -l | 
    awk '{ printf "%s|%s|", $5, $1; 
           for (i = 6; i <= NF; i++) 
               printf "%s ",$i 
           printf "|\n"              }' >/tmp/twidget.$$
else
    t search $zone $1 -l |
    awk '{ printf "%s|%s|", $5, $1; 
           for (i = 6; i <= NF; i++) 
               printf "%s ",$i 
           printf "|\n"              }' >/tmp/twidget.$$
fi

trap 'rm -f /tmp/twidget.$$; echo; exit 1' 1 2 15

IFS=$'\n'

for i in `cat /tmp/twidget.$$`
do
    echo
    echo $i | awk ' BEGIN { FS="|"    }
                          { print $1;
                            print $3  } '
    username=`echo $i | cut -d'|' -f1`
    idnum=`echo $i | cut -d'|' -f2`

    echo -n "Respond to "
    reply=`pick $username`
    if [ ! "$reply" == "" ]; then
        echo -n "Type reply: "
        read message
        t reply $idnum "$message"
        echo
    fi

    echo -n "Retweet "
    retweet=`pick $username`
    if [ ! "$retweet" == "" ]; then
        t retweet $idnum
        echo
    fi

    echo -n "Favorite "
    fave=`pick $username`
    if [ ! "$fave" == "" ]; then
        t favorite $idnum
    fi
done

rm /tmp/twidget.$$
