#!/usr/bin/ruby

require 'etc'

# bulkadduser
# Usage: bulkadduser [FILES...]
# Take a list of real names of the format "[FIRSTNAME ][MIDDLENAME ]LASTNAME" &
#   create list of usernames of the format "lastfmXX", where XX is a number
#   greater than or equal to 01. Then, add the users to the system. Usernames
#   are guaranteed not to conflict w/ pre-existing users or each other.

# Notes: Works perfectly on Red Hat systems, but when tested on Ubuntu the
#        useradd command seems to work differently (doesn't add home
#        directories, etc.)

#TODO: Error-check if file is invalid
#TODO: Check if user is root
#TODO: When there's an error adding a user, print it to stderr

PROGNAME = File.basename($0)

################################################################################
# Methods
################################################################################

# show_usage
# Print a note on usage & exit.
# Arguments:
#   message - message to display.
#   code    - code to exit with.
def show_usage(message, code=1)
    puts message
    printf("Usage: %s [FILES...]", PROGNAME)
    exit(code)
end

# gen_name_list
# Generate a list of real names (not usernames) in the following format:
#     * alphabetic characters only
#     * characters converted to lowercase
#     * strings of whitespace compacted to single space
#     * leading & trailing whitespace removed
# Arguments:
#   n/a
def gen_name_list
    names = Array.new

    ARGF.each do |line|
        line.sub!(/[^a-zA-Z ]/, '')
        line.downcase!
        line.sub!(/\s+/, ' ')
        line.strip!
        names.push(line)
    end

    return names
end

# random_passwd
# Generate a random password in the following format:
#     * 8 characters long
#     * Alphanumeric characters only (mixed case)
# Arguments:
#   n/a
def random_passwd
    [*('A'..'Z'), *('a'..'z'), *('0'..'9')].sample(8).join
end

class String
    # append_number
    # Remove number at end of string & replace it w/ $duplicate_count
    # Arguments:
    #   n/a
    def append_number
        self.sub!(/(\d+)$/, "")

        if $duplicate_count < 10
            self << '0'
        end

        self << $duplicate_count.to_s    
    end
    
    # resolve
    # Change string to novel username, following the established
    #   naming conventions
    # Arguments:
    #   n/a
    def resolve
        while true
            if not $formatted_names.include? self
                begin
                    Etc.getpwnam(self)
                rescue
                    return self
                end
            end
            # user already exists, either in $formatted_names or the system
            self =~ /(\d+)$/
            $duplicate_count = $1.to_i + 1

            self.append_number
        end
    end
end

################################################################################
# Start of script
################################################################################

names = gen_name_list   # function can be rewritten to pull from database, etc.

$formatted_names = Array.new
$duplicate_count = 1
curr = ""
prev = ""

names.each do |name|
    username_re = /^((\w)\w* )?((\w)\w* )?(\w{1,4})\w*$/
    next if not name =~ username_re

    # Note to self: should I avoid these Perlisms?
    name = $5           # Last name, or only name
    name += $2 if $2    # First initial
    name += $4 if $4    # Middle initial
    
    curr = name
    
    if curr == prev
        $duplicate_count += 1
    else
        $duplicate_count = 1
    end
    
    name.append_number
    name.resolve

    $formatted_names.push(name)
    prev = curr
end

$formatted_names.each do |username|
    if system("useradd #{username} 2>/dev/null")
        randompw = random_passwd
        
        if system("echo #{username}:#{randompw} | chpasswd")
            printf("%-8s : %s\n", username, randompw)
        else
            printf("%-8s : <error adding user>\n", username)
        end
    else
        printf("%-8s : <error adding user>\n", username)
    end
end
