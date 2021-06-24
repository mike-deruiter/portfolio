#!/bin/sh

# bulkadduser 
#   Takes a list of files containing proper names in the format 
#   "[FIRST ][MIDDLE ]LAST" & adds users to the system with random passwords &
#   usernames in the format "lastfmXX"

set -eu

HOME_DIR="/home"

PROGNAME=$(echo $0 | sed s#^.*/##)

DATE_FORMAT="+%y-%m-%d-%T"
USERNAMES_FILE="/tmp/bulkadduser.usernames.$(date $DATE_FORMAT)"
NAMELIST_FILE="/tmp/bulkadduser.name_list.$(date $DATE_FORMAT)"
LOG_FILE="bulkadduser.$(date $DATE_FORMAT).log"

# show_usage - print an error message, note on usage & exit
show_usage() {
    echo $1 1>&2
    echo "Usage: $PROGNAME FILE [FILES...]" 1>&2
    exit ${2:-1}
}

# gen_name_list - format the list of names
gen_name_list() {
    for file in $NAMES_FILES; do
        if [ ! -f $file ]; then
            echo "error : < not a file: $file >" >> $LOG_FILE
            continue
        fi
        
        sed -r 's/[[:blank:]][[:blank:]]*/ /g' $file | \
        sed -r 's/[^a-zA-Z ]//g' | tr '[:upper:]' '[:lower:]' | \
        sed -r 's/^[[:space:]]*?//g' | sed -r 's/[[:space:]][[:space:]]*$//' > \
            $NAMELIST_FILE
    done
}

# replace_number - replace the number at the end of the username
replace_number() {
    username=$(echo $username | sed 's/[[:digit:]]//g')
    
    if [ $dup_count -lt 10 ]; then
        username=$(echo $username'0')
    fi
    username=$(echo $username$dup_count)
}

# resolve_username - find a unique login for this user
resolve_username() {
    while true; do
        if ! grep -q $username $USERNAMES_FILE; then
            if ! grep -q $username /etc/passwd; then
                return
            fi
        fi        
        number_at_end=$(echo $username | sed -r \
                    's/^.*([[:digit:]][[:digit:]]+)/\1/')
          
        dup_count=$(expr $number_at_end + 1 || true)         
        replace_number
    done
}

# 
find_line() {
    awk 'NR == '$1' { print; }' $NAMES_FILE
}

if [ "$(id -u)" -ne 0 ]; then
    show_usage "$PROGNAME: must be run as root." 1
fi

if [ "$#" -lt 1 ]; then
    show_usage "$PROGNAME: 1 or more arguments required; you supplied $#." 2
fi

NAMES_FILES=$(echo "$*")

gen_name_list

touch $USERNAMES_FILE

dup_count=1
line=1
curr=""
prev=""

# Build the Regex
regex='^(([a-z])[a-z]* )?'          # First Name
regex=$regex'(([a-z])[a-z]* )?'     # Middle Name
regex=$regex'(([a-z])[a-z]* )*'     # Further Middle Names
regex=$regex'([a-z]{1,4})[a-z]*$'   # Last Name

IFS='
'
for name in $(cat $NAMELIST_FILE); do


    if ! echo $name | grep -Eq "$regex"; then
        echo "error : <invalid name: $(find_line $line)>" >> $LOG_FILE
        continue
    fi
    
    name=$(echo $name | sed -r "s/$regex/\7\2\4/")
    
    curr=$name
    
    if [ "$curr" = "$prev" ]; then
        dup_count=$(expr $dup_count + 1 || true)
    else
        dup_count=1
    fi
    
    username=$name
    
    replace_number
    resolve_username
    
    echo $username >> $USERNAMES_FILE
    
    prev=$curr
    line=$(expr $line + 1 || true)
done

for user in $(cat $USERNAMES_FILE | sort); do
    rand_passwd=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 8 | head -1)
    # For Demonstration Purposes - Don't execute the commands
    echo "useradd $user"
    echo "echo $user:$rand_passwd | sudo chpasswd"
    echo "$user : $rand_passwd" >> $LOG_FILE
done

chmod 400 $LOG_FILE
echo "Changes written to $LOG_FILE" 1>&2

rm $USERNAMES_FILE $NAMELIST_FILE
