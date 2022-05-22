# file of constants which are not in the CMQC, and CMQCFC files
def py23long(x):
    # this will convert py2's int to py2's long without need to suffix it with L (which doesn't work in py3).
    # (note the recommended way is to use "from builtins import int",
    #     as per http://python-future.org/compatible_idioms.html#long-integers),
    # however "builtins" is in the "futures" package which should not be dependency of pymqi.
    return x + 0 * 0xffffffffffffffff

MQIA_USE_DEAD_LETTER_Q         = 234
MQCA_CUSTOM                    = 2119
MQCA_CLUS_CHL_NAME             = 2124
MQCA_QSG_CERT_LABEL            = 2131
MQIA_GROUP_UR                  = 221
MQIA_PUBSUB_CLUSTER            = 249
MQIA_CHLAUTH_RECORDS           = 248
MQIA_DEF_CLUSTER_XMIT_Q_TYPE   = 250
MQIA_PROT_POLICY_CAPABILITY    = 251
MQIA_REVERSE_DNS_LOOKUP        = 254
MQCA_CONN_AUTH                 = 2125
MQIA_ADVANCED_CAPABILITY       = 273
MQIA_QMGR_CFCONLOS             = 245
MQIACF_USAGE                   = 12
MQIACF_USAGE_TYPE              = 1157
MQIACF_Q_STATUS_TYPE           = 1103
MQIACF_UNCOMMITTED_MSGS        = 1027
MQIACF_Q_TIME_INDICATOR        = 1226
MQIACF_OLDEST_MSG_AGE          = 1227
MQIACF_SYSP_TYPE               = 1175
MQIACF_SYSP_ALLOC_UNIT         = 1203
MQIACF_SYSP_ALLOC_PRIMARY      = 1209
MQIACF_SYSP_ALLOC_SECONDARY    = 1210
MQIACF_SYSP_BLOCK_SIZE         = 1206
MQIACF_SYSP_ARCHIVE_RETAIN     = 1204
MQIACF_SYSP_ARCHIVE_WTOR       = 1205
MQIACF_SYSP_TIMESTAMP          = 1213
MQIACF_SYSP_CATALOG            = 1207
MQIACF_SYSP_COMPACT            = 1208
MQIACF_SYSP_ROUTING_CODE       = 1196
MQIACF_SYSP_PROTECT            = 1211
MQIACF_SYSP_QUIESCE_INTERVAL   = 1212
MQIACF_SYSP_SMF_STAT_TIME_SECS = 1442
MQIACF_SYSP_SMF_ACCT_TIME_MINS = 1443
MQIACF_SYSP_SMF_ACCT_TIME_SECS = 1444
MQCACF_EXCL_OPERATOR_MESSAGES  = 3205
MQIA_GROUP_UR                  = 221
MQIA_UR_DISP                   = 222
MQIA_COMM_INFO_TYPE            = 223
MQIA_CF_OFFLOAD                = 224
MQIA_CF_OFFLOAD_THRESHOLD1     = 225
MQIA_CF_OFFLOAD_THRESHOLD2     = 226
MQIA_CF_OFFLOAD_THRESHOLD3     = 227
MQIA_CF_SMDS_BUFFERS           = 228
MQIA_CF_OFFLDUSE               = 229
MQIA_MAX_RESPONSES             = 230
MQIA_RESPONSE_RESTART_POINT    = 231
MQIA_COMM_EVENT                = 232
MQIA_MCAST_BRIDGE              = 233
MQIA_USE_DEAD_LETTER_Q         = 234
MQIA_TOLERATE_UNPROTECTED      = 235
MQIA_SIGNATURE_ALGORITHM       = 236
MQIA_ENCRYPTION_ALGORITHM      = 237
MQIA_POLICY_VERSION            = 238
MQIA_ACTIVITY_CONN_OVERRIDE    = 239
MQIA_ACTIVITY_TRACE            = 240
MQIA_SUB_CONFIGURATION_EVENT   = 242
MQIA_XR_CAPABILITY             = 243
MQIA_CF_RECAUTO                = 244
MQIA_QMGR_CFCONLOS             = 245
MQIA_CF_CFCONLOS               = 246
MQIA_SUITE_B_STRENGTH          = 247
MQIA_CHLAUTH_RECORDS           = 248
MQIA_PUBSUB_CLUSTER            = 249
MQIA_DEF_CLUSTER_XMIT_Q_TYPE   = 250
MQIA_PROT_POLICY_CAPABILITY    = 251
MQIA_CERT_VAL_POLICY           = 252
MQIA_TOPIC_NODE_COUNT          = 253
MQIA_REVERSE_DNS_LOOKUP        = 254
MQIA_CLUSTER_PUB_ROUTE         = 255
MQIA_CLUSTER_OBJECT_STATE      = 256
MQIA_CLUSTER_PUB_ROUTE         = 255
MQIA_ADOPT_CONTEXT             = 260
MQIA_LDAP_SECURE_COMM          = 261
MQIA_DISPLAY_TYPE              = 262
MQIA_LDAP_AUTHORMD             = 263
MQIA_LDAP_NESTGRP              = 264
MQIA_AMQP_CAPABILITY           = 265
MQIA_AUTHENTICATION_METHOD     = 266
MQIA_KEY_REUSE_COUNT           = 267
MQIA_MEDIA_IMAGE_SCHEDULING    = 268
MQIA_MEDIA_IMAGE_INTERVAL      = 269
MQIA_MEDIA_IMAGE_LOG_LENGTH    = 270
MQIA_MEDIA_IMAGE_RECOVER_OBJ   = 271
MQIA_MEDIA_IMAGE_RECOVER_Q     = 272
MQIA_ADVANCED_CAPABILITY       = 273
MQIA_MAX_Q_FILE_SIZE           = 274
MQIA_STREAM_QUEUE_QOS          = 275
MQCA_CHLAUTH_DESC              = 2118
MQCACF_APPL_DESC               = 3174
MQIACF_CF_STATUS_TYPE          = 1135
MQIACF_CF_STATUS_SUMMARY       = 1136
MQIACF_CF_STATUS_CONNECT       = 1137
MQIACF_CF_STATUS_BACKUP        = 1138
MQIACF_CF_STATUS_SMDS          = 1333