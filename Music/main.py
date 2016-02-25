__author__ = 'Administrator'

import os
from melody_feature import *
from lshash import LSHash

hash_len = 10000
dm = 10
lsh = LSHash(hash_len, dm)

path = os.getcwd()
parent_path = os.path.dirname(path)
now_path = parent_path+"\dataset"
files = os.listdir(now_path)

for f in files:
    if (os.path.isfile(now_path+'\\'+f)):
        for note,name in note_from_midi(now_path+'\\'+f):
            lsh.index(note,extra_data=(name,0.8))

# kk = []
# i = 0
# result = {}
# for note,name in nlsh('test.wav'):
#     q = note
#     kk.extend(q)
#     r = lsh.query(q)
#     print '--------'+str(i)+'-----------'
#     i += 1
#     if(len(r) > 0):
#         print len(r)
#         nn = min(3,len(r))
#         for k in range(nn):
#             w = r[k][1]
#             name = r[k][0][1][0]
#             if(not result.has_key(name)):
#                 result[name] = 0.0
#             else:
#                 w *= 0.93
#             result[name] += w
#             print r[k][0][1],r[k][1]

# candidates = sorted(result.items(), lambda x, y: cmp(x[1], y[1]))
# print '----------------------------'
# print candidates

