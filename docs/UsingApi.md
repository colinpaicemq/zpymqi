# Using the API
### Creating MQ structures

You can create MQ structures for example 

      gmo =pymqi.GMO()
      print(gmo)
gives

        StrucId: b'GMO '                                               
        Version: 4                                                     
        Options: 0                                                     
        WaitInterval: 0                                                
        Signal1: 0                                                     
        Signal2: 0                                                     
        ResolvedQName: b''                                             
        MatchOptions: 3                                                
        GroupStatus: 32                                                
        SegmentStatus: 32                                              
        Segmentation: 32                                               
        Reserved1: b' '                                                
        MsgToken: b''                                                  
        ReturnedLength: -1                                             
        Reserved2: 0                                                   
        MsgHandle: 0                                                   

The structure classes are (for example pymqi.GMO())


| Python class|MQ object |Description|
| ---|---|---|      
| CD |MQCD |Channel Definition |
| CMHO |MQCMHO |Create Message Handle Operation
| MD |MQMD |Message descriptor |
| **GMO** |MQGMO |Get Message Options|
| IMPO |MQIMPO|Inquire Message Property Options|
| OD |MQOD |Object desciptor|
| PD |MQPD |Property Descriptor| 
| PMO |MQPMO |Put Message Options|
| RFH2 |MQRFH2|Rules and Formatting Header|
| SCO | MQSCO|SSL Configuration Options|
| SMPO |MQSMPO|Set Message Property Options|
| SRO | MQSRO| Subscription request Options|
| SD |MQSD|Subscription Descriptor| 
| TM |MQTM|Trigger Message|
| TMC2|MQTMC2|Trigger Message 2| 

## Exceptions
If you get a non zero return code from MQ, a MQMIError exception is thrown, for example

    pymqi.MQMIError: MQI Error. Comp: 2, Reason 2035: FAILED: MQRC_NOT_AUTHORIZED QM=CSQ9 


You can extract information from the exception
code::

      d = vars(e) # create a dict from the exception
      print("e:comp           ",d["comp"            ]) 
      print("e:reason         ",d["reason"            ]) 
      print("e:values         ",d["values"            ]) 
      # the following are optional depending on the MQ verb issued
      print("e:QName          ",d["QName"]) 
      print("e:Verb           ",d["Verb"]) 
      print("e:message        ",d["message"]) 
      print("e:original_length",d["original_length"   ]) 

This gives

      e:comp            2                                                                                         
      e:reason          2033                                                                                      
      e:values          QName=COLIN message= original_length=0 Verb=MQGET                                         
      e:QName           COLIN                                                                                               
      e:Verb            MQGET                                                                                                
      e:message         b''                                                                                               
      e:original_length 0                                                                                         

Other errors may result in an PYIFError exception, for example

1. An MQ control block passed to pymqi is not well formed; such as a bad eye catcher or length
1. A request has been made but the application is not connected to MQ.

## Trace

You can turn on a trace in the zpymqi module (from the C code).   This traces, the parameters passed to and from MQ calls.  

code::

        qmgr.set_debug(...) 

Where the ...  is a number for example  qmgr.set_debug(1+pymqi.xCC+pymqi.xPCF)
 
| constant |value |meaning|
| :-----    |-----:|:----- |
| | 1 | if the return code is non zero|
| pymqi.xCC| 2 | display the compcode and reason code including rc = 0 |
| pymqi.xPUTBUFFER |4| display the put buffer ( after the MQPUT*) |
| pymqi.xPUT |4| display the put buffer ( after the MQPUT*) |
| pymqi.xGETBUFFER |8| display the buffer after an MQGET|
| pymqi.xGET |8| display the buffer after an MQGET| 
| pymqi.xMQOD      |16| display the MQOD after an open |
| pymqi.xMQMD      |32| display the MQMD after an MQGET|
| pymqi.xMQPMO     |64| display the MQPMO options after the MQPUT|
| pymqi.xMQGMO     |128| display the MQGMO options after the MQGET| 
| pymqi.xSUB       |256| display the MQSD, Object name, ResObjectString, subname,sub options, after and MQSUB|
| pymqi.xSETQMP   |1512| displays the Set Message Properties options, and property name etc|
| pymqi.xINQMP    |1024| displays the Inquire Message Properties options, and property name etc|
| pymqi.xPCF      |2048| for the MQPUT, displays the cc and reason code, the PCF command, the count, and the   first 1024 bytes of the structure.
| pymqi.xPCF      |2048| for the MQGET, it displays the same information + PCF compcode and reason code|
| pymqi.xCONN     |4096| displays MQCONNX info, MQCNO, MQCSP| 
