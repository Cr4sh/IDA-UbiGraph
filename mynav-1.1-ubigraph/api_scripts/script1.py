#!/usr/bin/env python

import os
import sys

"""
Sample script using MyNav API. Add as entry point every function
with a name like *recv*
"""

def addRecvEntryPoints():
    for f in Functions():
        strname = GetFunctionName(f)
        if strname.find("recv") > -1:
            print "Adding %s as entry point" % strname
            if not mynav.addPoint(f, "E"):
                print "Error adding %s as entry point :(" % strname

if __name__ == "__main__":
    addRecvEntryPoints()
