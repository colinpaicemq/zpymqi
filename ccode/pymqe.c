/***********************************************************
Copyright 1991-1995 by Stichting Mathematisch Centrum, Amsterdam,
The Netherlands.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Stichting Mathematisch
Centrum or CWI or Corporation for National Research Initiatives or
CNRI not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

While CWI is the initial source for this software, a modified version
is made available by the Corporation for National Research Initiatives
(CNRI) at the Internet address ftp://ftp.python.org.

STICHTING MATHEMATISCH CENTRUM AND CNRI DISCLAIM ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH
CENTRUM OR CNRI BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

******************************************************************/
/*
 * Python MQ Extension. Low level mqi wrappers. These present a
 * low-level interface to the MQI 'C' library.
 *
 * Author: L. Smithson (lsmithson@open-networks.co.uk)
 * Author: Dariusz Suchojad (dsuch at zato.io)
 * z/OS changes: Colin Paice(colinpaice3@gmail.com)
 *
 * DISCLAIMER
 * You are free to use this code in any way you like, subject to the
 * Python & IBM disclaimers & copyrights. I make no representations
 * about the suitability of this software for any purpose. It is
 * provided "AS-IS" without warranty of any kind, either express or
 * implied. So there.
 *
 *
 */
#pragma langlvl(stdc99)
#ifdef __MVS__
  #ifdef __LP64__
    #define MQ_64_BIT
  #endif
#endif
#include "stddef.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "pyport.h"
#ifndef PRINTHEX_INCLUDE
#define PRINTHEX_INCLUDE
#include <ctype.h>
#define ASCII_STRING \
                "................" \
                "................" \
                " !\"#½%&'()*+,-./" \
                "0123456789:;<=>?" \
                "@ABCDEFGHIJKLMNO" \
                "PQRSTUVWXYZ[\\][_" \
                "'abcdefghijklmno" \
                "pqrstuvwxyz{ }~." \
                "................" \
                "................" \
                "................" \
                "................" \
                "................" \
                "................"
#define EBCDIC_STRING \
               "................"\
               "................"\
               "................"\
               "................"\
               " .........½.<(+|"\
               "&.........!$*);^"\
               "-/........Ý,%_>?"\
               ".........`:#@'=\""\
               ".abcdefghi......"\
               ".jklmnopqr......"\
               ".~stuvwxyz...í.."\
               ".............ù.."\
               "{ABCDEFGHI......"\
               "}JKLMNOPQR......"\
               "\\.STUVWXYZ......"\
               "0123456789......"\
// const char ascii_tab[256] = ASCII_STRING ;
/////////////////////////////
// now the data for dumping debug data
#define xCC        2
#define xPUTBUFFER 4
#define xGETBUFFER 8
#define xMQOD      16
#define xMQMD      32
#define xMQPMO     64
#define xMQGMO     128
#define xSUB       256
// #define xCRTMH     256
#define xSETMP     512
#define xINQMP    1024
#define xPCF      2048
#define xCONN     4096
//  logic for DEBUGIF
// If the bit for this field is on,
// and the "always" bit is on (bit 31)
//     or ( the bit is off ) and cc not zero )
// then ...
#define DEBUGIF(a,cc) \
  if ((pyOptions  & a)== a  \
      ||((pyOptions  & 1)== 1 && cc != 0)    \
     )
const char ascii_tab[256] = EBCDIC_STRING ;
//    char other_tab[256];
char * pEBCDIC = 0;

void printHex(FILE * pFile,void *buff, long length)
{
//if (pEBCDIC == 0)
//{
//  pEBCDIC = malloc(256);
//}
  char sepchars[2] = {' ',' '};
  long i, j;
  unsigned char *pbuff = (unsigned char*)buff;
  long bytesleft;
  char printchar;
  for( i=0; i<length; i+=16, pbuff+=16 )
  {
    if( (bytesleft=length-i) >= 16 )
            bytesleft=16;
    /**********************************************************
     * First print a row of hex chars.
     **********************************************************/
    fprintf(pFile,"%8.8X :",i);
    for( j=0; j<bytesleft; j++ ) {
            if(j % 4 == 0) fprintf(pFile," %2.2X",*(pbuff+j));
            else fprintf(pFile,"%2.2X",*(pbuff+j));
    }
    /**********************************************************
    * This pads the hex display with blanks if this is the
    * last row and it's less than 16 bytes long.
    **********************************************************/
    for( j=bytesleft; j<16; j++ ) {
            if(j % 4 == 0) fprintf(pFile,"   ");
            else fprintf(pFile,"  ");
    }
    /***********************************************************
     * Next print the characters themselves (if they're
     * printable).
     ********************************************************/
  fprintf(pFile," %c", sepchars[0]);
  for( j=0; j<bytesleft; j++ ) {
          printchar = (char) isprint(*(pbuff+j)) ? *(pbuff+j) : '.';
          fprintf(pFile,"%c",printchar);
  }
  /**********************************************************
   * ..and pad the last row with blanks if necessary.
   **********************************************************/
  for( j=bytesleft; j<16; j++ ) fprintf(pFile," ");
  fprintf(pFile,"%c",sepchars[1]);
  for( j=0; j<bytesleft; j++ ) {
          fprintf(pFile,"%c", ascii_tab[*(pbuff+j)]);
  }
  /**********************************************************
   * ..and pad the last row with blanks if necessary.
   **********************************************************/
  for( j=bytesleft; j<16; j++ ) fprintf(pFile," ");
  fprintf(pFile,"%c\n",sepchars[1]);
  }
  fflush(pFile);
  return;
}
#endif
 //  PyArg_ParseTuple is used to return get data from Python.
 //  Getting variable length character data eg s#
 //  returns the data, and its length.
 //  The length is returned a Py_ssize_t which is a long (8 chars on z/OS)
 //  MQ uses 4 byte lengths so you get messages
 //     INFORMATIONAL CCN3742 64-bit portability::
 //         possible loss of digits through conversion of long int type into int type.
 // when using (MQLONG)
 //
 // s is a string - utf8
 // y is a byte string
 #if PY_MAJOR_VERSION==2
   #define PSY "s"
 #else
   #define PSY "y"
 #endif
static char __version__[] = "1.13.0";  //  1.13 for z/OS support

static char pymqe_doc[] = " \
pymqe - A Python MQ Extension.  This presents a low-level Python \
interface to the MQI 'C' library. Its usage and conventions are more \
or less the same as the MQI 'C' language API. \
 \
MQI Connection & Object handles are passed around as Python \
longs. Structure parameters (such as MQGMO) are passed as Python \
string buffers. These buffers should be aligned & byte-ordered the \
same way the native 'C' compiler does. (Hint: use the Python struct.py \
package. See pymqi.py for an example). IN/OUT parameters are not\
updated in place, but returned as parameters in a tuple.\
\
All calls return the MQI completion code & reason as the last two\
elements of a tuple.\
\
The MQ verbs implemented here are:\
  MQCONN, MQCONNX, MQDISC, MQOPEN, MQCLOSE, MQPUT, MQPUT1, MQGET,\
  MQCMIT, MQBACK, MQBEGIN, MQINQ, MQSET, MQSUB, MQCRTMH, MQSETMP, MQINQMP\
\
The PCF MQAI call mqExecute is also implemented.\
\
The supported command levels (from 5.0 onwards) for the version of MQI \
linked with this module are available in the tuple pymqe.__mqlevels__. \
For a client build, pymqe.__mqbuild__ is set to the string 'client', otherwise \
it is set to 'server'.\
";


#include <cmqc.h>
#include <cmqxc.h>
#include <CMQSTRC.h>
#include "printmd.h"
#include "format.h"
#ifdef __MVS__
  #undef MQOD_CURRENT_LENGTH
  #if defined(MQ_64_BIT)
    #define MQOD_CURRENT_LENGTH            424
  #else
    #define MQOD_CURRENT_LENGTH            400
  #endif
  #undef MQCNO_CURRENT_LENGTH
  #if defined(MQ_64_BIT)
     #define MQCNO_CURRENT_LENGTH           256
  #else
     #define MQCNO_CURRENT_LENGTH           240
  #endif

  #undef MQIMPO_CURRENT_LENGTH
  #if defined(MQ_64_BIT)
   #define MQIMPO_CURRENT_LENGTH          64
  #else
    #define MQIMPO_CURRENT_LENGTH          60
  #endif

  #undef MQPMO_CURRENT_LENGTH
  #if defined(MQ_64_BIT)
    #define MQPMO_CURRENT_LENGTH           184
  #else
    #define MQPMO_CURRENT_LENGTH           176
  #endif
#endif
/*
 * 64bit suppport courtesy of Brent S. Elmer, Ph.D. (mailto:webe3vt@aim.com)
 *
 * On 64 bit machines when MQ is compiled 64bit, MQLONG is an int defined
 * in /opt/mqm/inc/cmqc.h or wherever your MQ installs to.
 **
 ** On z/OS MQLONG is of size 4 so an int, not a long, even when compiled with LP64.
 **
 * On 32 bit machines, MQLONG is a long and many other MQ data types are
 * set to MQLONG.
 */


/*
 * Setup features. MQ Version string is added to the pymqe dict so
 * that pymqi can find out what its been linked with.
 */
#ifdef MQCMDL_LEVEL_530
#define PYMQI_FEATURE_MQAI
#define PYMQI_FEATURE_SSL
#endif

#ifdef __MVS__
  #undef PYMQI_FEATURE_MQAI
  #define PYMQI_FEATURE_SSL
  #include <cmqcfc.h>

#else

  #ifdef PYMQI_FEATURE_MQAI
  #include <cmqcfc.h>
  #include <cmqbc.h>
  #endif
#endif

#define PY_SSIZE_T_CLEAN 1

#include "Python.h"
static PyObject *ErrorObj;

/*
 * MQI Structure sizes for the current supported MQ version are
 * defined here for convenience. This allows older versions of pymqi
 * to work with newer versions of MQI.
 */

#define PYMQI_MQCD_SIZEOF MQCD_CURRENT_LENGTH
#define PYMQI_MQOD_SIZEOF MQOD_CURRENT_LENGTH
#define PYMQI_MQMD_SIZEOF sizeof(MQMD2)
#define PYMQI_MQPMO_SIZEOF MQPMO_CURRENT_LENGTH
#define PYMQI_MQGMO_SIZEOF sizeof(MQGMO)
#ifdef PYMQI_FEATURE_SSL
#define PYMQI_MQSCO_SIZEOF sizeof(MQSCO)
#endif
#ifdef MQCMDL_LEVEL_700
#define PYMQI_MQSD_SIZEOF sizeof(MQSD)
#define PYMQI_MQSRO_SIZEOF sizeof(MQSRO)
#define PYMQI_MQCMHO_SIZEOF sizeof(MQCMHO)
#define PYMQI_MQSMPO_SIZEOF sizeof(MQSMPO)
#define PYMQI_MQIMPO_SIZEOF sizeof(MQIMPO)
#define PYMQI_MQPD_SIZEOF sizeof(MQPD)
#endif

/* Macro for cleaning up MQAI filters-related object.
*/
#define PYMQI_MQAI_FILTERS_CLEANUP \
  Py_XDECREF(filter_selector); \
  Py_XDECREF(filter_operator); \
  Py_XDECREF(filter_value); \
  Py_XDECREF(_pymqi_filter_type); \
  Py_XDECREF(filter);


/* Python 3 compatibility
 * Considerations:
 *   PyDict_GetItemString - safe to keep, the "String" here refers to C's char*
 *   PyErr_SetString - safe to keep, it will take C's char* and pyt it in Python's exception
 *                     (in Py3 it'll auto decode it with UTF-8, but in this file all instances are
 *                      hard-coded ascii anyway, so no issues.)
 *
 * Other functions renamed as per below between Py2 and Py3:
 */
#if PY_MAJOR_VERSION==2
// Py 2 Bytes - simples
#define Py23Bytes_FromString PyString_FromString // converts C char* to Py2 bytes/str
#define Py23Bytes_FromStringAndSize PyString_FromStringAndSize // converts C char* to Py2 bytes/str
#define Py23Bytes_AsString PyString_AsString  // converts Py2 bytes/str to C char*
#define Py23Bytes_Size PyString_Size  // get length of Py2 bytes/str
#define Py23Bytes_Check PyString_Check  // check object is Py2 bytes/str
#define Py23Object_Bytes PyObject_Str  // gets PyObject of Py2 type bytes/str
// Py 2 String (same as bytes) - simples
#define Py23Text_FromString PyString_FromString  // converts C char* to Py2 bytes/string
//#define Py23Text_Check PyString_Check  // check object is Py2 bytes/str
//#define Py23Text_AsString PyString_AsString  // converts Py2 bytes/str to C char*
//#define Py23Text_Size PyString_Size  // get length of Py2 bytes/str
static char* Py23BytesOrText_AsStringAndSize(PyObject *txtObj, MQLONG *outLen) {
  if(PyString_Check(txtObj)) {
    // bytes/str
    if (outLen != NULL) {
      (*outLen) = (MQLONG)PyString_Size(txtObj);
    }
    return PyString_AsString(txtObj);
  } else {
    return NULL;
  }
}
#define Py23BytesOrText_AsString PyString_AsString
#else
// Py 3 Bytes - simples
#define Py23Bytes_FromString PyBytes_FromString  // converts C char* to Py3 bytes
#define Py23Bytes_FromStringAndSize PyBytes_FromStringAndSize  // converts C char* to Py3 bytes
#define Py23Bytes_AsString PyBytes_AsString  // converts Py3 bytes to C char*
#define Py23Bytes_Size PyBytes_Size  // get length of Py3 bytes
#define Py23Bytes_Check PyBytes_Check  // check object is Py3 bytes
#define Py23Object_Bytes PyObject_ASCII  // gets PyObject of Py3 type bytes
// Py 3 String - tricky!
#define Py23Text_FromString PyUnicode_FromString  // converts C char* to Py3 str
//#define Py23Text_Check PyUnicode_Check  // check object is Py3 str

//static char* Py23Text_AsString(PyObject *txtObj) {  // converts Py3 str to C char* (works for UTF-8 only!)
//  PyObject *bytesObj;
//  if (PyUnicode_Check(txtObj)) {
//    bytesObj = PyUnicode_AsUTF8(txtObj);  // PyUnicode_AsUTF8 is not to be used on binary data! Text only.
//    char *bytes;
//    bytes = PyBytes_AsString(bytesObj);
//    return bytes;
//  } else {
//    return NULL;
//  }
//}

//static int Py23Text_Size(PyObject *txtObj) {  // get length of Py3 str (in 8-bit bytes) (works for UTF-8 only!)
//  PyObject *bytesObj;
//  if (PyUnicode_Check(txtObj)) {
//    bytesObj = PyUnicode_AsUTF8(txtObj);  // PyUnicode_AsUTF8 is not to be used on binary data! Text only.
//    int len = PyBytes_Size(bytesObj);
//    return len;
//  } else {
//    return NULL;
//  }
//}

static char* Py23BytesOrText_AsStringAndSize(PyObject *txtObj, MQLONG *outLen) {
  if(PyBytes_Check(txtObj)) {
    // bytes
    if (outLen != NULL) {
      (*outLen) = (MQLONG)PyBytes_Size(txtObj);
    }
    return PyBytes_AsString(txtObj);
  } else if (PyUnicode_Check(txtObj)) {
    PyObject *bytesObj;
    bytesObj = PyUnicode_AsUTF8String(txtObj);  // PyUnicode_AsUTF8 will return NULL on binary data! Text only.
    if (bytesObj != NULL) {
      if (outLen != NULL) {
        (*outLen) = (MQLONG)PyBytes_Size(bytesObj);
      }
      return PyBytes_AsString(bytesObj);
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

static char* Py23BytesOrText_AsString(PyObject *txtObj) {
  return Py23BytesOrText_AsStringAndSize(txtObj, NULL);
}

#endif /*PY_MAJOR_VERSION==2*/






/* ----------------------------------------------------- */


static int checkArgSize(Py_ssize_t given, Py_ssize_t expected, const char *name) {
  if (given != expected) {
    PyErr_Format(ErrorObj, "%s wrong size. Given: %lu, expected %lu", name, (unsigned long)given, (unsigned long)expected);
    return 1;
  }
  return 0;
}

static char pymqe_MQCONN__doc__[] =
"MQCONN(mgrName) \
 \
Calls the MQI MQCONN(mgrName) function to connect the Queue Manager \
specified by the string mgrName. The tuple (handle, comp, reason) is \
returned. Handle should be passed to subsequent calls to MQOPEN, etc.\
";

static PyObject * pymqe_MQCONN(PyObject *self, PyObject *args) {
  char *name;
  MQHCONN handle;
  MQLONG compCode, compReason;
  long pyOptions = 0;
  PyObject *nameObj;
  if (!PyArg_ParseTuple(args, "O|l", &nameObj,&pyOptions)) {  // pyOptions is optional
    return NULL;
  }
  name = Py23BytesOrText_AsString(nameObj);
  Py_BEGIN_ALLOW_THREADS
    MQCONN(name, &handle, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  return Py_BuildValue("(lll)", (long) handle, (long) compCode, (long) compReason);
  int x = ieantrcr();
}

/*
 * MQCONNX code courtesy of John OSullivan (mailto:jos@onebox.com)
 * SSL additions couresy of Brian Vicente (mailto:sailbv@netscape.net)
 * Connect options suggested by Jaco Smuts (mailto:JSmuts@clover.co.za)
 */

static char pymqe_MQCONNX__doc__[] =
#ifdef PYMQI_FEATURE_SSL
"MQCONNX(mgrName, options, mqcd, userpass, mqsco) \
 \
Calls the MQI MQCONNX(mgrName, options, mqcno) function to connect the Queue \
Manager specified by the string mgrName using options with the channel descriptor \
mqcd. The optional mqsco specifies SSL information. \
The tuple (handle, comp, reason) is returned. Handle should be \
passed to subsequent calls to MQOPEN, etc.\
 \
NOTE: The argument mqcd refers to the MQI MQCD structure, not MQCNO. \
";
#else
"MQCONNX(mgrName, options, mqcd) \
 \
Calls the MQI MQCONNX(mgrName, options, mqcno) function to connect the Queue \
Manager specified by the string mgrName using options with the channel descriptor \
mqcd. The optional mqsco specifies SSL information. \
The tuple (handle, comp, reason) is returned. Handle should be \
passed to subsequent calls to MQOPEN, etc.\
 \
NOTE: The argument mqcd refers to the MQI MQCD structure, not MQCNO. \
";
#endif

static PyObject * pymqe_MQCONNX(PyObject *self, PyObject *args) {
  char* name = NULL;
  MQHCONN handle;
  MQLONG comp_code, comp_reason;
  PMQCD mqcd = NULL;
  Py_ssize_t mqcd_buf_len = 0;
  MQCNO cno = {MQCNO_DEFAULT};
  __a2e_l(cno.StrucId,sizeof(cno.StrucId));
  PyObject* user_password = NULL;
  long pyOptions = 0;

  MQCSP csp = {MQCSP_DEFAULT};
  __a2e_l(csp.StrucId,sizeof(csp.StrucId));
  /*  Note: MQLONG is an int on 64 bit platforms and MQHCONN is an MQLONG
   */

  long options = MQCNO_NONE;

#ifdef PYMQI_FEATURE_SSL
    MQSCO *sco = NULL;
    Py_ssize_t sco_len = 0;
  #ifdef no23
    #if PY_MAJOR_VERSION==2
      if (!PyArg_ParseTuple(args, "slz#O|s#", &name, &options, &mqcd, &mqcd_buf_len, &user_password, &sco, &sco_len)) {
    #else
      if (!PyArg_ParseTuple(args, "ylz#O|y#", &name, &options, &mqcd, &mqcd_buf_len, &user_password, &sco, &sco_len)) {
    #endif
  #endif
    if (!PyArg_ParseTuple(args, PSY"lz#O|"PSY"#l", &name, &options, &mqcd, &mqcd_buf_len, &user_password,

        &sco, &sco_len,&pyOptions)) {
                                 // pyOptions is optional
          return 0;
       }
#else
  #ifdef no23
    #if PY_MAJOR_VERSION==2
      if (!PyArg_ParseTuple(args, "sls#", &name, &options, &mqcd, &mqcd_buf_len)) {
    #else
      if (!PyArg_ParseTuple(args, "yly#", &name, &options, &mqcd, &mqcd_buf_len)) {
    #endif
    if (!PyArg_ParseTuple(args, PSY"l"PSY"#|l", &name, &options, &mqcd, &mqcd_buf_len,&pyOptions)) {
  #endif
    return 0;
  }
#endif

  /*
   * Setup client connection fields appropriate to the version of MQ
   * we've been built with.
   */
#ifdef PYMQI_FEATURE_SSL
  cno.Version = MQCNO_VERSION_5;
  cno.SSLConfigPtr = sco;
#else
  cno.Version = MQCNO_VERSION_2;
#endif


  if(PyDict_Size(user_password)) {

    PyObject *user = PyDict_GetItemString(user_password, "user");
    PyObject *password = PyDict_GetItemString(user_password, "password");

    csp.AuthenticationType = MQCSP_AUTH_USER_ID_AND_PWD;
    csp.CSPUserIdPtr = Py23BytesOrText_AsStringAndSize(user, &csp.CSPUserIdLength);
    csp.CSPPasswordPtr = Py23BytesOrText_AsStringAndSize(password, &csp.CSPPasswordLength);
    if ((csp.CSPUserIdPtr == NULL && user!= NULL) || (csp.CSPPasswordPtr == NULL && password != NULL)) {
      PyErr_Format(ErrorObj, "Failed to parse user/password. Check they are bytes or string.");
      return NULL;
    }
    else {
      cno.SecurityParmsPtr = &csp;
    }
  }

  if(mqcd) {
    cno.ClientConnPtr = (MQCD *)mqcd;
  }
  cno.Options = (MQLONG)options;

  Py_BEGIN_ALLOW_THREADS
  MQCONNX(name, &cno, &handle, &comp_code, &comp_reason);
  DEBUGIF(xCC,comp_code)
    {
      printf("c:MQCONNX rc %d %rs %d\n", comp_code, comp_reason);
      printHex(stdout,name,48);
    }

  DEBUGIF(xCONN,comp_code)
    {
      printf("c:MQCONNX rc %d %rs %d\n", comp_code, comp_reason);
      printHex(stdout,&name,48);
      printf("c:MQCNO rc %d %rs %d\n", comp_code,  comp_reason);
      printHex(stdout,&cno,sizeof(MQCNO));
      printf("c:MQCSP\n");
      printHex(stdout,&csp,sizeof(MQCSP));
    }

  Py_END_ALLOW_THREADS
  return Py_BuildValue("(lll)", (long)handle, (long)comp_code, (long)comp_reason);
}



static char pymqe_MQDISC__doc__[] =
"MQDISC(handle) \
 \
Calls the MQI MQDISC(handle) function to disconnect the Queue \
Manager. The tuple (comp, reason) is returned. \
";

static PyObject * pymqe_MQDISC(PyObject *self, PyObject *args) {
  MQHCONN handle;
  MQLONG compCode, compReason;

  /*  Note: MQLONG is an int on 64 bit platforms and MQHCONN is an MQLONG
   */

  long lHandle;

  if (!PyArg_ParseTuple(args, "l", &lHandle)) {
    return NULL;
  }
  handle = (MQHCONN) lHandle;

  Py_BEGIN_ALLOW_THREADS
  MQDISC(&handle, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  return Py_BuildValue("(ll)", (long) compCode, (long) compReason);
}


static char pymqe_MQOPEN__doc__[] =
"MQOPEN(qMgr, qDesc, options) \
\
Calls the MQI MQOPEN(qMgr, qDesc, options) function to open the queue \
specified by the MQOD structure in the string buffer qDesc. QMgr is \
the Queue Manager handled returned by an earlier call to \
MQCONN. Options are the options for opening the Queue. \
 \
The tuple (qHandle, qDesc, comp, reason) is returned, where qHandle is \
the Queue Handle for the open queue and qDesc is the (possibly) \
updated copy of the Queue MQOD structure. \
 \
If qDesc is not the size expected for an MQOD structure, an exception \
is raised. \
" ;

static PyObject *pymqe_MQOPEN(PyObject *self, PyObject *args) {

  MQOD *qDescP;
  char *qDescBuffer;
  Py_ssize_t qDescBufferLength = 0;
  MQHOBJ qHandle;
  MQLONG compCode, compReason;

  long lOptions, lQmgrHandle;
  long pyOptions = 0;

#ifdef no23
#if PY_MAJOR_VERSION==2
  if (!PyArg_ParseTuple(args, "ls#l", &lQmgrHandle, &qDescBuffer,
#else
  if (!PyArg_ParseTuple(args, "ly#l", &lQmgrHandle, &qDescBuffer,
#endif
#endif
  if (!PyArg_ParseTuple(args, "l"PSY"#l|l", &lQmgrHandle, &qDescBuffer, // pyOptions is optional
            &qDescBufferLength, &lOptions,&pyOptions)) {
    return NULL;
  }
  qDescP = (MQOD *)qDescBuffer;
  if (checkArgSize(qDescBufferLength, PYMQI_MQOD_SIZEOF, "MQOD")) {
    return NULL;
  }
  Py_BEGIN_ALLOW_THREADS
    MQOPEN((MQHCONN)lQmgrHandle, qDescP, (MQLONG) lOptions, &qHandle, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  DEBUGIF(xMQOD,compCode)
    {
      printf("c:MQOD after open rc %d %rs %d\n", compCode, compReason);
      // display the options converted into strings using MQOO_STR function
      int i;
      int j = 1;  // used to 'and' with options to check flags if flag is set
      int iOptions = (int) lOptions; // we shift this left to test all options
      for (i = 0; i < 32 ;i ++)
      {
         int k = j  &  iOptions;
         if ( k   == j)
         printf("c:MQOD options %8.8x %s\n",j,MQOO_STR(k));
         j = j << 1; // shift left 1
      }
      printHex(stdout,qDescP,sizeof(MQOD));
      fflush(stdout);
    }

#ifdef no23
#if PY_MAJOR_VERSION==2
  return Py_BuildValue("(ls#ll)", (long) qHandle, qDescP,
#else
  return Py_BuildValue("(ly#ll)", (long) qHandle, qDescP,
#endif
#endif
  return Py_BuildValue("(l"PSY"#ll)", (long) qHandle, qDescP,
                       PYMQI_MQOD_SIZEOF, (long) compCode, (long) compReason);
}



static char pymqe_MQCLOSE__doc__[] =
"MQCLOSE(qMgr, qHandle, options) \
 \
Calls the MQI MQCLOSE(qMgr, qHandle, options) function to close the \
queue referenced by qMgr & qHandle. The tuple (comp, reason), is \
returned. \
";

static PyObject * pymqe_MQCLOSE(PyObject *self, PyObject *args) {
  MQHOBJ qHandle;
  MQLONG compCode, compReason;

  /* Note: MQLONG is an int on 64 bit platforms and MQHCONN and MQHOBJ are MQLONG
   */

  long lOptions, lQmgrHandle, lqHandle;

  if (!PyArg_ParseTuple(args, "lll", &lQmgrHandle, &lqHandle, &lOptions)) {
    return NULL;
  }
  qHandle = (MQHOBJ) lqHandle;

  Py_BEGIN_ALLOW_THREADS
  MQCLOSE((MQHCONN) lQmgrHandle, &qHandle, (MQLONG) lOptions, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  return Py_BuildValue("(ll)", (long) compCode, (long) compReason);
}


/*
 * Internal function that calls either PUT or PUT1 according to the
 * put1Flag arg
 */
static PyObject *mqputN(int put1Flag, PyObject *self, PyObject *args) {
  MQLONG compCode, compReason;
  char *mDescBuffer;
  Py_ssize_t mDescBufferLength = 0;
  MQMD2 *mDescP;
  char *putOptsBuffer;
  Py_ssize_t putOptsBufferLength = 0;
  MQPMO *pmoP;
  char *msgBuffer;
  Py_ssize_t msgBufferLength = 0;
  char *qDescBuffer;
  Py_ssize_t qDescBufferLength = 0;
  MQOD *qDescP = NULL;

  PyObject *rv;

  long lQmgrHandle, lqHandle;
  long pyOptions = 0;

  if (!put1Flag) {
    /* PUT call, expects qHandle for an open q */
#ifdef no23
#if PY_MAJOR_VERSION==2
    if (!PyArg_ParseTuple(args, "lls#s#s#", &lQmgrHandle, &lqHandle,
#else
    if (!PyArg_ParseTuple(args, "lly#y#y#", &lQmgrHandle, &lqHandle,
#endif
#endif
    if (!PyArg_ParseTuple(args, "ll"PSY"#"PSY"#"PSY"#|l", &lQmgrHandle, &lqHandle, // pyOptions is optional
              &mDescBuffer, &mDescBufferLength,
              &putOptsBuffer, &putOptsBufferLength,
              &msgBuffer, &msgBufferLength,&pyOptions)) {
      return NULL;
    }
  } else {
    /* PUT1 call, expects od for a queue to be opened */
#ifdef no23
#if PY_MAJOR_VERSION==2
    if (!PyArg_ParseTuple(args, "ls#s#s#s#", &lQmgrHandle,
#else
    if (!PyArg_ParseTuple(args, "ly#y#y#y#", &lQmgrHandle,
#endif
#endif
    if (!PyArg_ParseTuple(args, "l"PSY"#"PSY"#"PSY"#"PSY"#|l", &lQmgrHandle,
              &qDescBuffer, &qDescBufferLength,
              &mDescBuffer, &mDescBufferLength,
              &putOptsBuffer, &putOptsBufferLength,
              &msgBuffer, &msgBufferLength,&pyOptions)) { // pyOptions is optional
      return NULL;

    }
    if (checkArgSize(qDescBufferLength, PYMQI_MQOD_SIZEOF, "MQOD")) {
      return NULL;
    }
    qDescP = (MQOD *)qDescBuffer;
  }

  if (checkArgSize(mDescBufferLength, PYMQI_MQMD_SIZEOF, "MQMD")) {
    return NULL;
  }
  mDescP = (MQMD2 *)mDescBuffer;

  if (checkArgSize(putOptsBufferLength, PYMQI_MQPMO_SIZEOF, "MQPMO")) {
    return NULL;
  }
  pmoP = (MQPMO *)putOptsBuffer;
  if (!put1Flag) {
    Py_BEGIN_ALLOW_THREADS
    MQPUT((MQHCONN) lQmgrHandle, (MQHOBJ) lqHandle, mDescP, pmoP, (MQLONG) msgBufferLength, msgBuffer,
      &compCode, &compReason);
    Py_END_ALLOW_THREADS
  } else {
    Py_BEGIN_ALLOW_THREADS
    MQPUT1((MQHCONN) lQmgrHandle, qDescP, mDescP, pmoP, (MQLONG) msgBufferLength, msgBuffer,
       &compCode, &compReason);
    Py_END_ALLOW_THREADS
  }
  DEBUGIF(xCC,compCode)
  {
    printf("c:MQPUT rc %d rs %s (%d)\n",compCode,MQRCCF_STR(compReason),  compReason);
  }
  DEBUGIF(xMQPMO,compCode)
  {
    printf("c:MQPUT PMO\n");
    printHex(stdout,pmoP,sizeof(MQPMO)       );
  }
  DEBUGIF(xMQMD,compCode)
  {
    printf("MQPUT* MD\n");
    printMD(mDescP);
  }
  DEBUGIF(xPUTBUFFER,compCode)
  {
    printf("c:MQPUT buffer\n");
    printHex(stdout,msgBuffer,msgBufferLength);
  }
  MQCHAR8 mqADMIN ={MQFMT_ADMIN_ARRAY}; // the original version ( ascii)
  __a2e_l(mqADMIN,sizeof(mqADMIN)); //  convert to EBCDIC
  if (memcmp(  mDescP -> Format , mqADMIN ,sizeof(mqADMIN)   ) == 0)
  DEBUGIF(xPCF,compCode)  // format the data in PCF format
  {
    MQCFH * pMQCFH = (MQCFH *) msgBuffer;
    printf("c:MQPUT PCF rc %d rs %d\n",compCode, compReason);
    printf("c:MQPUT length %d\n",msgBufferLength  );
    printf("c:MQPUT PCF.Command %d %s\n",pMQCFH -> Command,MQCMD_STR(pMQCFH -> Command));
    printf("c:MQPUT PCF.Count  %d\n",pMQCFH -> ParameterCount);
    int iLoop;
    char * pEnd = msgBuffer + msgBufferLength; // in case we have been given bad data
    MQCFST * pMQCFST;
    char * pData = (char *)  pMQCFH + pMQCFH -> StrucLength ; // first data section
    for (iLoop = 0;iLoop < pMQCFH -> ParameterCount;iLoop ++)
    {
      long lData;
      if ( pData > pEnd)
      {
        printf("c:*** data has run off the end of the buffer\n\n");
        break;
      }
      pMQCFST = (MQCFST * ) pData;
      lData = pMQCFST -> StrucLength;
      if ( lData > pEnd - pData)
         lData = pEnd - pData;
      if (lData > 1024) lData  = 1024;
      printHex(stdout,pData,lData );
      pData += pMQCFST-> StrucLength;
    }
  fflush(stdout);
  }
#ifdef no23
#if PY_MAJOR_VERSION==2
  rv = Py_BuildValue("(s#s#ll)",
#else
  rv = Py_BuildValue("(y#y#ll)",
#endif
#endif
  rv = Py_BuildValue("("PSY"#"PSY"#ll)",
              mDescP, (Py_ssize_t)PYMQI_MQMD_SIZEOF,
              pmoP, (Py_ssize_t)PYMQI_MQPMO_SIZEOF,
              (long) compCode, (long) compReason);
  return rv;
}


static char pymqe_MQPUT__doc__[] =
"MQPUT(qMgr, qHandle, mDesc, options, msg) \
 \
Calls the MQI MQPUT(qMgr, qHandle, mDesc, putOpts, msg) function to \
put msg on the queue referenced by qMgr & qHandle. The message msg may \
contain embedded nulls. mDesc & putOpts are string buffers containing \
a MQMD Message Descriptor structure and a MQPMO Put Message Option \
structure. \
 \
The tuple (mDesc, putOpts, comp, reason) is returned, where mDesc & \
putOpts are the (possibly) updated copies of the MQMD & MQPMO \
structures. \
 \
If either mDesc or putOpts are the wrong size, an exception is raised. \
";

static PyObject *pymqe_MQPUT(PyObject *self, PyObject *args) {
  return mqputN(0, self, args);
}


static char pymqe_MQPUT1__doc__[] =
"MQPUT1(qMgr, qDesc, mDesc, options, msg) \
 \
Calls the MQI MQPUT1(qMgr, qDesc, mDesc, putOpts, msg) function to put \
the message msg on the queue referenced by qMgr & qDesc. The message \
msg may contain embedded nulls. mDesc & putOpts are string buffers \
containing a MQMD Message Descriptor structure and a MQPMO Put Message \
Option structure. \
 \
The tuple (mDesc, putOpts, comp, reason) is returned, where mDesc & \
putOpts are the (possibly) updated copies of the MQMD & MQPMO \
structures. \
 \
MQPUT1 is the optimal way to put a single message on a queue. It is \
equivalent to calling MQOPEN, MQPUT and MQCLOSE. \
 \
If any of qDesc, mDesc or putOpts are the wrong size, an exception is \
raised. \
";

static PyObject *pymqe_MQPUT1(PyObject *self, PyObject *args) {
  return mqputN(1, self, args);
}


static char pymqe_MQGET__doc__[] =
"MQGET(qMgr, qHandle, mDesc, getOpts, maxlen) \
 \
Calls the MQI MQGET(qMgr, qHandle, mDesc, getOpts, maxlen) function to \
get a message from the queue referred to by qMgr & qHandle.  mDesc & \
getOpts are string buffers containing a MQMD Message Descriptor and a \
MQGMO Get Message Options structure. maxlen specified the maximum \
length of messsage to read from the queue. If the message length \
exceeds maxlen, the the behaviour is as defined by MQI. \
 \
The tuple (msg, mDesc, getOpts, actualLen, comp, reason) is returned, \
where msg is a string containing the message read from the queue and \
mDesc & getOpts are copies of the (possibly) updated MQMD & MQGMO \
structures. actualLen is the actual length of the message in the \
Queue. If this is bigger than maxlen, then as much data as possible is \
copied into the return buffer. In this case, the message may or may \
not be removed from the queue, depending on the MQGMO options. See the \
MQI APG/APR for more details. \
 \
If mDesc or getOpts are the wrong size, an exception is raised. \
";

static PyObject *pymqe_MQGET(PyObject *self, PyObject *args) {
  MQLONG compCode, compReason;
  char *mDescBuffer;
  Py_ssize_t mDescBufferLength = 0;
  MQMD2 *mDescP;
  char *getOptsBuffer;
  Py_ssize_t getOptsBufferLength = 0;
  MQGMO *gmoP;
  long maxLength, returnLength;
  MQLONG actualLength;
  char *msgBuffer;
  PyObject *rv;
  long pyOptions= 0;  // for passing eebug information in

  long lQmgrHandle, lqHandle;
#ifdef no23
#if PY_MAJOR_VERSION==2
  if (!PyArg_ParseTuple(args, "lls#s#l", &lQmgrHandle, &lqHandle,
#else
  if (!PyArg_ParseTuple(args, "lly#y#l", &lQmgrHandle, &lqHandle,
#endif
#endif
  if (!PyArg_ParseTuple(args, "ll"PSY"#"PSY"#l|l", &lQmgrHandle, &lqHandle,
            &mDescBuffer, &mDescBufferLength,
            &getOptsBuffer, &getOptsBufferLength, &maxLength,&pyOptions)) {
    return NULL;
  }
  if (checkArgSize(mDescBufferLength, PYMQI_MQMD_SIZEOF, "MQMD")) {
    return NULL;
  }

  mDescP = (MQMD2 *)mDescBuffer;

  if (checkArgSize(getOptsBufferLength, PYMQI_MQGMO_SIZEOF, "MQGMO")) {
    return NULL;
  }
  gmoP = (MQGMO *)getOptsBuffer;

  /* Allocate temp. storage for message */
  if (!(msgBuffer = malloc(maxLength))) {
    PyErr_SetString(ErrorObj, "No memory for message");
    return NULL;
  }
  actualLength = 0;
  struct timeval timeBefore;
  gettimeofday( &timeBefore,0 );
  Py_BEGIN_ALLOW_THREADS
  MQGET((MQHCONN) lQmgrHandle, (MQHOBJ) lqHandle, mDescP, gmoP, (MQLONG) maxLength, msgBuffer, &actualLength,
    &compCode, &compReason);
  Py_END_ALLOW_THREADS

  DEBUGIF(xCC,compCode)
  {
    struct timeval timeNow;
    gettimeofday( &timeNow,0 );
    char dateTime[21];
    struct tm *timeptr;
    double duration = timeNow.tv_sec - timeBefore.tv_sec + (timeNow.tv_usec - timeBefore.tv_usec)/1000000.0;
    timeptr = localtime(&timeNow.tv_sec);
    strftime((char *) &dateTime, sizeof(dateTime) - 1, "%H:%M:%S", timeptr);
    printf("C:MQGET0 Time %s.%6.6d duration %9.6f\n", dateTime,timeNow.tv_usec,duration);
    printf("c:MQGET1 rc %d rs %d\n",compCode, compReason);
  }
  DEBUGIF(xMQPMO,compCode)
  {
    printHex(stdout,gmoP,sizeof(MQGMO));
  }
  DEBUGIF(xMQMD,compCode)
  {
//  printHex(stdout,mDescP,sizeof(MQMD2));
    printf("MQGET MD\n");
    printMD(mDescP);
  }
  DEBUGIF(xGETBUFFER,compCode)
  {
    printf("c:MQGET2 data (length %d)\n",actualLength);
    printHex(stdout,msgBuffer,  actualLength);
    fflush(stdout);
  }
  MQCHAR8 mqADMIN ={MQFMT_ADMIN_ARRAY}; // the original version ( ascii)
  __a2e_l(mqADMIN,sizeof(mqADMIN)); //  convert to EBCDIC

  if (memcmp(  mDescP -> Format , mqADMIN ,sizeof(mqADMIN)   ) == 0)
  DEBUGIF(xPCF,compCode)  // format the data in PCF format
  {
    MQCFH * pMQCFH = (MQCFH *) msgBuffer;
    printf("c:MQGET3  PCF rc %d rs %d\n",compCode, compReason);
    if (compReason == 0) // do not print if non zero
    {
      printf("c:MQGET PCF.Type      %d %s\n",pMQCFH -> Type   ,MQCFT_STR(pMQCFH -> Type));
      printf("c:MQGET PCF.Command   %d %s\n",pMQCFH -> Command,MQCMD_STR(pMQCFH -> Command));
      printf("c:MQGET PCF.MsgSeq    %d\n",pMQCFH -> MsgSeqNumber                        );
      if (pMQCFH -> Control == MQCFC_LAST)
        printf("c:MQGET PCF.Control   1 Last message in set\n");
      else
        printf("c:MQGET PCF.Control   0 Not last message in set\n");
      printf("c:MQGET PCF.CompCode  %d\n",pMQCFH -> CompCode);
      printf("c:MQGET PCF.Reason    %d %s\n",pMQCFH -> Reason,MQRCCF_STR(pMQCFH -> Reason));
      printf("c:MQGET PCF.ParmCount %d\n",pMQCFH -> ParameterCount );
      int iLoop;
      MQCFST * pMQCFST;
      char * pData = (char *)  pMQCFH + pMQCFH -> StrucLength ;
      for (iLoop = 0;iLoop < pMQCFH -> ParameterCount;iLoop ++)
      {
        long  lData;
        pMQCFST = (MQCFST * ) pData;
        lData = pMQCFST -> StrucLength;
        if (lData > 1024) lData  = 1024;
        printHex(stdout,pData,lData );
        pData += pMQCFST-> StrucLength;
      }
    }
    fflush(stdout);
  }

  /*
   * Message may be too big for caller's buffer, so only copy in as
   * much data as will fit, but return the actual length of the
   * message. Thanks to Maas-Maarten Zeeman for this fix.
   */
  if(actualLength >= maxLength) {
    returnLength = maxLength;
  } else {
    returnLength = actualLength;
  }

#ifdef no23
#if PY_MAJOR_VERSION==2
  rv = Py_BuildValue("(s#s#s#lll)", msgBuffer, (int) returnLength,
#else
  rv = Py_BuildValue("(y#y#y#lll)", msgBuffer, (int) returnLength,
#endif
#endif
  rv = Py_BuildValue("("PSY"#"PSY"#"PSY"#lll)", msgBuffer, (int) returnLength,
             mDescP, PYMQI_MQMD_SIZEOF, gmoP, PYMQI_MQGMO_SIZEOF,
             (long) actualLength, (long) compCode, (long) compReason);
  free(msgBuffer);
  return rv;
}

#ifndef __MVS__

static char pymqe_MQBEGIN__doc__[] =
"MQBEGIN(handle)  \
\
Calls the MQI MQBEGIN(handle) function to begin a new global \
transaction. This is used in conjunction with MQ coodinated \
Distributed Transactions and XA resources. \
 \
The tuple (comp, reason) is returned.\
";

static PyObject * pymqe_MQBEGIN(PyObject *self, PyObject *args) {
  MQLONG compCode, compReason;
  MQBO beginOpts = {MQBO_DEFAULT};

  long lHandle;

  if (!PyArg_ParseTuple(args, "l", &lHandle)) {
    return NULL;
  }
  Py_BEGIN_ALLOW_THREADS
  MQBEGIN((MQHCONN) lHandle, &beginOpts, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  return Py_BuildValue("(ll)", (long) compCode, (long) compReason);
}

#endif /* mvs */

static char pymqe_MQCMIT__doc__[] =
"MQCMIT(handle) \
 \
Calls the MQI MQCMIT(handle) function to commit any pending gets or \
puts in the current unit of work. The tuple (comp, reason) is \
returned. \
";

static PyObject * pymqe_MQCMIT(PyObject *self, PyObject *args) {
  MQLONG compCode, compReason;

  long lHandle;

  if (!PyArg_ParseTuple(args, "l", &lHandle)) {
    return NULL;
  }
  Py_BEGIN_ALLOW_THREADS
  MQCMIT((MQHCONN) lHandle, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  return Py_BuildValue("(ll)", (long) compCode, (long) compReason);
}

static char pymqe_MQBACK__doc__[] =
"MQBACK(handle) \
 \
Calls the MQI MQBACK(handle) function to backout any pending gets or \
puts in the current unit of work. The tuple (comp, reason) is \
returned. \
";

static PyObject * pymqe_MQBACK(PyObject *self, PyObject *args) {
  MQLONG compCode, compReason;

  long lHandle;

  if (!PyArg_ParseTuple(args, "l", &lHandle)) {
    return NULL;
  }
  Py_BEGIN_ALLOW_THREADS
  MQBACK((MQHCONN) lHandle, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  return Py_BuildValue("(ll)", (long) compCode, (long) compReason);
}


/*
 * MQINQ Interface. Only supports Inquire of single attribute.
 */
static char pymqe_MQINQ__doc__[] =
"MQINQ(qMgr, handle, selector) \
\
Calls MQINQ with a single attribute. Returns the value of that \
attribute. \
";

/*
 * This figure plucked out of thin air + 1 added for null. Doesn't
 * look like there's anything bigger than this in cmqc.h. I'm sure
 * someone will tell me if I'm wrong.
 */
#define MAX_CHARATTR_LENGTH 257

static PyObject *pymqe_MQINQ(PyObject *self, PyObject *args) {
  MQLONG compCode, compReason;
  MQLONG selectorCount = 1;
  MQLONG selectors[1];
  MQLONG intAttrCount = 0;
  MQLONG intAttrs[1];
  MQLONG charAttrCount = 0;
  MQCHAR charAttrs[MAX_CHARATTR_LENGTH];
  PyObject *retVal = NULL;
  long pyOptions = 0;
  long lQmgrHandle, lObjHandle, lSelectors[1];

  if (!PyArg_ParseTuple(args, "lll|l", &lQmgrHandle, &lObjHandle, lSelectors,&pyOptions)) { // pyOptions is optional
    return NULL;
  }
  selectors[0] = (MQLONG) lSelectors[0];

  if ((selectors[0] >= MQIA_FIRST) && (selectors[0] <= MQIA_LAST)) {
    intAttrCount = 1;
  } else {
    charAttrCount = sizeof(charAttrs) -1;
    memset(charAttrs, 0, sizeof(charAttrs));
  }
  Py_BEGIN_ALLOW_THREADS
  MQINQ((MQHCONN) lQmgrHandle, (MQHOBJ) lObjHandle, selectorCount, selectors,
        intAttrCount, intAttrs, charAttrCount, charAttrs, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  char  * sValue = formatValue(selectors[0]);
  DEBUGIF(xCC,compCode)
    {
      printf("c:MQINQ selector %d(%s) rc %d rs %d\n", selectors[0], sValue, compCode, compReason);
    }

  if (  compReason == MQRC_SELECTOR_ERROR
      ||compReason == MQRC_SELECTOR_NOT_FOR_TYPE )
  {
    //  printf(">>>>> %s Not supported",sValue);
    compReason = 0;
    compCode   = 0;
    retVal = Py_BuildValue("(" PSY PSY "ll)", NULL, sValue, (long) compCode, (long) compReason);
  }
  else
  if (intAttrCount) {
      DEBUGIF(xCC,compCode)
      {
        printf("c:MQINQ Integer type %d(%s) value %d\n",selectors[0], sValue, intAttrs[0]);
      }
    retVal = Py_BuildValue("(l"PSY"ll)", (long) intAttrs[0],sValue, (long) compCode, (long) compReason);
  } else {
       DEBUGIF(xCC,compCode)
      {
        printf("c:MQINQ char type %s value %ld\n", sValue);
        printHex(stdout,charAttrs,charAttrCount);
      }
#ifdef no23
#if PY_MAJOR_VERSION==2
    retVal = Py_BuildValue("(sll)", charAttrs, (long) compCode, (long) compReason);
#else
    retVal = Py_BuildValue("(yll)", charAttrs, (long) compCode, (long) compReason);
#endif
#endif
    retVal = Py_BuildValue("("PSY PSY"ll)", charAttrs, sValue, (long) compCode, (long) compReason);
  }
  free ( sValue);
  return retVal;
}

/*
 * MQSET Interface. Only supports Set of single attribute.
 */
static char pymqe_MQSET__doc__[] =
"MQSET(qMgr, handle, selector, val) \
 \
Calls MQSET with a single selector and attribute. \
";

static PyObject *pymqe_MQSET(PyObject *self, PyObject *args) {
  MQLONG compCode, compReason;
  MQLONG selectorCount = 1;
  MQLONG selectors[1];
  MQLONG intAttrCount = 0;
  MQLONG intAttrs[1];
  MQLONG charAttrCount = 0;
  MQCHAR charAttrs[MAX_CHARATTR_LENGTH];
  PyObject *attrArg = NULL;
  long pyOptions = 0;

  long lQmgrHandle, lObjHandle, lSelectors[1];

  if (!PyArg_ParseTuple(args, "lllO|l", &lQmgrHandle, &lObjHandle, lSelectors, &attrArg,&pyOptions)) {
    return NULL;
  }
  selectors[0] = (MQLONG) lSelectors[0];

  if ((selectors[0] >= MQIA_FIRST) && (selectors[0] <= MQIA_LAST)) {
    if (!PyLong_Check(attrArg)) {
      PyErr_SetString(ErrorObj, "Arg is not a long integer");
      return NULL;
    }
    intAttrs[0] =(MQLONG) PyLong_AsLong(attrArg);
    intAttrCount = 1;
  } else {
    if (!Py23Bytes_Check(attrArg)) {
      PyErr_SetString(ErrorObj, "Arg is not a byte-string");
      return NULL;
    }
    strncpy(charAttrs, Py23Bytes_AsString(attrArg), MAX_CHARATTR_LENGTH);
    charAttrCount = (MQLONG)strlen(charAttrs);
  }

  Py_BEGIN_ALLOW_THREADS
  MQSET((MQHCONN) lQmgrHandle, (MQHOBJ) lObjHandle, selectorCount, selectors,
        intAttrCount, intAttrs, charAttrCount, charAttrs, &compCode, &compReason);
  Py_END_ALLOW_THREADS
  char  * sValue = formatValue(selectors[0]);
  if ( compReason > 0)
  {
    printf("c:rc %d cc %d\n",compCode,compReason);
    if (intAttrs > 0)
    {
      printf("c:MQSET INT %d %d\n",selectors[0],intAttrs[0]);
    }
    if (charAttrCount > 0)
    {
      printf("c:MQSET CHAR %d %*.*s\n",selectors[0],charAttrCount,charAttrCount,charAttrs[0]);
    }
  }
  return Py_BuildValue("(ll)", (long) compCode, (long) compReason);
}

/* Publish/subscribe - Hannes Wagener 2011 */
#ifdef MQCMDL_LEVEL_700

static char pymqe_MQSUB__doc__[] =
"MQSUB(connectionHandle, sd, objectHandle) \
 \
Calls the MQI MQSUB(connectionHandle, subDesc, objectHandle) \
";

static PyObject * pymqe_MQSUB(PyObject *self, PyObject *args) {
  MQSD *subDescP;
  MQHOBJ subHandle;
  MQHOBJ objectHandle;
  MQLONG compCode, compReason;
  PyObject *rv;
  long pyOptions = 0;

  char *subDescBuffer;
  Py_ssize_t subDescBufferLength = 0;

  long lQmgrHandle;

#ifdef no23
#if PY_MAJOR_VERSION==2
  if (!PyArg_ParseTuple(args, "ls#l", &lQmgrHandle,
#else
  if (!PyArg_ParseTuple(args, "ly#l", &lQmgrHandle,
#endif
#endif
  long lObjectHandle;
  if (!PyArg_ParseTuple(args, "l"PSY"#l|l", &lQmgrHandle,
            &subDescBuffer, &subDescBufferLength,
            &lObjectHandle,&pyOptions)) { // pyOptions is optional
    printf("C:1345  parse failed!\n");
    return NULL;
  }
  if (checkArgSize(subDescBufferLength, PYMQI_MQSD_SIZEOF, "MQSD")) {
    return NULL;
  }
  objectHandle = (MQHOBJ) lObjectHandle;
  subDescP = (MQSD *)subDescBuffer;

  MQHOBJ inHObj;
  inHObj = objectHandle;
  Py_BEGIN_ALLOW_THREADS
  MQSUB((MQHCONN) lQmgrHandle, subDescP, &objectHandle, &subHandle,
    &compCode, &compReason);

//printf("c:SubLevel A %d\n", subDescP -> SubLevel);
  Py_END_ALLOW_THREADS
  DEBUGIF(xCC,compCode)
    {
      printf("c:MQSUB, cc %d rs %d\n",compCode, compReason);
    }
  DEBUGIF(xSUB,compCode)
   {
      printf("c:MQSUB input hObj %d output hObj %d\n",inHObj, objectHandle);
      printf("c:MQSUB MQSD\n");
      printHex(stdout, subDescP, sizeof(MQSD));
      printf("c:MQSUB ObjectName    \n");
      printHex(stdout, subDescP-> ObjectName,48);
      printf("c:MQSUB ObjectString length %d ccsid %d\n",
              subDescP-> ObjectString.VSLength,subDescP-> ObjectString.VSCCSID);
      if (subDescP->ObjectString.VSLength > 0 && subDescP-> ObjectString.VSPtr > 0)
        printHex(stdout, subDescP-> ObjectString.VSPtr,subDescP-> ObjectString.VSLength);
      if (subDescP->ResObjectString.VSLength > 0   & subDescP-> ResObjectString.VSPtr >0 )
      {
        printf("c:MQSUB ResObjectString\n");
        printHex(stdout, subDescP->ResObjectString  .VSPtr,subDescP->ResObjectString.VSLength);
      }
      printf("c:MQSUB SubName Length %d \n",subDescP-> SubName    .VSLength);
      printHex(stdout, subDescP-> SubName     .VSPtr,subDescP-> SubName     .VSLength);

      int i;
      int j = 1;  // used to and with options to check flags
      for (i = 0; i < 32 ;i ++)
      {
         int k = j  &  subDescP-> Options;
         if ( k   == j)
         printf("c:MQSD SO.options %8.8x %s\n",j,MQSO_STR(k));
         j = j << 1; // shift left 1
      }
      fflush(stdout);
    }
#ifdef no23
#if PY_MAJOR_VERSION==2
  rv = Py_BuildValue("(s#llll)", subDescP, PYMQI_MQSD_SIZEOF, objectHandle, subHandle,
#else
  rv = Py_BuildValue("(y#llll)", subDescP, PYMQI_MQSD_SIZEOF, objectHandle, subHandle,
#endif
#endif
  rv = Py_BuildValue("("PSY"#llll)", subDescP, PYMQI_MQSD_SIZEOF, objectHandle, subHandle,
             (long) compCode, (long) compReason);
  return rv;

}

#endif /* MQCMDL_LEVEL_700 */

#ifdef __MVS__
  #define PYMQI_FEATURE_MH
#else
  #ifdef PYMQI_FEATURE_MQAI
    #define PYMQI_FEATURE_MH
  #endif
#endif

#ifdef PYMQI_FEATURE_MH
/* Message properties and selectors - start */

#ifdef MQCMDL_LEVEL_700

static char pymqe_MQCRTMH__doc__[] =
"MQCRTMH(conn_handle, cmho) \
 \
Calls the MQI's MQCRTMH function \
";

static PyObject* pymqe_MQCRTMH(PyObject *self, PyObject *args) {

  long    conn_handle; // if comes from Python as a 8 byte long
  MQHCONN  hConn;      // an int
  char *cmho_buffer;
  Py_ssize_t cmho_buffer_length = 0;

  MQCMHO *cmho;
  MQHMSG msg_handle = MQHM_UNUSABLE_HMSG;
  MQLONG comp_code = MQCC_UNKNOWN, comp_reason = MQRC_NONE;
  long pyOptions = 0;
  PyObject *rv;

#ifdef no23
#if PY_MAJOR_VERSION==2
  if (!PyArg_ParseTuple(args, "ls#", &conn_handle, &cmho_buffer, &cmho_buffer_length)) {
#else
  if (!PyArg_ParseTuple(args, "ly#", &conn_handle, &cmho_buffer, &cmho_buffer_length)) {
#endif
#endif
  if (!PyArg_ParseTuple(args, "l"PSY"#|l", &conn_handle, &cmho_buffer, &cmho_buffer_length,&pyOptions)) {
                                                                                           // pyOptions is optional
    return NULL;
  }

  if (checkArgSize(cmho_buffer_length, PYMQI_MQCMHO_SIZEOF, "MQCMHO")) {
    return NULL;
  }

  cmho = (MQCMHO *)cmho_buffer;

  hConn = (MQHCONN) conn_handle; // covert from long to int ( Possible different lengths)
  Py_BEGIN_ALLOW_THREADS
  MQCRTMH(hConn                , cmho, &msg_handle, &comp_code, &comp_reason);
  Py_END_ALLOW_THREADS
  DEBUGIF(xCC   ,comp_code)
    {
      printf("c:MQCRTMH rc %d rs %d\n",comp_code,  comp_reason);
    }

  rv = Py_BuildValue("(Lll)", msg_handle, (long)comp_code, (long)comp_reason);

  return rv;
}

static char pymqe_MQSETMP__doc__[] =
"MQSETMP(conn_handle, msg_handle, smpo, name, pd, type, value, value_length) \
 \
Calls the MQI's MQSETMP function \
";

static PyObject* pymqe_MQSETMP(PyObject *self, PyObject *args) {

  long   conn_handle = MQHC_UNUSABLE_HCONN;
  long   msg_handle = MQHM_UNUSABLE_HMSG;
//MQHMSG msg_handle = MQHM_UNUSABLE_HMSG;
  MQSMPO *smpo;
  char *smpo_buffer;
  Py_ssize_t smpo_buffer_length = 0;

  MQPD *pd;
  char *pd_buffer;
  Py_ssize_t pd_buffer_length = 0;

  MQCHARV name = {MQCHARV_DEFAULT};
  char *property_name;
  Py_ssize_t property_name_length = 0;

//MQLONG property_type;
  long   property_type;
//long long property_type;

  MQLONG comp_code = MQCC_UNKNOWN;
  MQLONG comp_reason = MQRC_NONE;
  long pyOptions = 0;

  void *value = NULL;
  Py_ssize_t value_length = 0;

  PyObject *rv;
  PyObject *property_value_object;
#ifdef no23
#if PY_MAJOR_VERSION==2
  if (!PyArg_ParseTuple(args, "lls#s#s#lOl",
#else
  if (!PyArg_ParseTuple(args, "lLy#y#y#lOl",
#endif
#endif
  if (!PyArg_ParseTuple(args, "lL"PSY"#"PSY"#"PSY"#lOl|l", // pyOptions is optional
  &conn_handle, &msg_handle,
  &smpo_buffer, &smpo_buffer_length, // #
  &property_name, &property_name_length, // #
  &pd_buffer, &pd_buffer_length, // #
  &property_type, &property_value_object, &value_length,&pyOptions)) // l o l  |(optional) l
  {
    return NULL;
  }
  Py_ssize_t property_value_free = 0;
  long lValue = value_length;
  switch(property_type){
    /* Boolean value */
    case 0:
      printf("c:=== invalid property type: 0\n");
      break;

    case MQTYPE_BOOLEAN:
      lValue = sizeof(MQBOOL);
      value = malloc(lValue)        ;
      property_value_free = 1;
      *(PMQBOOL)value = (MQBOOL)PyFloat_AsDouble(property_value_object);
      break;

    /* Byte-string value */
    case MQTYPE_BYTE_STRING:
//    value = PyBytes_AsString(property_value_object);
      // extract the value and length using python calls.
      PyBytes_AsStringAndSize(property_value_object,(char **) &value,&lValue);

      break;

    /* 8-bit integer value */
    case MQTYPE_INT8:
      lValue = sizeof(MQINT8) ;
      value = malloc(lValue)        ;
      property_value_free = 1;
      *(PMQINT8)value = (MQINT8)PyLong_AsLong(property_value_object);
      break;

    /* 16-bit integer value */
    case MQTYPE_INT16:
      lValue = sizeof(MQINT16) ;
      value = malloc(lValue)        ;
      property_value_free = 1;
      *(PMQINT16)value = PyLong_AsLong(property_value_object);
      break;

    /* 32-bit integer value */
    case MQTYPE_INT32:{
      lValue = sizeof(MQINT32) ;
      value = malloc(lValue)        ;
      property_value_free = 1;
      *(PMQINT32)value = (MQINT32)PyLong_AsLongLong(property_value_object);
      break;
    }

    /* 64-bit integer value */
    case MQTYPE_INT64:
      lValue = sizeof(MQINT64) ;
      value = malloc(lValue)        ;
      property_value_free = 1;
      *(PMQINT64)value = (MQINT64)PyLong_AsLongLong(property_value_object);
      break;


    /* String value */
    case MQTYPE_STRING:
    #if PY_MAJOR_VERSION >= 3
      value = (PMQBYTE)PyUnicode_AsUTF8(property_value_object);
    #else
      value = (PMQBYTE)PyString_AsString(property_value_object);
    #endif
      break;

    /* 32-bit floating-point number value */
    case MQTYPE_FLOAT32:
      lValue = sizeof(MQFLOAT32) ;
      value = malloc(lValue)        ;
      property_value_free = 1;
      *(PMQFLOAT32)value = (MQFLOAT32)PyFloat_AsDouble(property_value_object);
      break;

    /* 64-bit floating-point number value */
    case MQTYPE_FLOAT64:
      lValue = sizeof(MQFLOAT64) ;
      value = malloc(lValue)        ;
      property_value_free = 1;
      *(PMQFLOAT64)value = (MQFLOAT64)PyFloat_AsDouble(property_value_object);
      break;

    /* without value */
    case MQTYPE_NULL:
      //*value = NULL;
      break;

  }

  if (checkArgSize(smpo_buffer_length, PYMQI_MQSMPO_SIZEOF, "MQSMPO")) {
    return NULL;
  }
  smpo = (MQSMPO *)smpo_buffer;

  if (checkArgSize(pd_buffer_length, PYMQI_MQPD_SIZEOF, "MQPD")) {
    return NULL;
  }
  pd = (MQPD *)pd_buffer;

  name.VSPtr = property_name;
  name.VSLength = (MQLONG) property_name_length;

  MQHCONN  hConn;      // an int
  hConn = (MQHCONN) conn_handle; // covert from long to int ( Possible different lengths)
  Py_BEGIN_ALLOW_THREADS
  DEBUGIF(xSETMP,comp_code)
    {
      printf("c:MQSETMP0 MPO.Options %8.8x CCSID %.1d Encoding %d\n",smpo-> Options,smpo->ValueCCSID,smpo->ValueEncoding);
      printf("c:MQSETMP1 type %s vl %d\n", MQTYPE_STR((MQLONG)property_type),value_length);
      printf("c:MQSETMP2 Name\n");
      printHex(stdout,property_name,name.VSLength);
      printf("c:MQSETMP3 Value,  length(%d)\n",value_length);
      if ( value_length >= 0)
           printHex(stdout,value,value_length);

      fflush(stdout);
    }
  MQSETMP((MQHCONN) hConn, (MQHMSG) msg_handle, smpo, &name, pd, (MQLONG) property_type, (MQLONG) lValue,
            value, &comp_code, &comp_reason);
  Py_END_ALLOW_THREADS

  DEBUGIF(xCC,comp_code)
    {
      printf("c:MQSETMP4 rc %d rs %d\n", comp_code,  comp_reason);
    }
  if (property_value_free){
    free(value);
  }


  rv = Py_BuildValue("(ll)", (long)comp_code, (long)comp_reason);
  return rv;

}
#include "inqmp.h"
#endif /* MQCMDL_LEVEL_700 */

/* Message properties and selectors - end */

#ifndef __MVS__
#include "bag2.h"
#endif /*mvs */
#endif

#ifdef __MVS__
#include <doPCFValues.h>
static char pymqe_PCFValue__doc__[] =
"PCFValue(PCFID,value) \
 \
Returns the string maching the value, for example  \
PCFValue(MQIA_Q_TYPE_,1) returns 'MQQT_LOCAL' and QT_Local' \
 \
 \
";

static PyObject *pymqe_PCFValue(PyObject *self, PyObject *args) {
  long which;
  long value;
  PyObject *rv;
  if (!PyArg_ParseTuple(args, "ll", &which,&value)) {
    return NULL;
     }
//rv = Py_BuildValue("("PSY")", getPCFValue(which,value));
  char * pWhat;
  char * pValue;
  char * pPValue; /* pretty value */
  char *  prc = getPCFValue((int)which,(int)value,&pWhat,&pValue,&pPValue);
//rv = Py_BuildValue("s", getPCFValue(which,value,);
  rv = Py_BuildValue("sss", pWhat,pValue,pPValue );
// pWHat and pValue are the read only strings from the MQ code
// the pPValue is the modified value to make it pretty, so we must free it
//free(pWhat);
//free(pValue);
  free(pPValue);
  return rv;
}

#endif

/* List of methods defined in the module */

static struct PyMethodDef pymqe_methods[] = {
  {"MQCONN", (PyCFunction)pymqe_MQCONN,    METH_VARARGS, pymqe_MQCONN__doc__},
  {"MQCONNX", (PyCFunction)pymqe_MQCONNX, METH_VARARGS, pymqe_MQCONNX__doc__},
  {"MQDISC", (PyCFunction)pymqe_MQDISC,    METH_VARARGS, pymqe_MQDISC__doc__},
  {"MQOPEN", (PyCFunction)pymqe_MQOPEN,    METH_VARARGS, pymqe_MQOPEN__doc__},
  {"MQCLOSE", (PyCFunction)pymqe_MQCLOSE, METH_VARARGS, pymqe_MQCLOSE__doc__},
  {"MQPUT", (PyCFunction)pymqe_MQPUT, METH_VARARGS, pymqe_MQPUT__doc__},
  {"MQPUT1", (PyCFunction)pymqe_MQPUT1, METH_VARARGS, pymqe_MQPUT1__doc__},
  {"MQGET", (PyCFunction)pymqe_MQGET, METH_VARARGS, pymqe_MQGET__doc__},
#ifndef __MVS__
  {"MQBEGIN", (PyCFunction)pymqe_MQBEGIN, METH_VARARGS, pymqe_MQBEGIN__doc__},
#endif
  {"MQCMIT", (PyCFunction)pymqe_MQCMIT, METH_VARARGS, pymqe_MQCMIT__doc__},
  {"MQBACK", (PyCFunction)pymqe_MQBACK, METH_VARARGS, pymqe_MQBACK__doc__},
  {"MQINQ", (PyCFunction)pymqe_MQINQ, METH_VARARGS, pymqe_MQINQ__doc__},
  {"MQSET", (PyCFunction)pymqe_MQSET, METH_VARARGS, pymqe_MQSET__doc__},
#ifdef  PYMQI_FEATURE_MQAI
  {"mqaiExecute", (PyCFunction)pymqe_mqaiExecute, METH_VARARGS, pymqe_mqaiExecute__doc__},
#endif
#ifdef MQCMDL_LEVEL_700
  {"MQSUB", (PyCFunction)pymqe_MQSUB, METH_VARARGS, pymqe_MQSUB__doc__},
  {"MQCRTMH", (PyCFunction)pymqe_MQCRTMH, METH_VARARGS, pymqe_MQCRTMH__doc__},
  {"MQSETMP", (PyCFunction)pymqe_MQSETMP, METH_VARARGS, pymqe_MQSETMP__doc__},
  {"MQINQMP", (PyCFunction)pymqe_MQINQMP, METH_VARARGS, pymqe_MQINQMP__doc__},
#endif
#ifdef __MVS__
  {"PCFValue", (PyCFunction)pymqe_PCFValue, METH_VARARGS, pymqe_PCFValue__doc__},
#endif
  {NULL, (PyCFunction)NULL, 0, NULL}        /* sentinel */
};


/* Initialization function for the module (*must* be called initpymqe) */

static char pymqe_module_documentation[] =
""
;

#ifdef WIN32
__declspec(dllexport)
#endif
#if PY_MAJOR_VERSION==2
     void initpymqe(void) {
  PyObject *m, *d;

  /* Create the module and add the functions */
  m = Py_InitModule4("pymqe", pymqe_methods,
             pymqe_module_documentation,
             (PyObject*)NULL,PYTHON_API_VERSION);
#else
static struct PyModuleDef pymqe_module = {
    PyModuleDef_HEAD_INIT,
    "pymqe",
    pymqe_module_documentation,
    -1,
    pymqe_methods
};

PyMODINIT_FUNC PyInit_zpymqe(void) {
  PyObject *m, *d;

  /* Create the module and add the functions */
  m = PyModule_Create(&pymqe_module);
#endif


  /* Add some symbolic constants to the module */
  d = PyModule_GetDict(m);
  ErrorObj = PyErr_NewException("pymqe.error", NULL, NULL);
  PyDict_SetItemString(d, "pymqe.error", ErrorObj);

  PyDict_SetItemString(d, "__doc__", Py23Text_FromString(pymqe_doc));
  PyDict_SetItemString(d,"__version__", Py23Text_FromString(__version__));

  /*
   * Build the tuple of supported command levels, but only for versions
   *  5.* onwards
   */
  {
    PyObject *versions = PyList_New(0);
#ifdef MQCMDL_LEVEL_500
      PyList_Append(versions, Py23Text_FromString("5.0"));
#endif
#ifdef MQCMDL_LEVEL_510
      PyList_Append(versions, Py23Text_FromString("5.1"));
#endif
#ifdef MQCMDL_LEVEL_520
      PyList_Append(versions, Py23Text_FromString("5.2"));
#endif
#ifdef MQCMDL_LEVEL_530
      PyList_Append(versions, Py23Text_FromString("5.3"));
#endif
#ifdef MQCMDL_LEVEL_600
      PyList_Append(versions, Py23Text_FromString("6.0"));
#endif
#ifdef MQCMDL_LEVEL_700
      PyList_Append(versions, Py23Text_FromString("7.0"));
#endif
#ifdef MQCMDL_LEVEL_710
      PyList_Append(versions, Py23Text_FromString("7.1"));
#endif
#ifdef MQCMDL_LEVEL_750
      PyList_Append(versions, Py23Text_FromString("7.5"));
#endif
#ifdef MQCMDL_LEVEL_800
      PyList_Append(versions, Py23Text_FromString("8.0.0"));
#endif
#ifdef MQCMDL_LEVEL_801
      PyList_Append(versions, Py23Text_FromString("8.0.1"));
#endif
#ifdef MQCMDL_LEVEL_802
      PyList_Append(versions, Py23Text_FromString("8.0.2"));
#endif
#ifdef MQCMDL_LEVEL_900
      PyList_Append(versions, Py23Text_FromString("9.0.0"));
#endif
#ifdef MQCMDL_LEVEL_901
      PyList_Append(versions, Py23Text_FromString("9.0.1"));
#endif
#ifdef MQCMDL_LEVEL_902
      PyList_Append(versions, Py23Text_FromString("9.0.2"));
#endif
#ifdef MQCMDL_LEVEL_903
      PyList_Append(versions, Py23Text_FromString("9.0.3"));
#endif
#ifdef MQCMDL_LEVEL_904
      PyList_Append(versions, Py23Text_FromString("9.0.4"));
#endif
#ifdef MQCMDL_LEVEL_905
      PyList_Append(versions, Py23Text_FromString("9.0.5"));
#endif
#ifdef MQCMDL_LEVEL_910
      PyList_Append(versions, Py23Text_FromString("9.1.0"));
#endif
#ifdef MQCMDL_LEVEL_911
      PyList_Append(versions, Py23Text_FromString("9.1.1"));
#endif
#ifdef MQCMDL_LEVEL_912
      PyList_Append(versions, Py23Text_FromString("9.1.2"));
#endif
#ifdef MQCMDL_LEVEL_913
      PyList_Append(versions, Py23Text_FromString("9.1.3"));
#endif
#ifdef MQCMDL_LEVEL_914
      PyList_Append(versions, Py23Text_FromString("9.1.4"));
#endif
#ifdef MQCMDL_LEVEL_915
      PyList_Append(versions, Py23Text_FromString("9.1.5"));
#endif
#ifdef MQCMDL_LEVEL_920
      PyList_Append(versions, Py23Text_FromString("9.2.0"));
#endif
#ifdef MQCMDL_LEVEL_921
      PyList_Append(versions, Py23Text_FromString("9.2.1"));
#endif
#ifdef MQCMDL_LEVEL_922
      PyList_Append(versions, Py23Text_FromString("9.2.2"));
#endif
#ifdef MQCMDL_LEVEL_923
      PyList_Append(versions, Py23Text_FromString("9.2.3"));
#endif
#ifdef MQCMDL_LEVEL_924
      PyList_Append(versions, Py23Text_FromString("9.2.4"));
#endif
      PyDict_SetItemString(d,"__mqlevels__", PyList_AsTuple(versions));
      Py_XDECREF(versions);
  }

  /*
   * Set the client/server build flag
   */
#if PYQMI_BINDINGS_MODE_BUILD == 1
  PyDict_SetItemString(d,"__mqbuild__", Py23Text_FromString("bindings"));
#else
  PyDict_SetItemString(d,"__mqbuild__", Py23Text_FromString("client"));
#endif

  /* Check for errors */
  if (PyErr_Occurred())
    Py_FatalError("can't initialize module pymqe");

#if PY_MAJOR_VERSION==3
  return m;
#endif
}
