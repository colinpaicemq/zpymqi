/*************************************************************************/
/*   <copyright                                                          */
/*   notice="lm-source-program"                                          */
/*   pids="5724-H72"                                                     */
/*   years="2007,2020"                                                   */
/*   crc="3294411364" >                                                  */
/*   Licensed Materials - Property of IBM                                */
/*                                                                       */
/*   5724-H72                                                            */
/*                                                                       */
/*   (C) Copyright IBM Corp. 2007, 2020 All Rights Reserved.             */
/*                                                                       */
/*   US Government Users Restricted Rights - Use, duplication or         */
/*   disclosure restricted by GSA ADP Schedule Contract with             */
/*   IBM Corp.                                                           */
/*   </copyright>                                                        */
/*************************************************************************/
/* FUNCTION: formatConstant                                              */
/*                                                                       */
/* Make an MQI constant more readable.                                   */
/*                                                                       */
/* Given a string like MQRC_UNEXPECTED_ERROR, remove the '_' characters, */
/* convert the upper case into mixed case for each word, and return a    */
/* pointer to the second word. So returned string is "Unexpected Error". */
/*                                                                       */
/* The input string is modified by this process, so must not be in a     */
/* readonly area.                                                        */
/*                                                                       */
/* Some strings still look better in all-upper-case. So we look for the  */
/* converted mixed-case version and force them back into upper. We do not*/
/* extend the length of the buffer.                                      */
/*************************************************************************/

#if !defined(FALSE)
#define FALSE (0)
#endif
#if !defined(TRUE)
#define TRUE (1)
#endif

MQBOOL Unformatted = FALSE;

static char *formatConstant(char *);
static char *formatConstantBase(char *,MQBOOL);
const char *forceUpper[] = {
  "Amqp",
  "Cf",
  "Clwl",
  "Cpi",
  "Crl",
  "Csp",
  "Dns", /* Do this before "Dn" */
  "Dn",
  "Ds",  /* z/OS */
  "Idpw",
  "Igq",
  "Ip ",  /* note trailing space */
  "Ipv",
  "Ldap",
  "Lu62",
  "Lu ",  /* z/OS and trailingspace */
  "Mca ", /* note trailing space */
  "Mqi",
  "Mqsc",
  "Mr ",
  "Mru",
  "Pcf",
  "PM ", /* z/OS and trailing space Peristent Message */
  "Ps", /* z/OS */
  "Rba", /* z/OS */
  "Sctq",
  "Smds",
  "Ssl",
  "Tcp",
  "4kb",
};


static char* formatConstant(char *s)
{
  return formatConstantBase(s,TRUE);
}

static char* formatConstantBase(char *s,MQBOOL mixedCase )
{
  char *c;
  unsigned int i,j;
  MQBOOL swapNext = FALSE;
  MQBOOL firstUnderscore = TRUE;

  if (!s || Unformatted)
    return s;

  /* If no '_' characters, make no modifications */
  if (!strchr(s,'_'))
    return s;

  /* One special case for reformatting */
  if (!strcmp(s,"MQOT_Q"))
    return "Queue";


  /* And now work on the strings to make them mixed case */
  for (c=s;*c;c++)
  {
    if (!isspace((unsigned char)*c))
    {
      if (isupper((unsigned char)*c) && swapNext && mixedCase)
        *c = tolower(*c);
      if (*c == '_')
      {
        swapNext = FALSE;
        if (firstUnderscore)
          firstUnderscore = FALSE;
        else
        {
          *c = ' ';
        }
      }
      else
      {
        swapNext = TRUE;
      }
    }
  }

  /***********************************************************************/
  /* Patch up a few items that look better without mixed case            */
  /* An item may appear more than once in the string so loop until all   */
  /* have been converted.                                                */
  /***********************************************************************/
  for (i=0;i<(sizeof(forceUpper)/sizeof(forceUpper[0]));i++)
  {
    const char *m = forceUpper[i];
    MQBOOL done = FALSE;
    while (!done)
    {
      c = strstr(s,m);
      if (c)
      {
        for (j=0;j<strlen(m);j++)
           c[j] = toupper(c[j]);
      }
      else
        done = TRUE;
    }
  }

  /***********************************************************************/
  /* After converting to mixed-case and resetting a few strings, there   */
  /* are still a small number of cases that look better with special     */
  /* handling.                                                           */
  /***********************************************************************/
  c = strstr(s,"Zos");
  if (c)
   memcpy(c,"zOS",3);    /* Need to keep same length so can't say "z/OS" */

  c = strstr(s," Os");   /* "Operating system", also OS2 and OS400 */
  if (c)
    memcpy(c," OS",3);
  c = strstr(s,"_Os");   /* May be first token after an underscore */
  if (c)
    memcpy(c,"_OS",3);

  /***********************************************************************/
  /* And finally return a pointer to the second word (remove the prefix) */
  /* provided we can.                                                    */
  /***********************************************************************/
  c = strchr(s,'_');
  if (!c || c == &s[strlen(s)-1])
    c= s;
  else
    c++;

  return c;
}

char * formatValue( MQLONG v ) {
  char * string  ;
  char * stringIA;
  char * stringCA;
  string   = MQIA_STR(v);  // try to see if it is a Integer   version
  if (strlen(string  ) == 0)
  {
     string   = MQCA_STR(v);  // try a string version
     if (strlen(string) == 0) // it was neither
       string = "Unknown";
  }
  else
  {
    // check for both
    char * stringCA = MQCA_STR(v);
    if (strlen(stringCA) != 0)
    {
      printf("*** Error value %d has %s and %s\n", v, stringCA, string  );
    }
  }
  char * string2   = strdup(string);  // because it is modified in the following
  char * retString = strdup(formatConstant(string2));
  free(string2);
  char * c1;
  char * c2;
  c1 = retString;
  c2 = retString;
  int i;
  for(i=0;i <50;i++)
  {
//  printf("c1=%c",*c1);
    if (*c1 ==  ' ') c1++;
    *c2 = *c1;
//  printf("c2=%c",*c2);
    if (*c1 == 0   ) break;
    c1++;
    c2++;
  }
    printf("\n");
  if ( i > 30) printf(">>>>>>>>> format problem line 204\n");
  return retString;  // need to remember to free it afterwards
}
