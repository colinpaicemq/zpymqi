## Opening and closing a queue

Pymqi has a lazy open.  If you are just getting or putting a message to a queue then it will open the queue when you put or get the message.
For example

code::

      qmgr = pymqi.connect("CSQ9")
      queue = pymqi.Queue(qmgr, 'CP0000')
      message = queue.put("COLINSMESSAGE")

This will open the queue for put.

code::

      qmgr = pymqi.connect("CSQ9")
      queue = pymqi.Queue(qmgr, 'CP0000')
      message = queue.put1("COLINSMESSAGE")

Will do an MQPUT1.

## You can explicitly open the queue.

code::

      queue2= pymqi.Queue(qmgr, 'CP0000', pymqi.CMQC.MQOO_INPUT_SHARED +   
                                          pymqi.CMQC.MQOO_INQUIRE+
                                          pymqi.CMQC.MQOO_SET) 
      minq = queue2.inquire(pymqi.CMQC.MQIA_INHIBIT_GET) 
      print("inquire ",minq)
      mset = queue2.set(pymqi.CMQC.MQIA_INHIBIT_GET,0 ) 
      print("set",mset)

gives

> inq 0      
> set None

Where the 0 is MQQA_GET_ALLOWED (and 1 would be MQQA_GET_INHIBITED).

If you get a non zero return code an exception is thrown, for example
> pymqi.MQMIError: MQI Error. Comp: 2, Reason 2035: FAILED: MQRC_NOT_AUTHORIZED QM=CSQ9     

## Close the queue

code::

      queue2.close()