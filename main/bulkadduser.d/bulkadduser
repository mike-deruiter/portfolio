#!/usr/bin/python3

# bulkadduser
# Usage: bulkadduser [FILES...]
# Take a list of real names of the format "[FIRSTNAME ][MIDDLENAME ]LASTNAME" &
#   create list of usernames of the format "lastfmXX", where XX is a number
#   greater than or equal to 01. Then, add the users to the system. Usernames
#   are guaranteed not to conflict w/ pre-existing users or each other.

# NOTE: Works perfectly on Red Hat systems, but when tested on Ubuntu the
#       useradd command seems to work differently (doesn't add home
#       directories, etc.)

DEBUG = True

import os, sys, subprocess, pwd, string, re, fileinput, random

# Function defintions ##########################################################

def show_usage(message, code=1):
    print(message)
    print("Usage: %s file" % (sys.argv[0]))
    sys.exit(code)
    
def shell(command):
    if not DEBUG:
        proc = subprocess.Popen(command, shell=True)
        proc.wait()
        return proc.returncode
    else:
        print(command)
        return 0
    
def gen_name_list():
    '''gen_name_list
    
    Read the files specified as arguments & create a list of names for further 
    processing.
    '''
    names = []
    
    try:
        for line in fileinput.input():
            # Compress whitespace, strip leading & trailing whitespace, remove
            # all non-alphabetic, non-whitespace characters & convert to
            # lowercase        
            line = re.sub('\s+', ' ', line)
            line = line.strip()
            line = re.sub('[^a-zA-Z ]', '', line)
            line = line.lower()
            # Add to list
            names.append(line)
    except IOError:
        show_usage("%s: Invalid filename in arguments" % (sys.argv[0]))
    
    list.sort(names)
    
    return names
    
def random_passwd():
    '''random_passwd
    
    Return an random 8-character string of alphanumeric characters
    '''
    return ''.join( random.choice(string.ascii_lowercase + 
                                  string.ascii_uppercase +
                                  string.digits) for _ in range(8) )

def append_number(name, number):
    '''append_number
    
    Replace the number at the end of a string w/ a new number
    '''
    name = re.sub("\d+$", "", name)
    
    if number < 10:
        name += "0"
    name += str(number)
    
    return name
    
def resolve_username(name):
    '''resolve_username
    
    Keep checking if a username exists, incrementing the number at the end by
    1 each time, until a novel login is found
    '''
    re_number_at_end = re.compile(r'^[^\d]*(\d+)$')
    while True:
        if not name in formatted_names:
            try:
                pwd.getpwnam(name)
            except KeyError:
                return name
            
        duplicate_count = int(re_number_at_end.match(name).group(1)) + 1
        name = append_number(name, duplicate_count)
            
# Start of script ##############################################################

PROGNAME = os.path.split(sys.argv[0])[1]

if os.geteuid() != 0:
    show_usage("%s: must be run as root." % (PROGNAME))
    
if len(sys.argv) == 1:
    show_usage("%s: 1 or more arguments required; you supplied 0." % (PROGNAME))
    
names = gen_name_list()
formatted_names = []

duplicate_count = 1
curr = ""
prev = ""

for user in names:
    re_username = re.compile(r'^((\w)\w* )?((\w)\w* )?((\w)\w* )*(\w{1,4})\w*$')
    
    rematch_username = re_username.match(user)
    
    if not rematch_username:
        print("%s : <invalid name>" % (user))
        continue
    
    name = rematch_username.group(7) # Last name
    if rematch_username.group(2): # First initial
        name += rematch_username.group(2)
    if rematch_username.group(4): # Middle initial
        name += rematch_username.group(4)
        
    curr = name
    
    if curr == prev:
        duplicate_count += 1
    else:
        duplicate_count = 1
        
    name = append_number(name, duplicate_count)
    name = resolve_username(name)
    
    formatted_names.append(name)
    prev = curr
    
for user in formatted_names:
    if shell("useradd %s 2>/dev/null" % (user)) == 0:
        randompw = random_passwd()
        
        if shell("echo %s:%s | chpasswd" % (user, randompw)) == 0:
            print("%-8s : %s" % (user, randompw))
        else:
            print("%-8s : <error initializing password>" % (user))
    else:
        print("%-8s : <error adding user>" % (user))
