#!/bin/sh

# user2alph
#   Replaces the logins all users in the system that have numbers at the end to
#   have letters at the end instead, so that test01, test02, test27 become
#   test_A, test_B, testAA, etc.

PROGNAME=$(basename $0)

USER_ID_MIN=1000    # Starts searching from this UID upwards
HOME_DIR="/home"

DATE_FORMAT="+%y-%m-%d-%T"
LOG_FILE="user2alph.$(date $DATE_FORMAT).log"

show_usage() {
    echo $1 1>&2
    echo "Usage: $PROGNAME" 1>&2
    exit ${2:-1}
}

int_to_char() {
    int=$(echo "($1-1) % 26" | bc)
    char=$(echo $int   | sed "s/25/Z/" | sed "s/24/Y/" | sed "s/23/X/" | \
         sed "s/22/W/" | sed "s/21/V/" | sed "s/20/U/" | sed "s/19/T/" | \
         sed "s/18/S/" | sed "s/17/R/" | sed "s/16/Q/" | sed "s/15/P/" | \
         sed "s/14/O/" | sed "s/13/N/" | sed "s/12/M/" | sed "s/11/L/" | \
         sed "s/10/K/" | sed "s/9/J/"  | sed "s/8/I/"  | sed "s/7/H/"  | \
         sed "s/6/G/"  | sed "s/5/F/"  | sed "s/4/E/"  | sed "s/3/D/"  | \
         sed "s/2/C/"  | sed "s/1/B/"  | sed "s/0/A/")
    echo $char
}

move_user() {
    echo "usermod -l $2 $1"
    echo "groupmod -n $2 $1"
    echo "usermod -md $HOME_DIR/$2 $1"
    echo "$1 : $2" >> $LOG_FILE
}

if [ $(id -u) -ne 0 ]; then
    show_usage "$PROGNAME: Must be run as root." 1
fi

user_list=""

regex="^([^0-9]*)([0-9]+)$"

IFS='
'
for line in $(cat /etc/passwd); do
    user_name=$(echo $line | cut -d: -f1)
    user_id=$(echo $line | cut -d: -f3)
    
    if [ $user_id -ge $USER_ID_MIN ]; then
        if echo $user_name | grep -Eq "$regex"; then
            user_list="$user_list$user_name "
        fi
    fi
done

IFS=' '
for user in $user_list; do
    base_name=$(echo $user | sed -r "s/$regex/\1/")
    num_at_end=$(echo $user | sed -r "s/$regex/\2/")
    
    alpha_append=""
    
    i=$num_at_end
    while [ $i -gt 0 ]; do
        alpha_append="$(int_to_char $num_at_end)$alpha_append"
        
        if [ $(echo "$i % 26" | bc) -eq 0 ]; then
            i=$(expr $i - 26 || true)
        fi
        
        i=$(expr $i / 26 || true)
    done
    
    if [ $num_at_end -lt 26 ]; then
        alpha_append="_$alpha_append"
    elif [ $num_at_end -eq 0 ]; then
        alpha_append="_a"
    fi
    
    new_name="$base_name$alpha_append"
    if ! grep -q $new_name /etc/passwd; then
        move_user $user $new_name 
    else
        echo "error : <$user not renamed, $new_name already exists>" >> \
            $LOG_FILE
    fi
done

chmod 400 $LOG_FILE
echo "Changes written to $LOG_FILE" 1>&2
