
      
## Inquire and set on the queue

code::

      import sys 
      import pymqi 
      from pymqi import CMQC 
      queue_manager = 'M801' 
      qmgr = pymqi.connect(queue_manager) 
      queue = pymqi.Queue(qmgr, 'CP0000',pymqi.CMQC.MQOO_INQUIRE+pymqi.CMQC.MQOO_SET) 
      inq =            queue.inquire(pymqi.CMQC.MQIA_INHIBIT_GET) 
      print("pymqi.CMQC.MQIA_INHIBIT_GET",inq) 
      inqcd =            queue.inquire(pymqi.CMQC.MQIA_CURRENT_Q_DEPTH ) 
      print("pymqi.CMQC.MQIA_CURRENT_Q_DEPTH",inqcd) 
                                                                                      

Trying to use MQSET with CMQC.MQIA_MAX_Q_DEPTH fails with Reason 2067: FAILED: MQRC_SELECTOR_ERROR. [What do you mean, I can’t set the maximum queue depth?](https://colinpaice.blog/2022/01/25/what-do-you-mean-i-cant-set-the-maximum-queue-depth/).

using MQSET with the above code

      y = queue.set(CMQC.MQIA_TRIGGER_DEPTH,3) 
      print("Set",y) 

gave

      Set None 



