# base class for pyqmi unit tests

import sys
#mport logging
import time
import platform
import pymqi
import threading
import utoptions as utoptions
from pymqi import CMQCFC
from pymqi import CMQC
from pymqi import PCF as PCF
import unittest
import myConnect # this isolates the MQ Connect from the main programs
print("14", dir(sys)    )
print("14", sys.path)

import inspect
class pymqiut(unittest.TestCase):
    def __init__(self,*args,**kwargs):
        self.name = args
        super(pymqiut,self).__init__(*args,**kwargs)
    def setUp(self):
        self.qmgr = myConnect.connect()
        self.pcf = PCF.PCF(self.qmgr,
             response_wait_interval=5000,
             convert=True);
        self.qmgr.set_debug(0)
#       self.qmgr.set_debug(1+pymqi.xCC+pymqi.xPCF)

    def tearDown(self):
        self.qmgr.disconnect()

    def checkHeader(self,hh):
        if hh[0]['Reason'] != 0:
            for h in hh:
                print(k)
            print(flush=True)
        self.assertEqual(hh[0]['Reason'],0)

    def checkMQList(self,l):
        p = self.pcf.prettify(l )
        return True

    def check(self,x,records,*args):
        kw = {}
        if len(args) > 0:
           kw = args[0]
        if len(args) > 1:
             headers = args[1]
        else:
            headers = None;
        if utoptions.print is True:
#           if headers is not None:
#               for h in headers:
#                   print("=====")
#                   # make command into string
#                   self.pcf.prettifyPCFHeader(h)
#                   print(h)
##                  for x in h:
##                      print(x,h[x])
            self.printDetails(x,kw)
        self.assertEqual(len(x),len(records)) # data and trailer
        for (item,count) in list(zip(x, records)):
            self.assertGreaterEqual(len(item),count)
        for item in x:
           self.assertTrue(self.checkMQList(item))
    # Write to the console and write to the html file
    def printDetails(self,x,kw):
        i = 0
        fn = self.name[0][5:]

        # the test is test_inq_cfstatus_summary, so map this to inq_cfstatus_summary
        # and use this as the file name
        # take the dict passed in as the parameters
        pad = "" # so we can have a=b,c=d  and put the comma in
        kwlist = ""
        print("Output from:" + fn)
        for k in kw:
             if isinstance(kw[k],int):
                v = str(kw[k])
             else:
                v = '"' + str(kw[k])  + '"'
             kwlist += pad+ k + '=' +  v
             pad = ","
        with open('list.html', 'a') as fi      :
#           fi.write('<p>' + fn +'(')
            fi.write('<p><a href=./'+fn+'.html>result=pcf.'+fn+'(')
            fi.write(kwlist)
            fi.write(')</a></p>\n')
        with open(fn+'.html', 'w') as f:
            f.write('<html>\n')
            f.write('<h1>Output from:' + fn +'(')
            f.write(kwlist)
#           for k in kw:
#               f.write(k +'="'  +str(kw[k])+'"')
            f.write(') </h1>\n')
            sizex = len(x)
            for xx in x:
                i = i + 1
                z = self.pcf.prettify(xx  )
                f.write("<h3>Record " + str(i)  + ", with " + str(len(z))  + " elements</h3>")
                f.write('<table>\n')
                print("=======Record:",i,"/",sizex,"==========")
                print(">>>Number of entries:",len(z))
                for a in z:
                    value = z[a]
                    if not isinstance(value,list ):
                         value = (value,) # make it a tuple
                    for v in value:
                        aout = str(v)
                        print(a,":",aout)
                        f.write('<tr><td>'+a+'</td>\n    <td>'+aout+'</td></tr>\n')
                f.write('</table>\n')
#               f.write('<tr><td> </td>\n    <td> </td></tr>\n')
#               print(">>>",len(z))
#               for a in z:
#                   aout = str(z[a])
#                   print(a,":",aout)
#                   f.write('<tr><td>'+a+'</td>\n    <td>'+aout+'</td></tr>\n')
                f.write('</table>\n')
#               f.write('<tr><td> </td>\n    <td> </td></tr>\n')
#           write an empty line after each record
            f.write('</html>\n')
            print(flush=True)

