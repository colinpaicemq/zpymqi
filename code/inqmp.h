static char pymqe_MQINQMP__doc__[] =
"MQINQMP(conn_handle, msg_handle, smpo, name, pd, type, value_length) \
 \
Calls the MQI's MQINQMP function \
";

static PyObject* pymqe_MQINQMP(PyObject *self, PyObject *args) {

  MQLONG comp_code = MQCC_UNKNOWN, comp_reason = MQRC_NONE;

  long    conn_handle = MQHC_UNUSABLE_HCONN;
  MQHMSG msg_handle = MQHM_UNUSABLE_HMSG;
//printf("1669: size of handle %d\n",sizeof(msg_handle ));
  MQCHARV name = {MQCHARV_DEFAULT};
  char *property_name;
  Py_ssize_t property_name_length = 0;
  long long ll;
  long impo_optionsl;
  int  impo_options ;
  MQLONG data_length ;
  MQLONG property_type;
  MQLONG value_length;

  int  pd_options;
  PyObject *rv;
  int  pyOptions = 0;

#ifdef no23
#if PY_MAJOR_VERSION==2
  if (!PyArg_ParseTuple(args, "llls#lll",
#else
  if (!PyArg_ParseTuple(args, "lLly#lll",
#endif
#endif
  if (!PyArg_ParseTuple(args, "lLi"PSY"#iiii|i",  // pyOptions is optional
                        &conn_handle, &msg_handle, // lL
                        &impo_options , // i
                        &property_name, &property_name_length, // PSY #
                        &pd_options, // i
                        &property_type,&value_length,&pyOptions)) {  // ii  |i
    return NULL;
  }
  MQIMPO impo = {MQIMPO_DEFAULT};
  // if we are in ascii mode we need to convert eye catcher from ascii to EBCDIC
  __a2e_l(impo.StrucId,sizeof(impo.StrucId));
  data_length = value_length;  // preset this
  impo.RequestedCCSID = 437; // please return in ASCII
  MQPD pd = {MQPD_DEFAULT};
  __a2e_l(pd.StrucId,sizeof(pd.StrucId));

  char impo_name[1024];
  impo.ReturnedName.VSPtr = impo_name;
  impo.ReturnedName.VSBufSize = sizeof(impo_name);

  char * EName;
  EName = (char *) malloc(property_name_length);
  memcpy(EName,property_name,property_name_length);
  __a2e_l(EName,property_name_length);

  impo.Options = impo_options |  MQIMPO_CONVERT_VALUE                 ;
  pd.Options   = pd_options;

  name.VSPtr = EName        ;  // need the EBCDIC version of it, not ASCII
  name.VSLength = property_name_length;

  void *value = NULL;
  MQHCONN  hConn;      // an int
  hConn = (MQHCONN) conn_handle; // covert from long to int ( Possible different lengths)
  value = (PMQBYTE)malloc(value_length);

  int iLoop;
  // loop if the buffer we got was too small.
  // this gets round the problem that data_length is not returned if length of value > buffer
  // we just loop gtting a bigger buffer
  for ( iLoop = 0; iLoop < 10;iLoop++)
  {
    MQINQMP(hConn, msg_handle, &impo, &name, &pd, &property_type, value_length,
      value, &data_length, &comp_code, &comp_reason);
    DEBUGIF(xCC,comp_code)
       {
         printf("c:MQINQMP0 rc %d rs %d %s dl %d\n",comp_code,  comp_reason,MQRC_STR(comp_reason), data_length);
       }
    DEBUGIF(xINQMP,comp_code)
       {
         if ( comp_code == 0) {
           printf("c:MQINQMP1 property name (length %d)\n",impo.ReturnedName.VSLength);
           printHex(stdout,impo.ReturnedName.VSPtr,impo.ReturnedName.VSLength );
           printf("c:MQINQMP2 type %s valuelen %d datalen %d\n",MQTYPE_STR(property_type),
                       value_length,data_length);
           // printf("Compiled %s %s.\n", __DATE__, __TIME__);
           fflush(stdout);
           }
       }
    // only loop if the buffer was too small
    if ( comp_reason != MQRC_PROPERTY_VALUE_TOO_BIG) break;  // 2469

    // we need to make the buffer bigger, and get the same propery again.
    if (value_length < 1024)
        value_length = 32767;
    else
        value_length = value_length * 2;
    if (value_length > 10 * 1024 * 1024)  // 10 MB
        break; // return saying too big
    value       = (PMQBYTE) realloc(value, value_length);

    impo.Options =  MQIMPO_INQ_PROP_UNDER_CURSOR |  MQIMPO_CONVERT_VALUE     ;

    DEBUGIF(xINQMP,comp_code)
       {
         printf("c:MQINQMP3 Reallocate buffer. Size now %d\n",value_length);
         fflush(stdout);
       }
  }
  DEBUGIF(xINQMP,comp_code)
  {
   if (comp_code == 0)
   {
     printf("c:MQINQMP4 property name (length %d)\n",impo.ReturnedName.VSLength);
     printHex(stdout,impo.ReturnedName.VSPtr,impo.ReturnedName.VSLength );
     if (data_length == 0)
        printf("c:MQINQMPData length is zero\n");
     else
     {
       printf("c:MQINQMP5 property %s value.length(%d)\n", MQTYPE_STR(property_type), data_length);
       printHex(stdout,value,data_length );
     }
   }
   fflush(stdout);
  }


  MQLONG return_length;
  if (value_length > data_length)
    return_length = data_length;
  else
    return_length = value_length;

  free(EName);
  /*
  * These all use "s" rather than "y" to return a character string
  */
  switch(property_type){
    /* Boolean value */
    case MQTYPE_BOOLEAN:
      rv = Py_BuildValue(
#ifdef no23
    #if PY_MAJOR_VERSION==2
           "(is#lll)",
    #else
           "(iy#lll)",
    #endif
#endif
           "(ils#lll)",
            *(MQBOOL*)value,(long)data_length,   // il
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,  // PSY#
            (long)comp_code, (long)comp_reason);  // ll
      break;

    /* Byte-string value */
    case MQTYPE_BYTE_STRING:
#ifdef no23
    #if PY_MAJOR_VERSION==2
      rv = Py_BuildValue("(s#lll)",
    #else
      rv = Py_BuildValue("(y#lll)",
    #endif
#endif
      rv = Py_BuildValue("("PSY"#ls#ll)",
            (PMQBYTE)value, (Py_ssize_t)return_length, // PSY#
            (long)data_length, // l
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength, // PSY#
            (long)comp_code, (long)comp_reason); // ll
       break;

    /* 8-bit integer value */
    case MQTYPE_INT8:
      rv = Py_BuildValue("(bls#ll)",
            *(PMQINT8)value,   // b
            (long)data_length, //l
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,  // PSY#
            (long)comp_code, (long)comp_reason //ll
          );
      break;

    /* 16-bit integer value */
    case MQTYPE_INT16:
      rv = Py_BuildValue("(hls#ll)",
            *(PMQINT16)value,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason
          );
      break;

    /* 32-bit integer value */
    case MQTYPE_INT32:
      rv = Py_BuildValue("(ils#ll)",
            *(PMQINT32)value,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason
          );
      break;

    /* 64-bit integer value */
    case MQTYPE_INT64:
      rv = Py_BuildValue("(Lls#ll)",
            *(PMQINT64)value,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason
          );
      break;

    /* 32-bit floating-point number value */
    case MQTYPE_FLOAT32:
      rv = Py_BuildValue("(fls#ll)",
            *(PMQFLOAT32)value,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason);
      break;

    /* 64-bit floating-point number value */
    case MQTYPE_FLOAT64:
      rv = Py_BuildValue("(dls#ll)",
            *(PMQFLOAT64)value,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason);
      break;

    /* String value */
    case MQTYPE_STRING:
//      rv = Py_BuildValue(
//    "(s#ls#ll)",
//  #if PY_MAJOR_VERSION==2
//         "(s#ls#ll)",
//  #else
//         "(y#ly#lll)",
//  #endif
      rv = Py_BuildValue("("PSY"#ls#ll)",
            (PMQCHAR)value, (Py_ssize_t)return_length,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason);
      break;

    /* NULL value */
    case MQTYPE_NULL:
      rv = Py_BuildValue("(sls#ll)",
            NULL,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason);
      break;

    default:
      rv = Py_BuildValue("(sls#ll)",
            NULL,
            (long)data_length,
            (PMQBYTE)impo.ReturnedName.VSPtr,(Py_ssize_t) impo.ReturnedName.VSLength,
            (long)comp_code, (long)comp_reason);
      break;
  }

  free(value);

  return rv;
}

