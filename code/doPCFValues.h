/*
*
* Copyright (C) Copyright IBM Corp. 1977, 2014
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* * Contributors:
*   Colin Paice
*
*/
/* Include standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

/* Include mqseries headers */
#include <cmqc.h>
#include <cmqcfc.h>
#include <cmqxc.h>
#include <cmqstrc.h>


typedef char * (*pProcess)(MQLONG value);
pProcess whichFormat[] = { MQIA_STR,  MQIACF_STR,  MQIACH_STR,
 MQBACF_STR,  MQCA_STR,  MQCACF_STR,  MQCACH_STR,  MQIAMO_STR,  MQIAMO64_STR};
struct tagDoIt
{
  MQLONG   which;
  pProcess routine;
};
typedef struct tagDoIt doIt;


int binarySearch(doIt arr[], long l, long r, long x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        // itself
        if (arr[mid].which == x)
            return mid;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (arr[mid].which > x)
            return binarySearch(arr, l, mid - 1, x);

        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid + 1, r, x);
    }

    // We reach here when element is not
    // present in array
    return -1;
}

#ifdef no

char *MQQSUMS_STR (MQLONG v)  // MQIACF_SYSP_OFFLOAD_STATUS

{
  char *c;
  c = "Not yet supported";
  return c;
}



char *MQSYSP_STATUS_STR (MQLONG v)  // MQIACF_SYSP_OFFLOAD_STATUS

{
  char *c;
  c = "Not yet supported";
  return c;
}

 (MQLONG v)
{
  char *c;
  c = "Not yet supported";
  return c;
}

char *MQIACF_CF_STATUS_STR (MQLONG v)
{
  char *c;
  c = "Not yet supported";
  return c;
}


char *MQSQQN_STR (MQLONG v)
{
  char *c;
  c = "Not yet supported";
  return c;
}

#endif
//   {    1, (pProcess)  MQAT_STR }, // MQIA_APPL_TYPE
#define MQMAP(a,b) {a,(pProcess) b }
doIt DOIT[] =
{
//nclude <PCFMapping.h>
#include <MQMAP.h>
{99999999, MQTT_STR} // the last one needed for C to compile
};


char * getPCFValue(MQLONG what, MQLONG value, char **pWhat, char **pValue, char **pPValue)
{
  int i;
  char * p;
  /* convert the what type value into a string */
  for (i = 0;i < sizeof(whichFormat)/sizeof(whichFormat[0]);i ++)
  {
     p =  whichFormat[i](what);
     if (p != "") break;
  }
  *pWhat = "";
  if (p != "")
  {
    //  formatConstant modifies the string, so we need our own r/w copy
    char * p2 = strdup(p);
    /* formatConstant makes it pretty and removes the xxx_ from the front */
    *pWhat =        formatConstant(p2) ; /* return the pretty version     */
  }
  *pPValue = 0; /* preset this */
  *pValue  = 0; // preset this
  long lDoIt = sizeof(DOIT)/sizeof(doIt);
  long rc =  binarySearch(DOIT, 0, lDoIt, what);

   if (rc < lDoIt && rc > 0)
   {
     *pValue   =        (DOIT[rc].routine)(value) ;
     //  formatConstant modifies the string, so we need our own r/w copy
     char * p2 = strdup(*pValue);
     /* formatConstant makes it pretty and removes the xxx_ from the front */
     *pPValue =        formatConstant(p2) ; /* return the pretty version     */
     return(NULL  );
   }
  return(NULL  ); // element not in array

}
