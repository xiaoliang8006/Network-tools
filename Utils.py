# -*- coding: utf-8 -*-
import sys
import os
import warnings
warnings.filterwarnings('ignore')

# log
class Logger(object):
    def __init__(self, fileN="Default.log"):
        self.terminal = sys.stdout
        self.log = open(fileN, "a+")
    def write(self,message):
        self.terminal.write(message)
        self.log.write(message)
    def flush(self):
        pass
#日志输出到result.txt
#sys.stdout = Logger("result.txt")

#批量获取文件夹下子文件名
class List_file(object):
    def findfile(self,filepath):
        pathdir = os.listdir(filepath)
        for s in pathdir:
            newdir = os.path.join(filepath, s)
            if os.path.isfile(newdir):
                self.soundfile.append(newdir)
            elif os.path.isdir(newdir):
                self.findfile(newdir)

    def eachfile(self,filepath):
        self.soundfile = []
        self.findfile(filepath)
        self.soundfile.sort()
        return self.soundfile
