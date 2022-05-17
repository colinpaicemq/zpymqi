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
<tr><td>begin()</td></tr>
<tr><td>commit()</td></tr>
<tr><td>backout()</td></tr>
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
<tr><td>get_handle()</td></tr>
<tr><td>get_debug()</td></tr>
<tr><td>set_debug(value) </td></tr>
<tr><td>get_name()</td></tr>
<tr><td> inquire(attribute) </td>
<td>attribute is an integer like pymqi.CMQC.MQCA_Q_MGR_IDENTIFIER
<td>value, such as byte string or integer, matching the request.   
</tr>

<tr><td> put1(qDescm,msg &lt;,md() &lt;,pmo&gt; &gt; )
<td><ul><li>qDesc is either a name or an od() object
<li>md is a md() object or None
<li>pmo is a pmo() object or None
</td></tr>
</table> 

## Queue
<table style="border: 1px solid black;">
  <tr>
    <th width=50%>Method</th>
    <th width=25%>Parameters</th>
    <th width=25%>Return value </th>
  </tr>
 <tr>
    <td>
Queue(qmgr() &lt;,od() , &lt;  mqoo &gt;&gt;)  
</td>

</tr>
<tr><td>
  open(qdesc,&lt; open_options)
   
</td>
<td><ul><li>qdesc = queue name or od()</ul></td>
<td>queue handle object</td>
</tr>
<tr>
<td>
  put(message&lt;,md()&lt;,pmo()&gt;&gt;)
</td>
<td><ul><li>string: message<li>md()<li>pmo</ul></td>
<td>queue handle object</td>
</tr>
<tr><td>
  put_rfh2(message,md(),(rfh2(),rfh2())
<td><ul><li>????</ul></td>
<td></td>
</tr>
<tr>
<td>
  get(&lt;maxLength=None&gt;,&lt;md() &lt;,gmo()&gt;&gt;)
</td>
<td><ul><li>string: message<li>md()<li>pmo</ul></td>
<td>queue handle object</td>
</tr>
</table>


#### get_rfh2
 get_rfh2(%lt;max_length=None%gt;
  close()
  inquire(attribute)
  set(attribute,arg)
  set_handle(queue_handle)
  get_handle()
<table style="border: 1px solid black;">
  <tr>
    <th width=50%>Method</th>
    <th width=25%>Parameters</th>
    <th width=25%>Return value </th>
  </tr>
 <tr>
<tr><td>
Topic(queue_manager &lt;, topic_name=None&gt; &lt;,topic_string=None&gt; &lt;,topic_desc=None&gt; &lt;,open_opts=None&gt;)
</td>
</tr>
<tr><td>
open(def open(topic_name=None, topic_string=None, topic_desc=None, open_opts=None) 
</td>
<td><ul><li>String:topic_name<li>String:topic_string<li>String: topic_desc<li>int:open_opts</ul>
</td>
<td>topic object</td>
</tr>
<tr><td>
  pub(msg &lt;,md() (,pmo&gt;&gt;) 
</td>
<td><ul><li>byte string:msg<li>pmo:></ul></td>
<td></td>
</tr>
<tr><td>
  pub_rfh2()
</td>
<td></td>
<td></td>
</tr>
<tr><td>
  sub(&lt;sd()&gt;,&lt;queuename&gt;)
</td>
<td><ul><li>sd:sd<li>qmgr object</u></td>
<td></td>
</tr>
<tr><td>
  close(&lt;options=CMQC.MQCO_NONE&gt;)
</td>
<td><ul><li>integer:options</ul></td>
<td></td>
</tr>
</table>
#### Subscription

<table style="border: 1px solid black;">
  <tr>
    <th width=50%>Method</th>
    <th width=25%>Parameters</th>
    <th width=25%>Return value </th>
  </tr>
 <tr>
<tr>
<td>
Subscription(queue_manager, sub_desc=None, sub_name=None,
                 sub_queue=None, sub_opts=None, topic_name=None, topic_string=None)
</td>
<td>
<ul>
<li>qmgr object: queue_manager
<li>sub_desc
<li>sub_name
<li>sub_queue
<li>integer:sub_opts
<li>String: topic_name
</ul>
</td>
</tr>
<tr>
<td>
  sub(&lt;sd()&gt;,&lt;queuename&gt;)
</td>
<td>
<ul><li>sd:sd<li>qmgr object</u>
</td>
<td>
</td>
</tr>
<tr>
<td>
    get(max_length=None &lt;md() &lt;,gmo&gt;&gt; )
</td>
<td>
<ul>
<li>integer:max_length
<li>md:
<li>gmo:
</ul>
</td>
<td>
</td>Byte string
</tr>
<tr>
<td>
    sub(sub_desc=None, sub_queue=None, sub_name=None, sub_opts=None,
            topic_name=None, topic_string=None)
</td>
</tr>
<tr>
<td>
close(sub_close_options=CMQC.MQCO_NONE,close_sub_queue=False, close_sub_queue_options=CMQC.MQCO_NONE)
<td>
<ul>
<li>integer: sub_close_options
<li>Boolean: close_sub_queue
<li>Integer: close_sub_queue_options
</ul>
</td>
</tr>
</table> 

### MessageHandle
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

