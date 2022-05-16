## Commit or Backout

You should always explicitly include either 

      qmgr.backout() or 
      qmgr.commit()

or use the MQGMO_NO_SYNCPOINT or MQPMO_NO_SYNCPOINT option,
because the default syncpoint behaviour is different on z/OS and midrange.

