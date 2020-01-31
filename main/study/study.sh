#!/bin/sh

set -eu

PROGNAME=$(basename $0)

#FLASHCARD_DIR=$HOME/Documents/Flashcards
FLASHCARD_DIR="."

# show_usage - print an error message, note on usage & exit
show_usage() {
    echo $1 1>&2
    echo "Usage: $PROGNAME FLASHCARDS [FLASHCARDS...]" 1>&2
    exit ${2:-1}
}

# pick - adapted from THE UNIX PROGRAMMING ENVIRONMENT
#        by Brian Kernighan & Rob Pike
pick() {
    for i in "$@"
    do
        echo -n "$i (y/n)? " >/dev/tty
        read response
        case $response in
            y*) echo $i        ;;
            n*)                ;;
            *)  echo error     ;;
        esac
    done </dev/tty
}

if [ $# -lt 1 ]; then
    show_usage "$PROGNAME: Please specify flashcard set(s) to study." 1
fi

# gather all the files containing cards to study
files="/tmp/cards.$$"
for i in $@; do
    if [ ! -e $FLASHCARD_DIR/$i ]; then
        show_usage "$PROGNAME: Flashcards not found." 2
    fi
    tail -n +2 $FLASHCARD_DIR/$i >> $files
done

# append title to temporary deck
if [ $# -eq 1 ]; then
    head -1 $FLASHCARD_DIR/$1 > /tmp/shuffle.$$
else
    echo Multiple Flashcards > /tmp/shuffle.$$
fi

# shuffle deck & store in temporary deck
cat $files | awk \
    'BEGIN { RS = "@@"
             FS = "\n"
             num_cards = 0                                              }
           { card_titles[num_cards] = $1
             card_data[num_cards]   = $2
             for (i = 3; i < NF; ++i) {
                card_data[num_cards] = card_data[num_cards] "\n" $i
             }
             num_cards++                                                }
     END   { for (i = 1; i < num_cards; ++i) {
                 r = (int(rand() * 10) % (num_cards-1))+1
                 
                 temp = card_titles[i] 
                 card_titles[i] = card_titles[r]
                 card_titles[r] = temp
                 
                 temp = card_data[i]
                 card_data[i] = card_data[r]
                 card_data[r] = temp
             }
             for (i = 1; i < num_cards; ++i) {
                 printf "@@";
                 print card_titles[i];
                 print card_data[i];
             }                                                          }' \
>> /tmp/shuffle.$$

NUM_CARDS=$(grep -c ^@@ /tmp/shuffle.$$)
num_correct=0
incorrect="no"

# print title of deck
head -1 /tmp/shuffle.$$
echo

i=$NUM_CARDS
while [ $i -gt 0 ]; do
    # print flashcard title & wait for user to press enter
    flashcard=`awk 'BEGIN { RS = "@@";
                            FS = "\n"; }
                    NR == ('$NUM_CARDS' - '$i' + 2) { printf "%s", $1 }' \
               < /tmp/shuffle.$$`
    echo -n $flashcard "(Press enter) "
    read cont
    
    # print flashcard answer
    awk 'BEGIN { RS = "@@";
                 FS = "\n"; 
                 printf "\n"}
         NR == ('$NUM_CARDS' - '$i' + 2) { for (i = 2; i < NF; ++i) {
                                               print $i
                                           }                          }' \
         < /tmp/shuffle.$$ | fmt -s -w 80          
    
    # ask user if answer is correct (only accept 'y' or 'n')
    err="err"
    correct="err"
    while [ "$err" = "err" ]; do
        echo -n '>> '
        correct=$(pick correct)
        
        if [ "$correct" != "error" ]; then 
            err="none"
        fi
    done
    
    # if correct,   add to $num_correct
    # if incorrect, save title of flashcard to display at end
    if [ "$correct" = "correct" ]; then
        num_correct=$(expr $num_correct + 1)
    else
        echo @@$flashcard >> /tmp/wrong.$$
        incorrect="yes"
    fi
    
    echo
    
    # increment loop index. if the result is 0, the exit code is 1, so since we
    # have set -e on we write "|| true" to give it a 0 exit status
    i=$(expr $i - 1 || true)
done


# print titles of flashcards user missed
if [ "$incorrect" = "yes" ]; then    
    echo Missed Questions:
    cat /tmp/wrong.$$ | cut -d@ -f 3 | sed "s/^/>> /"
    echo
fi

# display percentage
PERCENTAGE=$(echo "scale=2; ($num_correct / $NUM_CARDS) * 100" | bc | \
             cut -d. -f 1,1)
echo Correct: $PERCENTAGE%

touch /tmp/wrong.$$
rm /tmp/cards.$$ /tmp/shuffle.$$ /tmp/wrong.$$ # clean up
