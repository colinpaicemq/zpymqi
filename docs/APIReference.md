# API reference

## Usage

You can set up control blocks in a variety of ways.
For example

      gmo = pymqi.GMO() 
      gmo.Options = pymqi.CMQC.MQGMO_WAIT | pymqi.CMQC.MQGMO_FAIL_IF_QUIESCING | pymqi.CMQC.MQPMO_SYNCPOINT 
      gmo.WaitInterval = 1  * 1000 # 1  seconds

or 

      gmo = pymqi.GMO(WaitInterval= 1*1000,Options = ....)
  
To find the names (and values) of all of the fields, you can use the MQ documentation or use the Python statement such as 

      print("GMO",gmo)

Some fields are positional, some are keyword=value.  If you wish to omit a positional value use None

#### GMO()
#### PMO()
#### OD()
#### MD()
#### RFH2()
      add_folder(folder_data)
      byte string = pack(encoding=None)
     () =unpack(buffer,encoding=None)
#### TM()
#### TMC2()
#### CD()
#### SCO()
#### SD()
#### SRO()
   
CMHO()
PD()
SMPO()
IMPO()
XQH()
CFH()
 get()
CFBF()
CFBS()
CFQR()
CFIF()
CFIL()
CFIL64()
CFIN()
CFIN64()
CFSF()
CFSL()
CFST()
#### QueueManager
(name='', disconnect_on_exit=True,bytes_encoding=default.bytes_encoding, default_ccsid=default.ccsid)
 connect(name)
 connect_with_options(name, &lt;cd() &lt;,sco()&gt; &lt;,user=...&gt;&lt;,password=...&gt;)

<table style="border: 1px solid black;">
  <tr>
    <th width=50%>Method</th>
    <th width=25%>Parameters</th>
    <th width=25%>Return value </th>
  </tr>
 <tr>
    <td>QueueManager
(&lt;name=''&gt; &lt;disconnect_on_exit=True,&gt; &lt;,bytes_encoding=default.bytes_encoding&gt; &lt;,default_ccsid=default.ccsid&gt;)</td>
    <td>String:name</td>
    <td>qmgr object </td>
  </tr>

  <tr>
    <td>connect_with_options(name [, opts=cnoopts][ ,cd=mqcd][ ,sco=mqsco]) )</td>
    <td><ul><li>String:name 
          <li>Integer:cnoopts
          <li>mqcd object:cd
          <li>mqsco object: sco
    </td>
    <td>queue manager object</td>
  </tr>
 <tr>
    <td>connect_tcp_client(name, cd, channel, conn_name, user=None, password=None)<p> not supported on z/OS</td>
</td>
    <td></td>
  </tr>
<tr>
    <td>disconnect()</td>
    <td></td>
    <td></td>
  </tr>
<tr><td>disconnect()<td>
</tr>
<tr><td>get_handle()</td></tr>
<tr><td>get_debug()</td></tr>
<tr><td>set_debug(value) </td></tr>
<tr><td>get_name()</td></tr>
<tr><td>begin()</td></tr>
<tr><td>commit()</td></tr>
<tr><td>backout()</td></tr>
<tr><td> put1(qDescm,msg &lt;,md() &lt;,pmo&gt; &gt; )
<td><ul><li>qDesc is either a name or an od() object
<li>md is a message description object or None
<li>pmo is a put message options object or None
</td></tr>
<tr><td> inquire(attribute) </td>
<td>attribute is an integer like pymqi.CMQC.MQCA_Q_MGR_IDENTIFIER
<td>value, such as string or integer, matching the request  
</tr>

</table> 




 put1(qDescm,msg &lt;,md() &lt;,pmo&gt; &gt; )
    qDesc is either a name or an od() object)
 inquire(attribute) 
 _is_connected() 
Queue(qmgr() &lt;,od() , &lt;  mqoo &gt;&gt;
  open(qdesc,&lt; open_options)
    where qdesc = queue name or od()
  put(message&lt;,md()M,pmo()&gt;&gt;)
  put_rfh2(message,md(),,(rfh2(),rfh2())
  get(&lt;maxLength=None&gt;,&lt;md() &lt;,gmo()&gt;&gt;)
#### get_rfh2
 get_rfh2(%lt;max_length=None%gt;
  close()
  inquire(attribute)
  set(attribute,arg)
  set_handle(queue_handle)
  get_handle()

Topic(queue_manager &lt;, topic_name=None&gt; &lt; , topic_string=None&gt; &lt;,topic_desc=None&gt; &lt;, open_opts=None&gt;)
  open(def open(self, topic_name=None, topic_string=None, topic_desc=None, open_opts=None) 
  pub(msg &lt;,md() (,pmo&gt;&gt;) 
  pub_rfh2()
  sub(&lt;sd()&gt;,&lt;queuename&gt;)
  close(&lt;options=CMQC.MQCO_NONE&gt;
Subscription(queue_manager, sub_desc=None, sub_name=None,
                 sub_queue=None, sub_opts=None, topic_name=None, topic_string=None)
    get(max_length=None &lt;md() &lt;,gmo&gt;&gt; )
    sub(sub_desc=None, sub_queue=None, sub_name=None, sub_opts=None,
            topic_name=None, topic_string=None)
    close(sub_close_options=CMQC.MQCO_NONE,close_sub_queue=False, close_sub_queue_options=CMQC.MQCO_NONE)
MessageHandle()
  set(name,value)
  get_all_properties()

FilterOperator(selector,name)
Filter(selector)
PCFExecute(name=None,
                 disconnect_on_exit=True,
                 model_queue_name=b'SYSTEM.DEFAULT.MODEL.QUEUE',
                 dynamic_queue_name=b'PYMQPCF.*',
                 command_queue_name=b'SYSTEM.ADMIN.COMMAND.QUEUE',
                 response_wait_interval=5000,
                 convert=False)
  disconnect()
  unpack(message)
  prettify(data)
  prettifyPCFHeader(data)
  PCFValue(p1, p2)

ByteString(value)

connect(queue_manager, 
            user=None, password=None, disconnect_on_exit=True,
            bytes_encoding=default.bytes_encoding, default_ccsid=default.ccsid):

connect(queue_manager, channel=None, conn_info=None,
            user=None, password=None, disconnect_on_exit=True,
            bytes_encoding=default.bytes_encoding, default_ccsid=default.ccsid):

