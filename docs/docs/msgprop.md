## Message handle and message properties

## Put with message properties
## Message properties

The z/OS version of Pymqi has better support for message properties.
It can accept a name of "name" or b"name".

code::

      import pymqi 
      from pymqi import CMQCFC 
      from pymqi import CMQC 
      queue_manager = 'M801' 
      qmgr = pymqi.connect(queue_manager) 
      hMsg = pymqi.MessageHandle(qmgr) 
      qmgr.set_debug(1+pymqi.xSETMP ) 
      # Char or byte names are supported
      hMsg.set(b'COLINCHAR','COLINAAA')  #name, value
      hMsg.set('COLINBYTE',B'12345BYT') 
      hMsg.set(b'COLINFLOAT',1.0E2) 
      hMsg.set('COLININT64',7) 
                                                                                                                  
      q1 = pymqi.Queue(qmgr, 'CP0000') 
      pmo = pymqi.PMO( # Version 3 required for setting properties, defaults to 3 on z/OS
                      OriginalMsgHandle=hMsg.msg_handle, 
                      Options=pymqi.CMQC.MQGMO_NO_SYNCPOINT + 
                              pymqi.CMQC.MQGMO_FAIL_IF_QUIESCING) 
      md = None
      q1.put('Message from Python',md,pmo)
      p = hMsg.get_all_properties() 
      print("Put properties", p) 
                                                 
This prints

      Put properties {'COLINCHAR': b'COLINAAA', 'COLINBYTE': b'COLINBYT', 'COLINFLOAT': 100.0, 'COLININT64': 7}  


You can use message properties in a get
code::

      gmo = pymqi.GMO() 
      gmo = pymqi.GMO() # Version 4 required for getting properties, defaults to 4 on z/OS 
      gmo.MsgHandle = hGMsg.msg_handle 
      q2   = pymqi.Queue(qmgr, 'CP0000') 
      gmo  = pymqi.GMO( 
                    MsgHandle=hGMsg.msg_handle, 
                    Options=pymqi.CMQC.MQGMO_NO_SYNCPOINT + 
                           pymqi.CMQC.MQGMO_FAIL_IF_QUIESCING)
      md = None 
      m = q2.get(None,md,gmo) 
      print("Message",m ) 
      p = hGMsg.get_all_properties() 
      print("Properties at get",p) 

This prints the same data as the preceeding put.

    Properties at get {b'COLINCHAR': b'COLINAAA', b'COLINBYTE': b'12345BYT', b'COLINFLOAT': 100.0, b'COLININT64': 7}  

You can get individual properties
code::

      gp = hGMsg.properties.get(b'COLINBYTE') 
      print("ColinByte Property",gp) 
      
      gf = hGMsg.properties.get(b'COLINFLOAT') 
      print("ColinFloat property",gf) 
This produces

      ColinByte Property b'12345BYT'
      ColinFloat property 100.0

To put message properies with the midrange pymqi the syntax is 

code::

      hMsg.properties.set(b'COLININT64',7,property_type=CMQC.MQTYPE_INT32,value_length=4) 
      hMsg.properties.set(b'COLINCHAR','COLINAAA',property_type=CMQC.MQTYPE_STRING,value_length=8) 
      hMsg.properties.set(b'COLINBYTE',B'COLINBYT',property_type=CMQC.MQTYPE_BYTE_STRING,value_length=8) 

Where you need to specify the type. If you are using MQTYPE_STRING, or MQTYPE_BYTE_STRING you must specify the length.
