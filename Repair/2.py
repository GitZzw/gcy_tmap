import os
import string
fr = open('/home/hjk/all.txt','r')
i = 0
line = fr.readline()
while(line):
    path = '/home/hjk/reload/'+str(i)+'.txt'
    f = open(path,'w')
    f.write(line)
    f.close()
    i = i+1
    line = fr.readline()

fr.close()