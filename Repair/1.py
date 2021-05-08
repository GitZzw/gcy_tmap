import os
import string
temp = list()
for i in range(139):
    path = '/home/hjk/reload_copy/'+str(i)+'.txt'
    f = open(path,'r')
    temp.append(f.read())
    f.close()
fw = open('/home/hjk/all.txt','w')
for i in temp:
    fw.write(i)
fw.close()