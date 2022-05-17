## Getting a message from a queue
### Simple get 

code::

      import pymqi 
      from pymqi import CMQCFC 
      from pymqi import CMQC 
      queue_manager = 'M801' 
      qmgr = pymqi.connect(queue_manager)
 
      q1   = pymqi.Queue(qmgr, 'COLIN') 
      data= q1.get() 
      print(data) 

This prints *b'Message from Python'*.
Rerunning it produces

    Traceback (most recent call last):                                                                                                  
      File "get.py", line 7, in <module>                                                                                                
        data= q1.get()                                                                                                                  
      File "/u/betacp/pymqi/__init__.py", line 2219, in get                                                                             
        raise MQMIError(rv[-2], rv[-1], QName=self.__QName, message=rv[0], original_length=rv[-3],                                      
    pymqi.MQMIError: MQI Error. Comp: 2, Reason 2033: FAILED: MQRC_NO_MSG_AVAILABLE QName=COLIN message= original_length=0 Verb=MQGET   

## Different data types
On the previous page was the example *q1.put('Message from Python')*.  This puts a character string.

When the data is got, it is *b'Message from Python'* a byte string


### More complex get with MD and GMO
Create a default md and gmo

      md = pymqi.MD()
      gmo = pymqi.GMO()

You can do 
- a simple get:  m = q1.get(None)  
- a get and specify an MD :  m = q1.get(None, md)
- a get with MD and GMO: m = q1.get(None, md, gmo)
- or a get with a GMO and no md: m = q1.get(None,None,gmo)


code::

      import pymqi 
      from pymqi import CMQCFC 
      from pymqi import CMQC 
      queue_manager = 'M801' 
      qmgr = pymqi.connect(queue_manager) 
      q1   = pymqi.Queue(qmgr, 'COLIN') 
      gmo = pymqi.GMO() 
      gmo.Options = pymqi.CMQC.MQGMO_WAIT | pymqi.CMQC.MQGMO_FAIL_IF_QUIESCING| pymqi.CMQC.MQGMO__NO_SYNCPOINT 
      gmo.WaitInterval = 10 * 1000 # 10 seconds 
      try: 
          m = None 
          md = pymqi.MD() 
          print("Line 14",md['PutApplName']) 
          m = q1.get(None,md, gmo) 
          print("Message",m) 
          print("Line 18",md['PutApplName']) 
      except pymqi.MQMIError as e: 
          print(e) 

This produced

      Line 14 b''                                        
      Message b'Message from Python'                     
      Line 18 b'BETACP3                     '                  
After the MQ request, the returned control blocks are what MQ returned. The above example shows the  PutApplName information before and after the MQGET request.
