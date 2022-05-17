# Python MQI Class Wrappers. High level classes that for the MQI
# Extension. These present an object interface to MQI.
#
# This is for the extended PCF support, using familiar terms instead of constants
# Author: L. Smithson (lsmithson open-networks.co.uk)
# Author: Colin Paice  (colinpaice3@gmail.com)
#
# DISCLAIMER
# You are free to use this code in any way you like, subject to the
# Python & IBM disclaimers & copyrights. I make no representations
# about the suitability of this software for any purpose. It is
# provided 'AS-IS' without warranty of any kind, either express or
# implied. So there.
#
"""
PyMQI - Python MQI Wrapper Classes
These classes wrap the pymqe low level MQI calls. They present an OO
interface with a passing resemblance MQI C++.
Classes are also provided for easy use of the MQI structure parameters
(MQMD, MQGMO etc.) from Python. These allow Python scripts to set/get
structure members by attribute, dictionary etc.
The classes are:
an instance of a PCFExecute object.
Pymqi is thread safe. Pymqi objects have the same thread scope as
their MQI counterparts.
"""

from pymqi import MQOTHER
from pymqi import CMQCFC
from pymqi import CMQC
import pymqi
# stdlib
import struct
import ctypes
import sys
import platform
try:
    from typing import Any, Optional, Union, Dict, List
except ImportError:
    pass
class PCF(pymqi.PCFExecute):

    xCC = 2
    xPUTBUFFER = 4
    xGETBUFFER = 8
    xMQOD = 16
    xMQMD = 32
    xMQPMO = 64
    xMQGMO = 128
    xSUB = 256
#  #define xCRTMH     256
#  #define xSETMP     512
    xINQMP = 1024
    xPCF = 2048
    def prettifyPCFHeader(self, data):
        super().prettifyPCFHeader(data)

    def where(self, value):
        # type of data, operation, value
        ret = []
        if len(value) != 3:
            print("where needs 3 values")
            return ret
        dataType = self.kw_mapping(value[0])
        filterOperator = pymqi.FilterOperator(dataType, value[1])  # eg "qname", "eq"
        if isinstance(value[2], int):
            ret = [pymqi.IntegerFilter(dataType, value[2], filterOperator.operator)]
        return ret

    def kw_mapping(self, value):
        data_mapping = {"acctq"           : [CMQC.MQIA_ACCOUNTING_Q, "MQMON_"],
                        "actchl"          : CMQC.MQIA_ACTIVE_CHANNELS,
                        "actchannels"     : CMQC.MQIA_ACTIVE_CHANNELS,
                        "bp"              : CMQCFC.MQIACF_BUFFER_POOL_ID,
                        "bpid"            : CMQCFC.MQIACF_BUFFER_POOL_ID,
                        "certlabl"        : CMQC.MQCA_CERT_LABEL,
                        "cfname"          : CMQC.MQCA_CF_STRUC_NAME,
                        "cfstatus"        : MQOTHER.MQIACF_CF_STATUS_TYPE,   # not in CMQC
                        "channel"         : CMQCFC.MQCACH_CHANNEL_NAME,
                        "channeltype"     : [CMQCFC.MQIACH_CHANNEL_TYPE, "MQCHT_"],
                        "comminfoname"    : CMQCFC.MQCMD_INQUIRE_COMM_INFO,
                        "chltype"         : [CMQCFC.MQIACH_CHANNEL_TYPE, "MQCHT_"],
                        "cmdscope"        : CMQCFC.MQCACF_COMMAND_SCOPE,
                        "curdepth"        : CMQC.MQIA_CURRENT_Q_DEPTH,
                        "gconid"          : CMQCFC.MQBACF_GENERIC_CONNECTION_ID,
                        "lu62Channels"    : CMQC.MQIA_LU62_CHANNELS,
                        "lu62chl"         : CMQC.MQIA_LU62_CHANNELS,
                        "maxchl"          : CMQC.MQIA_MAX_CHANNELS,
                        "maxchannel"      : CMQC.MQIA_MAX_CHANNELS,
                        "maxdepth"        : CMQC.MQIA_MAX_Q_DEPTH,
                        "maxmsgl"         : CMQC.MQIA_MAX_MSG_LENGTH,
                        "maxmsglength"    : CMQC.MQIA_MAX_MSG_LENGTH,
                        "pageset"         : CMQC.MQIA_PAGESET_ID,
                        "ps"              : CMQC.MQIA_PAGESET_ID,
                        "psid"            : CMQC.MQIA_PAGESET_ID,
                        "q"               : CMQC.MQCA_Q_NAME,
                        "qname"           : CMQC.MQCA_Q_NAME,
                        "qdesc"           : CMQC.MQCA_Q_DESC,
                        "qtype"           : [CMQC.MQIA_Q_TYPE, "MQQT_"],
                        "stgclass"        : CMQC.MQCA_STORAGE_CLASS,
                        "replace"         : [CMQCFC.MQIACF_REPLACE, "MQRP_"],
                        "tcpchl"          : CMQC.MQIA_TCP_CHANNELS,
                        "tcpchlannels"    : CMQC.MQIA_TCP_CHANNELS,
                        "tn"              : CMQC.MQCA_TOPIC_NAME,
                        "ts"              : CMQC.MQCA_TOPIC_STRING, # topic string
                        "topicstring"     : CMQC.MQCA_TOPIC_STRING, # topic string
                        "triggerinterval" : CMQC.MQIA_TRIGGER_INTERVAL,
                        "trigint"         : CMQC.MQIA_TRIGGER_INTERVAL,
                        "subname"         : CMQCFC.MQCACF_SUB_NAME,
                        "smds"            : CMQCFC.MQCACF_CF_SMDS,
                        "smdsconn"        : CMQCFC.MQCACF_CF_SMDSCONN,
                        "subid"           : CMQCFC.MQBACF_SUB_ID,
                        "usage"           : [MQOTHER.MQIACF_USAGE, "MQUS_"],  # not in CMQC
                        "usagetype"       : [MQOTHER.MQIACF_USAGE_TYPE, "MQUS_"],  # not in CMQC
                        "psid"            : CMQC.MQIA_PAGESET_ID}
        data = data_mapping.get(value)
        return data

    def kw(self, kwargs):
        attr = {}
        filter = []
        for data1, value in kwargs.items():
            if data1 == "where":
                filter = self.where(value)
                continue
            data = self.kw_mapping(data1)
            if  data is None:
                print("bad parameter", data1)
                raise PYIFError('parameter not recognised %s'  % data1)
            # if value is [CMQC.MQIA_Q_TYPE, "MQQT_"] then
            # create a variable MQQT_value (eg MQQT_LOCAL)
            # and see if that exists in the pymqi constants
            if isinstance(data, list):
                if not isinstance(value, int):
                    prefix = data[1] # get the variable prefix
                    prefix = prefix + value  # make the combined prefix + name
                    v = getattr(CMQC, prefix, False) # try this
                    if v is False:
                        v = getattr(CMQCFC, prefix, False)  # and this
                    if v is False:
                        print("String not found ", prefix)
                    else:
                        value = v
                data = data[0]
            attr[data] = value
        return attr, filter

    def ret(self, rv):
        self.__PCFHeaders = rv[1]
        return rv[0]

    def headers(self):
        return self.__PCFHeaders

    def doit(self, verb, dict):
        method =pymqi. _Method(self, verb)
        args, filter = self.kw(dict)
        rv = method._m_doit(args, filter)
        return self.ret(rv)
#############################
###  now the individual items
#############################
    def inq_qmgr(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_Q_MGR", kwargs)

    def inq_chinit(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_CHANNEL_INIT", kwargs)

    def inq_thread(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_THREAD", kwargs)

    def inq_comm_info(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_COMM_INFO", kwargs)

    def inq_log(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_LOG", kwargs)

    def inq_conn(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_CONNECTION", kwargs)

    def inq_trace(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_TRACE", kwargs)

    def inq_system(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_SYSTEM", kwargs)

    def inq_chl_auth_records(self, **kwargs):
        defaults = {"channel":"SYSTEM.*"}
        return self.doit("MQCMD_INQUIRE_CHLAUTH_RECS", {**defaults, **kwargs})

    def inq_q(self, **kwargs):
        defaults = {"q":"SYSTEM.*"}
        x = {**defaults, **kwargs}
        return self.doit("MQCMD_INQUIRE_Q", {**defaults, **kwargs})

    # midrange only
    def inq_appl_status(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_APPL_STATUS", kwargs)

    def inq_topic_names(self, **kwargs):
        defaults = {"tn":"*"}
        return self.doit("MQCMD_INQUIRE_TOPIC_NAMES", {**defaults, **kwargs})

    def inq_topic_status(self, **kwargs):
        defaults = {"ts":"Not_Specified"}
        return self.doit("MQCMD_INQUIRE_TOPIC_STATUS", {**defaults, **kwargs})

    def inq_sub(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_SUBSCRIPTION", kwargs)

    def inq_sub_status(self, **kwargs):
          return self.doit("MQCMD_INQUIRE_SUB_STATUS", kwargs)

    def inq_topic(self, **kwargs):
        defaults = {"tn":"Not_Specified"}
        return self.doit("MQCMD_INQUIRE_TOPIC", {**defaults, **kwargs})

    def inq_security(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_SECURITY", kwargs)

    def inq_stgclass(self, **kwargs):
        defaults = {"stgclass":"DEFAULT"}
        return self.doit("MQCMD_INQUIRE_STG_CLASS", {**defaults, **kwargs})

    def inq_archive(self, **kwargs):
        return  self.doit("MQCMD_INQUIRE_ARCHIVE", kwargs)
    def inq_pageset(self, **kwargs):
        return self.doit("MQCMD_INQUIRE_USAGE", kwargs)

    def inq_usage(self, **kwargs):
        defaults = {"usagetype":CMQCFC.MQIACF_ALL}
        return self.doit("MQCMD_INQUIRE_USAGE", {**defaults, **kwargs})

    def inq_usage_log(self, **kwargs) :
        defaults = {"usagetype":CMQCFC.MQIACF_USAGE_DATA_SET}
        return self.doit("MQCMD_INQUIRE_USAGE", {**defaults, **kwargs})

    def inq_usage_ps(self, **kwargs) :
        defaults = {"usagetype":CMQCFC.MQIACF_USAGE_PAGESET }
        return self.doit("MQCMD_INQUIRE_USAGE", {**defaults, **kwargs})

    def inq_q_status(self, **kwargs):
        defaults = {"q":"SYSTEM.*"}
        return self.doit("MQCMD_INQUIRE_Q_STATUS", {**defaults, **kwargs})

# Z/OS CF related stuff

    def inq_qsg(self, **kwargs):
        print("===========")
        return self.doit("MQCMD_INQUIRE_QSG", kwargs)

    def inq_groupg(self, **kwargs):
        print("===========")
        return self.doit("MQCMD_INQUIRE_QSG", kwargs)

    def inq_smds(self, **kwargs):
        defaults = {"smds":"*","cfname":"CSQSYSAPPL"}
        return self.doit("MQCMD_INQUIRE_SMDS", {**defaults, **kwargs})

    def inq_smdsconn(self, **kwargs):
        defaults = {"smdsconn":"M801","cfname":"CSQSYSAPPL"}
        return self.doit("MQCMD_INQUIRE_SMDSCONN", {**defaults, **kwargs})

    def inq_cf_struct(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC", {**defaults, **kwargs})

    def inq_cf(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC", {**defaults, **kwargs})

    def inq_cf_struc(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC", {**defaults, **kwargs})

    def inq_cf_names(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_NAMES", {**defaults, **kwargs})

    def inq_cf_struc_names(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_NAMES", {**defaults, **kwargs})

    def inq_cf_struct_status(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_STATUS", {**defaults, **kwargs})

    def inq_cfstatus(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_STATUS", {**defaults, **kwargs})

    def inq_cfstatus_summary(self, **kwargs):
        defaults = {"cfname":"*"}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_STATUS", {**defaults, **kwargs})

    def inq_cfstatus_connect(self, **kwargs):
        defaults = {"cfname":"*","cfstatus":MQOTHER.MQIACF_CF_STATUS_CONNECT}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_STATUS", {**defaults, **kwargs})

    def inq_cfstatus_backup(self, **kwargs):
        defaults = {"cfname":"*","cfstatus":MQOTHER.MQIACF_CF_STATUS_BACKUP}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_STATUS", {**defaults, **kwargs})

    def inq_cfstatus_smds(self, **kwargs):
        defaults = {"cfname":"*","cfstatus":MQOTHER.MQIACF_CF_STATUS_SMDS}
        return self.doit("MQCMD_INQUIRE_CF_STRUC_STATUS", {**defaults, **kwargs})

    def inq_usage_smds(self, **kwargs):
        defaults = {"usagetype":CMQCFC.MQIACF_USAGE_SMDS}
        return self.doit("MQCMD_INQUIRE_USAGE", {**defaults, **kwargs})

    def start_smdsconn(self, **kwargs):
        defaults = {"smdsconn":"*"}
        return self.doit("MQCMD_STARTSMDSCONN", {**defaults, **kwargs})

######### Channels
    def inq_channel_names(self, **kwargs):
        defaults = {"channel":"not specified"}
        return self.doit("MQCMD_INQUIRE_CHANNEL_NAMES", {**defaults, **kwargs})

    def inq_chl_names(self, **kwargs):
        defaults = {"channel":"not specified"}
        return self.doit("MQCMD_INQUIRE_CHANNEL_NAMES", {**defaults, **kwargs})

    def inq_channel_status(self, **kwargs):
        defaults = {"channel":"not specified"}
        return self.doit("MQCMD_INQUIRE_CHANNEL_STATUS", {**defaults, **kwargs})

#####################
    def alter_qmgr(self, **kwargs):
        return self.doit("MQCMD_CHANGE_Q_MGR", kwargs)

    def alter_q(self, **kwargs):
        return self.doit("MQCMD_CHANGE_Q", kwargs)

    def alter_ql(self, **kwargs):
        defaults = {"qtype":CMQC.MQQT_LOCAL}
        return self.doit("MQCMD_CHANGE_Q", kwargs)

    def alter_qa(self, **kwargs):
        defaults = {"qtype":CMQC.MQQT_ALIAS}
        return self.doit("MQCMD_CHANGE_Q", kwargs)

    def alter_qr(self  , **kwargs):
        defaults = {"qtype":CMQC.MQQT_REMOTE}
        return self.doit("MQCMD_CHANGE_Q", kwargs)

    def alter_qm(self, **kwargs):
        defaults = {"qtype":CMQC.MQQT_MODEL}
        return self.doit("MQCMD_CHANGE_Q", kwargs)
############
# define/delete
############
    def def_queue(self, **kwargs):
        defaults = {"qtype":CMQC.MQQT_LOCAL}
        return self.doit("MQCMD_CREATE_Q", kwargs)

    def def_q(self, **kwargs):
        defaults = {"qtype":CMQC.MQQT_LOCAL}
        return self.doit("MQCMD_CREATE_Q", kwargs)

    def del_q(self, **kwargs):
        defaults = {"qtype":CMQC.MQQT_LOCAL}
        return self.doit("MQCMD_DELETE_Q", kwargs)
####
    def def_channel(self, **kwargs):
        defaults = {"chltype":"SVRCONN"}
        return self.doit("MQCMD_CREATE_CHANNEL", {**defaults, **kwargs})

    def del_channel(self, **kwargs):
        defaults = {}
        return self.doit("MQCMD_DELETE_CHANNEL", {**defaults, **kwargs})

