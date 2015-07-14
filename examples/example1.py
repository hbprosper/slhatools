#!/usr/bin/env python
# ----------------------------------------------------------------------------
import os, sys
from ROOT import gSystem
# ----------------------------------------------------------------------------
def main():
    gSystem.Load("libslhatools")
    from ROOT import SLHAreader
    
    reader = SLHAreader("pMSSM.slha")
    reader.writeKeys()

    keys = reader.keys("35")
    for key in keys:
        BR = reader[key]
        print "%-24s\t= %10.3e" % (BR.comment, BR.value)
# ----------------------------------------------------------------------------
try:
    main()
except KeyboardInterrupt:
    print "ciao!"
    
