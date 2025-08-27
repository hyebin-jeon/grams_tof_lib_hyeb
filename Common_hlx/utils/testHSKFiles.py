#!/usr/bin/python3 -Wall
#
# This file is to test RAW HSK packet outputs vs the ROOT wrapper of them (which include timestamps) and other goodies
#
#

import sys
import pprint
import argparse
import datetime
import dateutil
import dateutil.parser as dp
from datetime import datetime
from datetime import timezone
from array import array
from colorama import *
from ctypes import *

# set up a global pretty printer
pp = pprint.PrettyPrinter(indent=4)

import ROOT
ROOT.gSystem.Load('./Common/libHelixCommonLib.so')  # load shared lib
#from ROOT import HLX_SFC_HSK_Packet

def main():

    if len(sys.argv) < 2:
        print("Usage: testHSKFiles.py rootFile rawFile")
        print("   ex: testHSKFiles.py  test1.root test1.dat")
        return -1

    rootFilename = sys.argv[1]
    rawFilename = sys.argv[2]

    #######################################################################################
    ipf = ROOT.TFile(rootFilename,'READ')
    if ipf.IsOpen is False:
        exit(-1)

    # should be ok by here...
    t = ipf.Get('t')

    rootData = []
    if(t is not None):
        N = t.GetEntries()
        print("Opened tree with ", N, " entries")
        for n in range(0, N):
            t.GetEntry(n)
            pack = getattr(t, 'pack', None)
            rvect = pack.exportAsSerialData()
            vect  = array('B', rvect.data())
            for i in range(0, len(vect)):
                rootData.append(vect[i])
                #print("{} => 0x{:02X}".format(i, vect[i]))


    print("ROOT Length:", len(rootData))
    #######################################################################################
    # Read the raw
    #######################################################################################
    rawFP = open(rawFilename, "rb")
    rawData = rawFP.read()
    rawCount = len(rawData)
    print("Raw Length :", rawCount)
    #for n in range(0,8):
    #    print("{} => 0x{:02X}".format(n, rawData[n]))
    rawFP.close()

    mismatchCount = 0
    for n in range(0, rawCount):
        if rawData[n] != rootData[n]:
            print("Mismatch! {}".format(n, rawData[n], rootData[n]))
            mismatchCount += 1

    print("There were {} mismatches in {} entries".format(mismatchCount, rawCount))


if __name__== "__main__":
    main()
