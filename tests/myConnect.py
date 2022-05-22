# routine to isolate the MQ connect from the other MQ scripts
import pymqi
def connect():
  queue_manager = 'M801'
  qmgr = pymqi.connect(queue_manager)
  qmgr.set_debug(1+pymqi.xCC+pymqi.xPCF)
  return qmgr

