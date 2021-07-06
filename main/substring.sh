#!/bin/sh

# substring - Returns a slice of a string. Takes three arguments: the string to 
#             slice, the starting position to take the snippet from (starting 
#             from 0), & the length of the snippet.
#
#             Ex: "snippet abcde 0 3" -> "abc"

set -eu

MAX_CAPTURE_GROUPS=9

string=$1  
starting_pos=$(expr $2 - 1)   
snippet_length=$3

cap_string=""
ret_string=""

output=""

outer_loop=$(expr $snippet_length / $MAX_CAPTURE_GROUPS + 1)
end_seq=9

word_length=$(echo $string | wc -c)

if [ $starting_pos -lt "0" ]; then
    starting_pos=0
fi

last_char=$(expr $(expr $starting_pos + $snippet_length || true) + 1 || true)

if [ $last_char -gt $word_length ]; then
    length_temp=$(expr $(expr $word_length - $starting_pos || true) - 1 \
                  || true)
else
    length_temp=$snippet_length
fi

for i in $(seq 1 $outer_loop); do
    if [ "$length_temp" -lt "0" ]; then
        break
    fi

    if [ "$length_temp" -lt "9" ]; then
        end_seq=$length_temp
    fi

    for j in $(seq 1 $end_seq); do
        cap_string=$cap_string"(.)"
        ret_string="$ret_string\\$j"
    done

    output="$output$(echo $string | \
                     sed -E s/^.{$starting_pos}$cap_string.*/$ret_string/)"

    starting_pos=$(expr $starting_pos + 9 || true)
    length_temp=$(expr $length_temp - 9 || true)

    cap_string=""
    ret_string=""
done

echo $output
