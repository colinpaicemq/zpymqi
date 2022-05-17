# Using the enhanced PCF in pymqi
###  Example usage

With the z/OS version of pymqi, the PCF commands have been simplified to make them more obvious, and the commands have been enhanced, for example to provide trace of the commands.
An example 

code::

      import sys
      import platform 
      import pymqi 
      import threading 
      from pymqi import CMQCFC                                                                                 
      from pymqi import CMQC 
                          
      queue_manager = 'M801' 
      qmgr = pymqi.connect(queue_manager)                                                       
      pcf = PCF.PCF(qmgr, 
              response_wait_interval=5000, 
              convert=True); 
      #mgr.set_debug(1+  pymqi.xPCF) 
      #= pcf.inq_q_status(q='SY*',where=("curdepth",">",1)) 
      data = pcf.inq_q(q=b'CP0000',qtype=pymqi.CMQC.MQQT_LOCAL) 
      # ta = pcf.inq_q(q=b'CP0000',qtype="LOCAL") 
      headers = pcf.headers() 
      for h in headers: 
           if h["Reason"] != 0: 
              print("PCF reason code non zero", h["Reason"] ) 
      #    print("=====") 
      #    print(h) 
      i = 0 
      for row in data: 
         i = i + 1
         # convert values to character strings
         pretty = self.pcf.prettify(row) 
         print("=======Record:",i,"==========") 
         print(">>>Number of entries:",len(pretty)) 
         for a in pretty: 
             value = pretty[a] # get the value
             if not isinstance(value,list ): 
                  value = (value,) # make it a tuple 
             for v in value:  # allow for arrays of values
                 aout = str(v) 
                 print(a,":",aout) 

This produced
```
=======Record: 1 ========== 
>>>Number of entries: 61 
Response Id : x'c3e2d840 d4f8f0f1 404040d9 d4f8f0f1 db4bdd57 a2cf6cd4' 
Response Q Mgr Name : b'M801' 
Q Name : b'CP0000' 
Q Type : Local 
Qsg Disp : Q Mgr 
Storage Class : b'DEFAULT' 
Pageset Id : 4 
CF Struc Name : b'' 
Cluster Name : b'' 
Cluster Namelist : b'' 
Q Desc : b'COLINS Comment' 
Inhibit Put : Put Allowed 
...
Alteration Time : b'17.49.14' 
=======Record: 2 ========== 
>>>Number of entries: 2 
Response Id : x'c3e2d840 d4f8f0f1 404040d9 d4f8f0f1 db4bdd57 a2cf6cd4' 
Response Q Mgr Name : b'M801' 
```


### PCF commands example 
pcf = pymqi.PCF("CSQ9",response_wait_interval=2000,convert=True)
<p><a href=./htmlOutput/inq_archive_all.html>result=pcf.inq_archive_all(cmdscope="*")</a></p>
<p><a href=./htmlOutput/inq_cfstatus_backup.html>result=pcf.inq_cfstatus_backup(cfname="CSQSYSAPPL")</a></p>
<p><a href=./htmlOutput/inq_cfstatus_connect.html>result=pcf.inq_cfstatus_connect(cfname="CSQSYSAPPL")</a></p>
<p><a href=./htmlOutput/inq_cfstatus_smds.html>result=pcf.inq_cfstatus_smds(cfname="CSQSYSAPPL")</a></p>
<p><a href=./htmlOutput/inq_cfstatus_summary.html>result=pcf.inq_cfstatus_summary(cfname="CSQSYSAPPL")</a></p>
<p><a href=./htmlOutput/inq_cf_struct.html>result=pcf.inq_cf_struct()</a></p>
<p><a href=./htmlOutput/inq_cf_struct_names.html>result=pcf.inq_cf_struct_names()</a></p>
<p><a href=./htmlOutput/inq_cf_struct_status.html>result=pcf.inq_cf_struct_status()</a></p>
<p><a href=./htmlOutput/inq_channel_status.html>result=pcf.inq_channel_status(channel="*")</a></p>
<p><a href=./htmlOutput/inq_chinit.html>result=pcf.inq_chinit()</a></p>
<p><a href=./htmlOutput/inq_chl_auth_records.html>result=pcf.inq_chl_auth_records(channel="*")</a></p>
<p><a href=./htmlOutput/inq_log.html>result=pcf.inq_log()</a></p>
<p><a href=./htmlOutput/inq_pageset.html>result=pcf.inq_pageset(psid=1)</a></p>
<p><a href=./htmlOutput/inq_q.html>result=pcf.inq_q(q="CP0000")</a></p>
<p><a href=./htmlOutput/inq_qmgr.html>result=pcf.inq_qmgr()</a></p>
<p><a href=./htmlOutput/inq_qsg.html>result=pcf.inq_qsg()</a></p>
<p><a href=./htmlOutput/inq_security.html>result=pcf.inq_security()</a></p>
<p><a href=./htmlOutput/inq_smdsconn.html>result=pcf.inq_smdsconn()</a></p>
<p><a href=./htmlOutput/inq_smds.html>result=pcf.inq_smds()</a></p>
<p><a href=./htmlOutput/inq_stgclass.html>result=pcf.inq_stgclass(stgclass="DEFAULT")</a></p>
<p><a href=./htmlOutput/inq_sub.html>result=pcf.inq_sub(subname="SYSTEM.DEFAULT.SUB")</a></p>
<p><a href=./htmlOutput/inq_sub_status.html>result=pcf.inq_sub_status(subname="SYSTEM.DEFAULT.SUB")</a></p>
<p><a href=./htmlOutput/inq_system.html>result=pcf.inq_system()</a></p>
<p><a href=./htmlOutput/inq_topic_names.html>result=pcf.inq_topic_names(tn="*")</a></p>
<p><a href=./htmlOutput/inq_topic_status.html>result=pcf.inq_topic_status(ts="SYSTEM.BROKER.ADMIN.STREAM")</a></p>
<p><a href=./htmlOutput/inq_usage_log.html>result=pcf.inq_usage_log()</a></p>
<p><a href=./htmlOutput/inq_usage_ps.html>result=pcf.inq_usage_ps(ps=1)</a></p>
<p><a href=./htmlOutput/inq_usage_smds.html>result=pcf.inq_usage_smds()</a></p>
<p><a href=./htmlOutput/set_ql.html>result=pcf.set_ql(q="CP0000",qtype=1,qdesc="COLINS Comment")</a></p>
<p><a href=./htmlOutput/set_qmgr.html>result=pcf.set_qmgr(certlabl="ibmWebSphereMQM801")</a></p>
<p><a href=./htmlOutput/set_qmgr_lu62chl.html>result=pcf.set_qmgr_lu62chl(lu62chl=201,actchl=201,maxchl=201)</a></p>
<p><a href=./htmlOutput/set_qmgr_maxmsgl.html>result=pcf.set_qmgr_maxmsgl(trigint=9999998)</a></p>
<p><a href=./htmlOutput/set_qmgr_tcpchl.html>result=pcf.set_qmgr_tcpchl(tcpchl=201,actchl=201,maxchl=201)</a></p>


```

x = pcf.inq_q(q='CP0000')
x = pcf.inq_log()
x = pcf.inq_usage(ps=1)
x = pcf.alter_qmgr(lu62chl=66)           
x = pcf.alter_q(q="CP0000",qtype=pymqi.CMQC.MQQT_LOCAL,qdesc="COLINS Comment") 
x = pcf.alter_ql(q="CP0000",qdesc="COLINS Comment2") 
x = pcf.alter_qa(q="CP0000QALIAS",qdesc="COLINS QA Comment") 

```

### Passing data
You can pass the parameters using q="CP0000" syntax or pass a dictionary

code:

      kw = {"subname":"SYSTEM.DEFAULT.SUB"}
      # Issue the command 
      data = pcf.inq_sub_status(**kw)
      # get the headers
      hh= pcf.headers() 


On z/OS you can have multiple messages returned for a command.  Each of these will usually have
a PCF header, and PCF data structures.
The data or header is a list of dictionaries which contain the keyword:value.  The raw data from the command will have
the data as returned from MQ. For example

```
3070:b'M801                                     
1175:10 
3116:b'TAPE    ' 
3148:b'        ' 
1203:20 
```

In SCSQC370(CMQSTRC) is a mapping of values to names, such as 

code::

      ...
      case 3116: c = "MQCACF_SYSP_ARCHIVE_UNIT1";
      ...


To covert the keyword values to strings, and interpret the data.You can use 
code::

      for d in result: 
          pretty = pcf.prettify(d) 
 

This gives, for example

```
Response Id : x'c3e2d840 d4f8f0f1 404040c7 d4f8f0f1 db48f387 9ce27814' 
Response Q Mgr Name : b'M801' 
Sysp Type : Type Initial 
Sysp Archive Unit1 : b'TAPE' 
Sysp Archive Unit2 : b'' 
Sysp Alloc Unit : Alloc Blk 
```

you can then use

code:: 

      pretty = pcf.prettify(d) 
      pfx1 = pretty["Sysp Archive Pfx1"] 


to get the high level log prefix.



### Returned data
The returned value is a list of dictionaries containing the data.
The PCF header information can be obtained using

```
 hh= self.pcf.headers() 
```
This returns a list of dictionaries which contain the return code, number of parameters etc.

where the commands are:

- pcf.inq_qmgr() 
- pcf.inq_chinit() 
- pcf.inq_thread() 
- pcf.inq_comm_info ()
- pcf.inq_log() 
- pcf.inq_conn()
- pcf.inq_trace() 
- pcf.inq_cf_names() 
- pcf.inq_system() 
- pcf.inq_chl_auth_records ()
- pcf.inq_q()
- pcf.inq_appl_status ()
- pcf.inq_topic_names() 
- pcf.inq_topic_status() 
- pcf.inq_sub()
- pcf.inq_sub_status()
- pcf.inq_topic()
- pcf.inq_security()
- pcf.inq_stgclass()
- pcf.inq_archive()
- pcf.inq_pageset()
- pcf.inq_usage()
- pcf.inq_usage_log()
- pcf.inq_q_status()
- pcf.alter_qmgr()
- pcf.alter_q()

The data returned is from the command.  For example 
```
x = pcf.inq_usage(ps=1) 
headers = pcf.headers() 
i = 0 
for h in headers: 
    i = i + 1 
    print("== Header",i,"==") 
    print(h) 

print(" ") 
i = 0 
for xx in x: 
    i = i + 1 
    print("=============",i,"====================") 
    z = pcf.prettify(xx  ) 
    for a in z: 
        print(a,":", z[a]) 

```
Gives output
```
== Header 1 ==                                      
Type: 18                                            
StrucLength: 36                                     
Version: 3                                          
Command: 126                                        
MsgSeqNumber: 1                                     
Control: 0                                          
CompCode: 0                                         
Reason: 0                                           
ParameterCount: 13                                  
== Header 2 ==
....

==Response 1 ====================                                                                             
Response Id : b'\xc3\xe2\xd8@\xc3\xe2\xd8\xf9@@@\xd9\xc3\xe2\xd8\xf9\xdb\x1c5v\xee\x0f\xd3\x80'               
Response Q Mgr Name : b'CSQ9'                                                                                 
Pageset Id : 1                                                                                                
Usage Total Pages : 1078                                                                                      
Usage Unused Pages : 1065                                                                                     
Usage Persist Pages : 13                                                                                      
Usage Nonpersist Pages : 0                                                                                    
Usage Expand Count : 0                                                                                        
Buffer Pool Id : 0                                                                                            
Usage Expand Type : Expand User                                                                               
DS Encrypted : No                                                                                             
Pageset Status : PS Available                                                                                 
Usage Type : Usage Pageset                                                                                    
==Response 2 ====================                                                                             
...
```
Without the prettify function, the data comes back in raw MQ PCF format
```
==Response 1 ====================                                                                        
7004 : b'\xc3\xe2\xd8@\xc3\xe2\xd8\xf9@@@\xd9\xc3\xe2\xd8\xf9\xdb\x1d7\xb81>\x17\x00'                    
3070 : b'CSQ9                                            '                                               
62 : 1                                                                                                   
1159 : 1078                                                                                              
1160 : 1065                                                                                              
1161 : 13                                                                                                
1162 : 0                                                                                                 
1164 : 0                                                                                                 
1158 : 0                                                                                                 
1265 : 1                                                                                                 
1436 : 0                                                                                                 
1165 : 0                                                                                                 
1157 : 1168                                                                                              
```
## Using short names in PCFcommands.
You can use short keyword names in some PCF requests.    The current list is defined below.
Some keyword values can be mapped from a string to a value, for example qtype is defined as

> maps to = CMQC.MQCA_CERT_LABEL, object prefix=MQQT_ 

When qtype=LOCAL is used, qtype will be replace with CMQC.MQCA_CERT_LABEL, and the value will be replaced by
MQQT_LOCAL.
This makes the definition of the parameters much easier to use.


|Short key          |maps to         | Object prefix|
|---                |---             | --- |
|q|                 CMQC.MQCA_Q_NAME | 
|comminfoname|      CMQCFC.MQCMD_INQUIRE_COMM_INFO| 
|certlabl|          CMQC.MQCA_CERT_LABEL| 
|channel|           CMQCFC.MQCACH_CHANNEL_NAME | 
|channeltype|       CMQCFC.MQCACH_CHANNEL_NAME | MQOT_| 
|chltype|           CMQCFC.MQCACH_CHANNEL_NAME | MQOT_| 
|cfname|            CMQC.MQCA_CF_STRUC_NAME| 
|cmdscope|          CMQCFC.MQCACF_COMMAND_SCOPE| 
|gconid|            CMQCFC.MQBACF_GENERIC_CONNECTION_ID| 
|lu62Channels|      CMQC.MQIA_LU62_CHANNELS| 
|lu62chl|           CMQC.MQIA_LU62_CHANNELS| 
|maxmsgl|           CMQC.MQIA_MAX_MSG_LENGTH| 
|maxmsglength|      CMQC.MQIA_MAX_MSG_LENGTH| 
|pageset|           CMQC.MQIA_PAGESET_ID| 
|ps|                CMQC.MQIA_PAGESET_ID| 
|psid|              CMQC.MQIA_PAGESET_ID| 
|qdesc|             CMQC.MQCA_Q_DESC| 
|qtype|             CMQC.MQCA_CERT_LABEL|MQQT_| 
|stgclass|          CMQC.MQCA_STORAGE_CLASS| 

