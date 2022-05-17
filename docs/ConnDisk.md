# Connect and disconnect

## Connect using local binding

### Using MQCONN


code ::

     import pymqi
     
     queue_manager = 'CSQ9'
     qmgr = pymqi.connect(queue_manager)


### Connecting from clients
From native z/OS you cannot use a client to connect, as there is no support to run a client on z/OS. You can still use the MQCONNX, but not use the client support.

### Connect using MQCONNX
It may be easier to create a connect method (function), so you define the connect parameters once, and call the function from your code.  To change queue manager parameters, you change just one file.

code::

      import pymqi
      def connect(): 
        queue_manager = 'CSQ9' 
        cd = pymqi.CD() 
        sco = pymqi.SCO() 
        qmgr = pymqi.QueueManager(None) 
        qmgr.connect_with_options(queue_manager, cd, sco) 
      return qmgr 

### Disconnect

code::

      qmgr.disconnect()
