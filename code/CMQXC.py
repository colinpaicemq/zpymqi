# Generated by h2py from /opt/mqm/inc/cmqxc.h

from struct import calcsize

# 64bit
if calcsize("P") == 8:
    MQACH_LENGTH_1 = 72
    MQACH_CURRENT_LENGTH = 72
    MQCD_LENGTH_4 = 1568
    MQCD_LENGTH_5 = 1584
    MQCD_LENGTH_6 = 1688
    MQCD_LENGTH_7 = 1792
    MQCD_LENGTH_8 = 1888
    MQCD_LENGTH_9 = 1912
    MQCD_LENGTH_10 = 1920
    MQCD_LENGTH_11 = 1984
    MQCD_LENGTH_12 = 1992
else:
    MQACH_LENGTH_1 = 68
    MQACH_CURRENT_LENGTH = 68
    MQCD_LENGTH_4 = 1540
    MQCD_LENGTH_5 = 1552
    MQCD_LENGTH_6 = 1648
    MQCD_LENGTH_7 = 1748
    MQCD_LENGTH_8 = 1840
    MQCD_LENGTH_9 = 1864
    MQCD_LENGTH_10 = 1876
    MQCD_LENGTH_11 = 1940
    MQCD_LENGTH_12 = 1944

MQCD_CURRENT_LENGTH = MQCD_LENGTH_12

MQACH_STRUC_ID = b"ACH "
MQACH_VERSION_1 = 1
MQACH_CURRENT_VERSION = 1
MQAXC_STRUC_ID = b"AXC "
MQAXC_VERSION_1 = 1
MQAXC_CURRENT_VERSION = 1
MQXE_OTHER = 0
MQXE_MCA = 1
MQXE_MCA_SVRCONN = 2
MQXE_COMMAND_SERVER = 3
MQXE_MQSC = 4
MQAXP_STRUC_ID = b"AXP "
MQAXP_VERSION_1 = 1
MQAXP_VERSION_2 = 2
MQAXP_CURRENT_VERSION = 2
MQXACT_EXTERNAL = 1
MQXACT_INTERNAL = 2
MQXPDA_NONE = b"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"\
              b"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"\
              b"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
MQXF_INIT = 1
MQXF_TERM = 2
MQXF_CONN = 3
MQXF_CONNX = 4
MQXF_DISC = 5
MQXF_OPEN = 6
MQXF_CLOSE = 7
MQXF_PUT1 = 8
MQXF_PUT = 9
MQXF_GET = 10
MQXF_DATA_CONV_ON_GET = 11
MQXF_INQ = 12
MQXF_SET = 13
MQXF_BEGIN = 14
MQXF_CMIT = 15
MQXF_BACK = 16
MQXF_STAT = 18
MQXF_CB = 19
MQXF_CTL = 20
MQXF_CALLBACK = 21
MQXF_SUB = 22
MQXF_SUBRQ = 23
MQCD_VERSION_1 = 1
MQCD_VERSION_2 = 2
MQCD_VERSION_3 = 3
MQCD_VERSION_4 = 4
MQCD_VERSION_5 = 5
MQCD_VERSION_6 = 6
MQCD_VERSION_7 = 7
MQCD_VERSION_8 = 8
MQCD_VERSION_9 = 9
MQCD_VERSION_10 = 10
MQCD_VERSION_11 = 11
MQCD_VERSION_12 = 12
MQCD_CURRENT_VERSION = MQCD_VERSION_12
MQCHT_SENDER = 1
MQCHT_SERVER = 2
MQCHT_RECEIVER = 3
MQCHT_REQUESTER = 4
MQCHT_ALL = 5
MQCHT_CLNTCONN = 6
MQCHT_SVRCONN = 7
MQCHT_CLUSRCVR = 8
MQCHT_CLUSSDR = 9
MQCOMPRESS_NOT_AVAILABLE = (-1)
MQCOMPRESS_NONE = 0
MQCOMPRESS_RLE = 1
MQCOMPRESS_ZLIBFAST = 2
MQCOMPRESS_ZLIBHIGH = 4
MQCOMPRESS_SYSTEM = 8
MQCOMPRESS_ANY = 0x0FFFFFFF
MQXPT_ALL = (-1)
MQXPT_LOCAL = 0
MQXPT_LU62 = 1
MQXPT_TCP = 2
MQXPT_NETBIOS = 3
MQXPT_SPX = 4
MQXPT_DECNET = 5
MQXPT_UDP = 6
MQPA_DEFAULT = 1
MQPA_CONTEXT = 2
MQPA_ONLY_MCA = 3
MQPA_ALTERNATE_OR_MCA = 4
MQCDC_SENDER_CONVERSION = 1
MQCDC_NO_SENDER_CONVERSION = 0
MQMCAT_PROCESS = 1
MQMCAT_THREAD = 2
MQNPMS_NORMAL = 1
MQNPMS_FAST = 2
MQSCA_REQUIRED = 0
MQSCA_OPTIONAL = 1
MQKAI_AUTO = (-1)
MQCAFTY_NONE = 0
MQCAFTY_PREFERRED = 1
MQCXP_STRUC_ID = b"CXP "
MQCXP_VERSION_1 = 1
MQCXP_VERSION_2 = 2
MQCXP_VERSION_3 = 3
MQCXP_VERSION_4 = 4
MQCXP_VERSION_5 = 5
MQCXP_VERSION_6 = 6
MQCXP_VERSION_7 = 7
MQCXP_CURRENT_VERSION = 7
MQXR2_PUT_WITH_DEF_ACTION = 0
MQXR2_PUT_WITH_DEF_USERID = 1
MQXR2_PUT_WITH_MSG_USERID = 2
MQXR2_USE_AGENT_BUFFER = 0
MQXR2_USE_EXIT_BUFFER = 4
MQXR2_DEFAULT_CONTINUATION = 0
MQXR2_CONTINUE_CHAIN = 8
MQXR2_SUPPRESS_CHAIN = 16
MQXR2_STATIC_CACHE = 0
MQXR2_DYNAMIC_CACHE = 32
MQCF_NONE = 0x00000000
MQCF_DIST_LISTS = 0x00000001
MQDXP_STRUC_ID = b"DXP "
MQDXP_VERSION_1 = 1
MQDXP_CURRENT_VERSION = 1
MQXDR_OK = 0
MQXDR_CONVERSION_FAILED = 1
MQPXP_STRUC_ID = b"PXP "
MQPXP_VERSION_1 = 1
MQPXP_CURRENT_VERSION = 1
MQDT_APPL = 1
MQDT_BROKER = 2
MQTXP_STRUC_ID = b"TXP "
MQTXP_VERSION_1 = 1
MQTXP_CURRENT_VERSION = 1
MQWDR_STRUC_ID = b"WDR "
MQWDR_VERSION_1 = 1
MQWDR_VERSION_2 = 2
MQWDR_CURRENT_VERSION = 2
MQWDR_LENGTH_1 = 124
MQWDR_LENGTH_2 = 136
MQWDR_CURRENT_LENGTH = 136
MQQMF_REPOSITORY_Q_MGR = 0x00000002
MQQMF_CLUSSDR_USER_DEFINED = 0x00000008
MQQMF_CLUSSDR_AUTO_DEFINED = 0x00000010
MQQMF_AVAILABLE = 0x00000020
MQWQR_STRUC_ID = b"WQR "
MQWQR_VERSION_1 = 1
MQWQR_VERSION_2 = 2
MQWQR_VERSION_3 = 3
MQWQR_CURRENT_VERSION = 3
MQWQR_LENGTH_1 = 200
MQWQR_LENGTH_2 = 208
MQWQR_LENGTH_3 = 212
MQWQR_CURRENT_LENGTH = 212
MQQF_LOCAL_Q = 0x00000001
MQQF_CLWL_USEQ_ANY = 0x00000040
MQQF_CLWL_USEQ_LOCAL = 0x00000080
MQWXP_STRUC_ID = b"WXP "
MQWXP_VERSION_1 = 1
MQWXP_VERSION_2 = 2
MQWXP_VERSION_3 = 3
MQWXP_CURRENT_VERSION = 3
MQWXP_PUT_BY_CLUSTER_CHL = 0x00000002
MQCLCT_STATIC = 0
MQCLCT_DYNAMIC = 1
MQXEPO_STRUC_ID = b"XEPO"
MQXEPO_VERSION_1 = 1
MQXEPO_CURRENT_VERSION = 1
MQXEPO_NONE = 0x00000000
MQXT_API_CROSSING_EXIT = 1
MQXT_API_EXIT = 2
MQXT_CHANNEL_SEC_EXIT = 11
MQXT_CHANNEL_MSG_EXIT = 12
MQXT_CHANNEL_SEND_EXIT = 13
MQXT_CHANNEL_RCV_EXIT = 14
MQXT_CHANNEL_MSG_RETRY_EXIT = 15
MQXT_CHANNEL_AUTO_DEF_EXIT = 16
MQXT_CLUSTER_WORKLOAD_EXIT = 20
MQXT_PUBSUB_ROUTING_EXIT = 21
MQXR_BEFORE = 1
MQXR_AFTER = 2
MQXR_CONNECTION = 3
MQXR_INIT = 11
MQXR_TERM = 12
MQXR_MSG = 13
MQXR_XMIT = 14
MQXR_SEC_MSG = 15
MQXR_INIT_SEC = 16
MQXR_RETRY = 17
MQXR_AUTO_CLUSSDR = 18
MQXR_AUTO_RECEIVER = 19
MQXR_CLWL_OPEN = 20
MQXR_CLWL_PUT = 21
MQXR_CLWL_MOVE = 22
MQXR_CLWL_REPOS = 23
MQXR_CLWL_REPOS_MOVE = 24
MQXR_END_BATCH = 25
MQXR_ACK_RECEIVED = 26
MQXR_AUTO_SVRCONN = 27
MQXR_AUTO_CLUSRCVR = 28
MQXR_SEC_PARMS = 29
MQXCC_OK = 0
MQXCC_SUPPRESS_FUNCTION = (-1)
MQXCC_SKIP_FUNCTION = (-2)
MQXCC_SEND_AND_REQUEST_SEC_MSG = (-3)
MQXCC_SEND_SEC_MSG = (-4)
MQXCC_SUPPRESS_EXIT = (-5)
MQXCC_CLOSE_CHANNEL = (-6)
MQXCC_REQUEST_ACK = (-7)
MQXCC_FAILED = (-8)
MQXUA_NONE = b"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
MQDCC_DEFAULT_CONVERSION = 0x00000001
MQDCC_FILL_TARGET_BUFFER = 0x00000002
MQDCC_INT_DEFAULT_CONVERSION = 0x00000004
MQDCC_SOURCE_ENC_NATIVE = 0x00000020
MQDCC_SOURCE_ENC_NORMAL = 0x00000010
MQDCC_SOURCE_ENC_REVERSED = 0x00000020
MQDCC_SOURCE_ENC_UNDEFINED = 0x00000000
MQDCC_TARGET_ENC_NATIVE = 0x00000200
MQDCC_TARGET_ENC_NORMAL = 0x00000100
MQDCC_TARGET_ENC_REVERSED = 0x00000200
MQDCC_TARGET_ENC_UNDEFINED = 0x00000000
MQDCC_NONE = 0x00000000
MQDCC_SOURCE_ENC_MASK = 0x000000F0
MQDCC_TARGET_ENC_MASK = 0x00000F00
MQDCC_SOURCE_ENC_FACTOR = 16
MQDCC_TARGET_ENC_FACTOR = 256

# Manually added
MQCXP_STRUC_ID_ARRAY = [b'C', b'X', b'P', b' ']
MQDXP_STRUC_ID_ARRAY = [b'D', b'X', b'P', b' ']
MQPXP_STRUC_ID_ARRAY = [b'P', b'X', b'P', b' ']
MQWDR_STRUC_ID_ARRAY = [b'W', b'D', b'R', b' ']
MQWQR_STRUC_ID_ARRAY = [b'W', b'Q', b'R', b' ']
MQWXP_STRUC_ID_ARRAY = [b'W', b'X', b'P', b' ']
MQXUA_NONE_ARRAY = [b'\0', b'\0', b'\0', b'\0', b'\0', b'\0', b'\0', b'\0',
                    b'\0', b'\0', b'\0', b'\0', b'\0', b'\0', b'\0', b'\0']

 