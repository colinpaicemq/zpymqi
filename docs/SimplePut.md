

## Putting a message to a queue

code::

      import pymqi 
      from pymqi import CMQCFC 
      from pymqi import CMQC 
      queue_manager = 'M801' 
      qmgr = pymqi.connect(queue_manager)
 
      q1   = pymqi.Queue(qmgr, 'COLIN') 
      q1.put('Message from Python') 

If the queue COLIN did not exist, an exception is thrown

      Traceback (most recent call last):                                                                       
        File "put.py", line 7, in <module>                                                                   
          q1.put('Message from Python')                                                                       
        File "/u/betacp/pymqi/__init__.py", line 2130, in put                                                 
        self.__realOpen()                                                                                   
          File "/u/betacp/pymqi/__init__.py", line 2033, in __realOpen                                          
          raise MQMIError(rv[-2], rv[-1], QName=qName)                                                        
      pymqi.MQMIError: MQI Error. Comp: 2, Reason 2085: FAILED: MQRC_UNKNOWN_OBJECT_NAME QName=COLIN 



## Putting a message with an MD      

code::

      md  = pymqi.MD() 
      md["Persistence"] = pymqi.CMQC.MQPER_NOT_PERSISTENT 
      md["Format"] = b'MQSTR' 

      q1.put('Message from Python',md)  

## Putting a message with an MD and a PMO
code:

      md = pymqi.MD()
      pmo = pymqi.PMO() 
      md["Persistence"] = pymqi.CMQC.MQPER_NOT_PERSISTENT 
      md["Format"] = b'MQSTR' 

      q1.put('Message from Python',md,PMO) 

### Put with message properties

See PUT with message properties below.  
