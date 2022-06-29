 /********************************************************************
 * printMD - Function to print the Message Descriptor.
 ********************************************************************/
 /* on z/OS we have to convert from EBCDIC back to ASCII because    */
 /* the program is compiled as ASCII and printf etc only does ASCII */
 char * E2A(char * pData,size_t lData){
   __e2a_l(pData,lData);
 return pData ;
 }
 void printMD(PMQMD2 md)
 {
   MQMD2  MD;
   /* We need local copy of MQMD as we change strings to ASCII    */
   int i;
   memcpy(&MD,md,sizeof(MQMD2));
   printf("MQMD \n");
   #ifdef delete
   char StrucId[5], Format[9], ReplyToQ[49];
   char ReplyToQMgr[49];
   char UserIdentifier[13], ApplIdentityData[33];
   char PutApplName[29];
   char PutDate[9], PutTime[9], ApplOriginData[5];
   #endif
   char printchar;

// StrucId[4] = Format[8] = ReplyToQ[48] =
//    ReplyToQMgr[48] =
//    UserIdentifier[12] = ApplIdentityData[32] =
//    PutApplName[28] =
//    PutDate[8] = PutTime[8] = ApplOriginData[4] = '\0';

   printf("StrucId         :%4.4s\t\t",E2A(MD.StrucId,4));
   printf("Version         :%d\n",MD.Version);
   printf("Report          :%8.8X\t",MD.Report);
   printf("MsgType         :%6.8X\n",MD.MsgType);
   printf("Expiry          :%8.8X\t",MD.Expiry);
   printf("Feedback        :%8.8X\n",MD.Feedback     );
   printf("Encoding        :%8.8X\t",MD.Encoding);
   printf("CodedCharSetId  :%8.8X (%d)\n",MD.CodedCharSetId,
                                          MD.CodedCharSetId);
   printf("Format          :%8.8s\n",E2A(MD.Format,8));
   printf("Priority        :%8.8X\t",MD.Priority);
   printf("Persistence     :%8.8X\n",MD.Persistence);
   printf("MsgId           :");
   /*****************************************************************/
   /* MsgId is not null terminated, so print one char at a time (if */
   /* the char is printable). Similarly for other strings below.    */
   /*****************************************************************/
   for(i=0;i<24;i++) printf("%c",(isprint(MD.MsgId[i]) \
                                  ? MD.MsgId[i] : '.') );
   printf("\n");


   printf("MsgidHex        <");
   for(i=0;i<24;i++) printf("%2.2X",(MD.MsgId[i]        ) );
     printf(">\n");
   printf("CorrelId        :");
   for(i=0;i<24;i++)
   for(i=0;i<24;i++) printf("%c",(isprint(MD.CorrelId[i]) \
                                  ? MD.CorrelId[i] : '.') );
   printf("\n");
   printf("CorrelidHex     <");
   for(i=0;i<24;i++) printf("%2.2X",(MD.CorrelId[i] ) );
     printf(">\n");
   char tTime[3];
   long secs = 0;
   /*hours*/
   memcpy(&tTime[0],&MD.PutTime[0],2);
   tTime[2] = 0;
   secs   =      3600* atol(tTime);
   /*mins */
   memcpy(&tTime[0],&MD.PutTime[2],2);
   tTime[2] = 0;
   secs  += 60 * atol(tTime);
   /*secs */
   memcpy(&tTime[0],&MD.PutTime[4],2);
   tTime[2] = 0;
   secs  +=  atol(tTime);
   printf("BackoutCount    :%8.8X\n",MD.BackoutCount);
   printf("ReplyToQ        :%48.48s\n",E2A(MD.ReplyToQ,48));
   printf("ReplyToQMgr     :%48.48s\n",E2A(MD.ReplyToQMgr,48));
   printf("UserIdentifier  :%12.12s\n",E2A(MD.UserIdentifier,12));
   printf("AccountingToken\n");
   printf("<");
   for(i=0;i<32;i++) printf("%2.2X",MD.AccountingToken[i]);
      printf(">\n");
   printf("ApplIdentityData:%32.32s\n",E2A(MD.ApplIdentityData,32));
   printf("PutApplType     :%8.8X\n",MD.PutApplType);
   printf("PutApplName     :%28.28s\n",E2A(MD. PutApplName,28));
   printf("PutDate         :%8.8s\t",E2A(MD.PutDate,8));
   printf("PutTime         :%8.8s\n",E2A(MD.PutTime,8));
   printf("ApplOriginData  :%4.4s\n",E2A(MD.ApplOriginData,4));
   /* Print version 2 fields */
     printf("GroupId\n");
     printf("<");
     for(i=0;i<24;i++) printf("%2.2X",MD.GroupId[i]);
     printf(">\n");
     printf("MsgSeqNumber    :%d\t",MD.MsgSeqNumber);
     printf("Offset          :%d\n",MD.Offset);
     printf("MsgFlags        :%2.2X\t",MD.MsgFlags);
     printf("OriginalLength  :%d\n",MD.OriginalLength);
}  /* End PrintMD() */
