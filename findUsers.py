#!/usr/bin/env python3

import subprocess

keys = ["Authenticated","closed"]

def findUsers():
    inFile = r"/var/log/mumble-server/mumble-server.log"
    message = "\
                   /´¯/)\n\
                  /¯  / \n\
                 /   /\n\
            /´¯/'   '/´¯¯`·¸\n\
         /'/   /   ./      /¨¯\ \n\
        ('(   ´    ´.   ¯~/'  ') \n\
         \                '   / \n\
          ''   \          _.·´ \n\
            \            ( \n\
             \            \ "        

    # Open log-file
    with open(inFile) as f:
        f = f.readlines()

    log = getLog(f)

    #for key, value in log.items():
    #    print(key, " ", value)

    # Prints Users.
    print("\n--- Current users online-----------")
#    print("-------",printTime(),"------\n")
    noOnline = True 
    for key, value in log.items():
        if value:
            if noOnline:
                noOnline = False
            print(key)
            
    if log == {} or noOnline:
        print(message)
        print("No Pokémon in the bag")
    print("--------------------------------------")

def getStartPos(pos, str):
    if str[pos] == ":":
        return pos + 1
    else:
        return getStartPos(pos + 1, str)

def getName(pos, str):
    if str[pos] == "(":
        return ""
    else:
        return str[pos] + getName(pos + 1, str)

def getLog(_file):
    _dict = {}
    for line in _file:
        if keys[0] in line:
            start = getStartPos(34, line)
            name = getName(start, line)
            _dict[name] = True
        elif keys[1] in line:
            start = getStartPos(34, line)
            name = getName(start, line)
            _dict[name] = False
    return _dict

def printTime():
    cmd = ["date", "+%A %W %Y %X"]
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    output =  proc.stdout.read().rstrip()
    str = output.decode('ascii')
    return str

findUsers()
