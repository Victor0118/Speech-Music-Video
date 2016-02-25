__author__ = 'Administrator'

import os
from melody_feature import *
from lshash import LSHash
import re
import pickle
import time
pattern = re.compile(r'.*mid')

# hash_len = 100
# dm = 10
# lsh = LSHash(hash_len, dm)

# path = os.getcwd()
# parent_path = os.path.dirname(path)
# now_path = parent_path+"/dataset"
# # print now_path
# files = os.listdir(now_path)

# i = 1
# for f in files:
#     if i == 80:
#         break
#     i = i+1
#     if (os.path.isfile(now_path+'/'+f)):
#         if f[0]=='B':
#             break;
#         if pattern.match(f):
#             print f
#             try:
#                 for note,name in note_from_midi(now_path+'/'+f):
#                     # print 'dfsa'
#                     lsh.index(note,extra_data=(name,0.8))
#             except Exception, e:
#                 pass
#                 #print "exception"
#                 # raise
#             else:
#                 pass
#             finally:
#                 pass
# output =  open('lsh.model','w')   
# pickle.dump(lsh,output)
# del lsh




#############################################
#test
time1= time.time()
myinput = open('lsh.model','r')
lsh = pickle.load(myinput)
kk = []
i = 0
result = {}
time2= time.time()
print 'load time:',str(time2-time1)
for note,name in nlsh('Untitled5.m4a'):
    q = note
    kk.extend(q)
    print 'test'
    r = lsh.query(q)
    print '--------'+str(i)+'-----------'
    i += 1
    if(len(r) > 0):
        print len(r)
        nn = min(3,len(r))
        for k in range(nn):
            w = r[k][1]
            name = r[k][0][1][0]
            if(not result.has_key(name)):
                result[name] = 0.0
            else:
                w *= 0.93
            result[name] += w
            print r[k][0][1],r[k][1]

candidates = sorted(result.items(), lambda x, y: cmp(x[1], y[1]))
print '----------------------------'
print candidates

