# MQGET Operations

## Simple GET from a queue

*In this example, we are getting one message from our queue.*

```py
import pymqi 

qmgr_name = 'QM01' 
my_queue = 'COLIN'

qmgr = pymqi.connect(qmgr_name)  # connect to the qm

my_q = pymqi.Queue(qmgr, my_queue) # create handle for our queue 

data = my_q.get() # get the message off the queue

print(data) 
```

This prints `b'Message from Python'`.

But what if that was the only message on the queue? Let's run that code again.

```text
Traceback (most recent call last):                                                                                                  
File "get.py", line 7, in <module>                                                                                                
      data= q1.get()                                                                                                                  
File "/u/betacp/pymqi/__init__.py", line 2219, in get                                                                             
      raise MQMIError(rv[-2], rv[-1], QName=self.__QName, message=rv[0], original_length=rv[-3],                                      
pymqi.MQMIError: MQI Error. Comp: 2, Reason 2033: FAILED: MQRC_NO_MSG_AVAILABLE QName=COLIN message= original_length=0 Verb=MQGET   
```

### Handling no more messages on a queue

*A simple try/except clause can be used to handle an empty queue.*

```py
try:
      my_q.get()

except pymqi.MQMIError as e:

if e.comp == pymqi.CMQC.MQCC_FAILED and e.reason == pymqi.CMQC.MQRC_NO_MSG_AVAILABLE:
      # No messages left to get, this is ok.
      continue
else:
      # Percolate on any other error code
      raise
```

----------

## Using different data types

By default, when you PUT to a queue using PyMQI, the message is a character string.

But by default when you GET this message, you will get a byte string: `b'Message from Python!'`

We can change this behaviour by defining Message Descriptors and Get-message options.

## More complex get with MD and GMO

Start by defining default Message descriptor (MD) and Get-message options objects:

```py
md = pymqi.MD()
gmo = pymqi.GMO()
```

With these objects, you can do:

```py
# a simple get
q1.get(None)

# specify a Message Descriptor (MD)
q1.get(None, md)

# specify an MD and a Get-message options (GMO)
q1.get(None, md, gmo)

# or a GMO but no MD
q1.get(None, None, gmo)
```

### Example

```py
import pymqi 
from pymqi import CMQC, CMQCFC 

qmgr_name = 'QM01' 
my_queue = 'COLIN'

qmgr = pymqi.connect(qmgr_name)    # connect to the Queue Manager
q1   = pymqi.Queue(qmgr, my_queue) # establish Queue handle

gmo = pymqi.GMO() 
gmo.Options = pymqi.CMQC.MQGMO_WAIT | pymqi.CMQC.MQGMO_FAIL_IF_QUIESCING| pymqi.CMQC.MQGMO__NO_SYNCPOINT 
gmo.WaitInterval = 10 * 1000 # 10 seconds 

try: 
      msg = None        
      md = pymqi.MD() 

      # expect empty MD
      print("Line 14:", md['PutApplName']) 

      # msg should be a byte string
      msg = q1.get(None, md, gmo) 
      print("Message:", msg) 

      # expect MD to now be populated
      print("Line 18:", md['PutApplName']) 

except pymqi.MQMIError as e: 
      print(e) 
```

This would produce:

```text
Line 14: b''                                        
Message: b'Message from Python'                     
Line 18: b'BETACP3                     ' 
```

After the MQGET request, the returned control blocks are what MQ returned. The above example shows the  `PutApplName` information before and after the MQGET request.
