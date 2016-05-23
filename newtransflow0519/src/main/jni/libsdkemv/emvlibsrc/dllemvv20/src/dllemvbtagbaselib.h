#ifndef _DLLEMVBTAGBASELIB_H_
#define _DLLEMVBTAGBASELIB_H_


#define TAG_CardAID                               "\x4F\x00\x00\x00"
#define TAG_AppLabel                              "\x50\x00\x00\x00"
#define TAG_PAYWAVESIGNCVM                        "\x55\x00\x00\x00"
#define TAG_PayPassTrack1Data                     "\x56\x00\x00\x00"
#define TAG_Track2Equ                             "\x57\x00\x00\x00"
#define TAG_PAN                                   "\x5A\x00\x00\x00"
#define TAG_CardHoldName                          "\x5F\x20\x00\x00"
#define TAG_AppExpireDate                         "\x5F\x24\x00\x00"
#define TAG_AppEffectDate                         "\x5F\x25\x00\x00"
#define TAG_IssuCountryCode                       "\x5F\x28\x00\x00"
#define TAG_TransCurcyCode                        "\x5F\x2A\x00\x00"
#define TAG_LangPrefer                            "\x5F\x2D\x00\x00"
#define TAG_ServiceCode                           "\x5F\x30\x00\x00"
#define TAG_PANSeq                                "\x5F\x34\x00\x00"
#define TAG_TransCurcyExp                         "\x5F\x36\x00\x00"
#define TAG_IssuerURL                             "\x5F\x50\x00\x00"
#define TAG_PAYPASS_AccountType                   "\x5F\x57\x00\x00"
#define TAG_FCITemplate                           "\x6F\x00\x00\x00"
#define TAG_SCRIPT1                               "\x71\x00\x00\x00"
#define TAG_SCRIPT2                               "\x72\x00\x00\x00"
#define TAG_RMTF2                                 "\x77\x00\x00\x00"
#define TAG_RMTF1                                 "\x80\x00\x00\x00"
#define TAG_AmtAuthBin                            "\x81\x00\x00\x00"
#define TAG_AIP                                   "\x82\x00\x00\x00"
#define TAG_DFName                                "\x84\x00\x00\x00"
#define TAG_AppPriority                           "\x87\x00\x00\x00"
#define TAG_AuthorCode                            "\x89\x00\x00\x00"
#define TAG_AuthRespCode                          "\x8A\x00\x00\x00"
#define TAG_CDOL1                                 "\x8C\x00\x00\x00"
#define TAG_CDOL2                                 "\x8D\x00\x00\x00"
#define TAG_CVMList                               "\x8E\x00\x00\x00"
#define TAG_CAPKI                                 "\x8F\x00\x00\x00"
#define TAG_IPKCert                               "\x90\x00\x00\x00"
#define TAG_IssuerAuthenData                      "\x91\x00\x00\x00"
#define TAG_IPKRem                                "\x92\x00\x00\x00"
#define TAG_SignStatAppData                       "\x93\x00\x00\x00"
#define TAG_AFL                                   "\x94\x00\x00\x00"
#define TAG_TVR                                   "\x95\x00\x00\x00"
#define TAG_TDOL                                  "\x97\x00\x00\x00"
#define TAG_TCHashValue                           "\x98\x00\x00\x00"
#define TAG_PIN                                   "\x99\x00\x00\x00"
#define TAG_TransDate                             "\x9A\x00\x00\x00"
#define TAG_TSI                                   "\x9B\x00\x00\x00"
#define TAG_TransTypeValue                        "\x9C\x00\x00\x00"
#define TAG_DDFName                               "\x9D\x00\x00\x00"
#define TAG_AcquireID                             "\x9F\x01\x00\x00"
#define TAG_AmtAuthNum                            "\x9F\x02\x00\x00"
#define TAG_AmtOtherNum                           "\x9F\x03\x00\x00"
#define TAG_AmtOtherBin                           "\x9F\x04\x00\x00"
#define TAG_AppDiscretData                        "\x9F\x05\x00\x00"
#define TAG_TermAID                               "\x9F\x06\x00\x00"
#define TAG_AUC                                   "\x9F\x07\x00\x00"
#define TAG_AppVer                                "\x9F\x08\x00\x00"
#define TAG_AppVerNum                             "\x9F\x09\x00\x00"
#define TAG_CardHoldNameExt                       "\x9F\x0B\x00\x00"
#define TAG_IACDefault                            "\x9F\x0D\x00\x00"
#define TAG_IACDenial                             "\x9F\x0E\x00\x00"
#define TAG_IACOnline                             "\x9F\x0F\x00\x00"
#define TAG_IssuAppData                           "\x9F\x10\x00\x00"
#define TAG_ICTI                                  "\x9F\x11\x00\x00"
#define TAG_AppPreferName                         "\x9F\x12\x00\x00"
#define TAG_LOATC                                 "\x9F\x13\x00\x00"
#define TAG_LCOL                                  "\x9F\x14\x00\x00"
#define TAG_MerchCateCode                         "\x9F\x15\x00\x00"
#define TAG_MerchID                               "\x9F\x16\x00\x00"
#define TAG_PINTryCount                           "\x9F\x17\x00\x00"
#define TAG_CountryCode                           "\x9F\x1A\x00\x00"
#define TAG_FloorLimit                            "\x9F\x1B\x00\x00"
#define TAG_TermID                                "\x9F\x1C\x00\x00"
#define TAG_TRMData                               "\x9F\x1D\x00\x00"
#define TAG_IFD_SN                                "\x9F\x1E\x00\x00"
#define TAG_Track1Discret                         "\x9F\x1F\x00\x00"
#define TAG_Track2Discret                         "\x9F\x20\x00\x00"
#define TAG_TransTime                             "\x9F\x21\x00\x00"
#define TAG_TermCAPKI                             "\x9F\x22\x00\x00"
#define TAG_UCOL                                  "\x9F\x23\x00\x00"
#define TAG_AppCrypt                              "\x9F\x26\x00\x00"
#define TAG_CryptInfo                             "\x9F\x27\x00\x00"
#define TAG_ICCPIN_EPKCert                        "\x9F\x2D\x00\x00"
#define TAG_ICCPIN_EPKExp                         "\x9F\x2E\x00\x00"
#define TAG_ICCPIN_EPKRem                         "\x9F\x2F\x00\x00"
#define TAG_IPKExp                                "\x9F\x32\x00\x00"
#define TAG_TermCapab                             "\x9F\x33\x00\x00"
#define TAG_CVMResult                             "\x9F\x34\x00\x00"
#define TAG_TermType                              "\x9F\x35\x00\x00"
#define TAG_ATC                                   "\x9F\x36\x00\x00"
#define TAG_UnpredictNum                          "\x9F\x37\x00\x00"
#define TAG_PDOL                                  "\x9F\x38\x00\x00"
#define TAG_POSEntryMode                          "\x9F\x39\x00\x00"
#define TAG_AmtReferCurcy                         "\x9F\x3A\x00\x00"
#define TAG_AppReferCurcy                         "\x9F\x3B\x00\x00"
#define TAG_TransReferCurcyCode                   "\x9F\x3C\x00\x00"
#define TAG_TransReferCurcyExp                    "\x9F\x3D\x00\x00"
#define TAG_TermAddCapab                          "\x9F\x40\x00\x00"
#define TAG_TransSeqCount                         "\x9F\x41\x00\x00"
#define TAG_AppCurcyCode                          "\x9F\x42\x00\x00"
#define TAG_AppReferCurcyExp                      "\x9F\x43\x00\x00"
#define TAG_AppCurcyExp                           "\x9F\x44\x00\x00"
#define TAG_DataAuthCode                          "\x9F\x45\x00\x00"
#define TAG_ICCPKCert                             "\x9F\x46\x00\x00"
#define TAG_ICCPKExp                              "\x9F\x47\x00\x00"
#define TAG_ICCPKRem                              "\x9F\x48\x00\x00"
#define TAG_DDOL                                  "\x9F\x49\x00\x00"
#define TAG_SDATagList                            "\x9F\x4A\x00\x00"
#define TAG_SignDynAppData                        "\x9F\x4B\x00\x00"
#define TAG_ICCDynNum                             "\x9F\x4C\x00\x00"
#define TAG_LogEntry                              "\x9F\x4D\x00\x00"
#define TAG_paywavemerchantname                   "\x9F\x4E\x00\x00"
#define TAG_OfflineAccumulatorBalance             "\x9F\x50\x00\x00"
#define TAG_DRDOL                                 "\x9F\x51\x00\x00"
#define TAG_MCHIPTransCateCode                    "\x9F\x53\x00\x00"
#define TAG_DSODSCard                             "\x9F\x54\x00\x00"
#define TAG_PaywaveAppProgId                      "\x9F\x5A\x00\x00"
#define TAG_DSDOL                                 "\x9F\x5B\x00\x00"
#define TAG_IssuerScriptREsult                    "\x9F\x5B\x00\x00"
#define TAG_DSOperatorID                          "\x9F\x5C\x00\x00"
#define TAG_qPBOCOfflineSpendAmount               "\x9F\x5D\x00\x00"
#define TAG_AppCapabilitiesInfor                  "\x9F\x5D\x00\x00"
#define TAG_DSID                                  "\x9F\x5E\x00\x00"
#define TAG_DSSlotAvail                           "\x9F\x5F\x00\x00"
#define TAG_PaypassCVC3Track1                     "\x9F\x60\x00\x00"
#define TAG_PaypassCVC3Track2                     "\x9F\x61\x00\x00"
#define TAG_CardHoldIdNo                          "\x9F\x61\x00\x00"
#define TAG_PaypassPCVC3Track1                    "\x9F\x62\x00\x00"
#define TAG_CardHoldIdType                        "\x9F\x62\x00\x00"
#define TAG_PaypassPunatcTrack1                   "\x9F\x63\x00\x00"
#define TAG_CardIdentifyInfo                      "\x9F\x63\x00\x00"
#define TAG_Paypassnatctrack1                     "\x9F\x64\x00\x00"
#define TAG_PaypassPcvc3Track2                    "\x9F\x65\x00\x00"
#define TAG_TermTransPredicable                   "\x9F\x66\x00\x00"
#define TAG_PaypassPunatcTrack2                   "\x9F\x66\x00\x00"
#define TAG_PaypassNatcTrack2                     "\x9F\x67\x00\x00"
#define TAG_PaywaveCardAuthData                   "\x9F\x69\x00\x00"
#define TAG_qPBOCUndfinedParam                    "\x9F\x69\x00\x00"
#define TAG_paypassun                             "\x9F\x6A\x00\x00"
#define TAG_PaypassMagTrack2                      "\x9F\x6B\x00\x00"
#define TAG_CardTransPredicable                   "\x9F\x6C\x00\x00"
#define TAG_paypassMagAppVer                      "\x9F\x6D\x00\x00"
#define TAG_ECResetThreshold                      "\x9F\x6D\x00\x00"
#define TAG_FormFactIndicator                     "\x9F\x6E\x00\x00"
#define TAG_PaypassThirdPartyData                 "\x9F\x6E\x00\x00"
#define TAG_DSSlotManage                          "\x9F\x6F\x00\x00"
#define TAG_ProtectedDataEnvelope1                "\x9F\x70\x00\x00"
#define TAG_ProtectedDataEnvelope2                "\x9F\x71\x00\x00"
#define TAG_ProtectedDataEnvelope3                "\x9F\x72\x00\x00"
#define TAG_ProtectedDataEnvelope4                "\x9F\x73\x00\x00"
#define TAG_ProtectedDataEnvelope5                "\x9F\x74\x00\x00"
#define TAG_VLPIssuAuthorCode                     "\x9F\x74\x00\x00"
#define TAG_UnprotectedDataEnvelope1              "\x9F\x75\x00\x00"
#define TAG_UnprotectedDataEnvelope2              "\x9F\x76\x00\x00"
#define TAG_UnprotectedDataEnvelope3              "\x9F\x77\x00\x00"
#define TAG_MaxECCashBalance                      "\x9F\x77\x00\x00"
#define TAG_UnprotectedDataEnvelope4              "\x9F\x78\x00\x00"
#define TAG_UnprotectedDataEnvelope5              "\x9F\x79\x00\x00"
#define TAG_VLPAvailableFund                      "\x9F\x79\x00\x00"
#define TAG_VLPIndicator                          "\x9F\x7A\x00\x00"
#define TAG_VLPTransLimit                         "\x9F\x7B\x00\x00"
#define TAG_CustomerExclusiveData                 "\x9F\x7C\x00\x00"
#define TAG_paypassMerchantCustomData             "\x9F\x7C\x00\x00"
#define TAG_DSSummary1                            "\x9F\x7D\x00\x00"
#define TAG_MobileSupportIndicator                "\x9F\x7E\x00\x00"
#define TAG_DSUnpredictableNumber                 "\x9F\x7F\x00\x00"
#define TAG_FCIProterty                           "\xA5\x00\x00\x00"
#define TAG_FCIDisData                            "\xBF\x0C\x00\x00"
#define TAG_PAYWAVEAP_RCTL                        "\xDF\x00\x00\x00"
#define TAG_PAYWAVEAP_CVMLimit                    "\xDF\x01\x00\x00"
#define TAG_PAYWAVEAP_FloorLimit                  "\xDF\x02\x00\x00"
#define TAG_PAYWAVEAP_EDDAVersion                 "\xDF\x03\x00\x00"
#define TAG_PAYWAVEAP_CVMREQ                      "\xDF\x04\x00\x00"
#define TAG_PAYWAVEAP_DispOfflineFundInd          "\xDF\x05\x00\x00"
#define TAG_PaywaveRCP                            "\xDF\x06\x00\x00"
#define TAG_POSCII                                "\xDF\x4B\x00\x00"
#define TAG_CirqueLogEntry                        "\xDF\x4D\x00\x00"
#define TAG_DSInputCARD                           "\xDF\x60\x00\x00"
#define TAG_DSDigestH                             "\xDF\x61\x00\x00"
#define TAG_DSODSInfo                             "\xDF\x62\x00\x00"
#define TAG_DSODSTerm                             "\xDF\x63\x00\x00"
#define TAG_ODAalgrthFlag                         "\xDF\x69\x00\x00"
#define TAG_DSSummary2                            "\xDF\x81\x01\x00"
#define TAG_DSSummary3                            "\xDF\x81\x02\x00"
#define TAG_BalanceBeforeGAC                      "\xDF\x81\x04\x00"
#define TAG_BalanceAfterGAC                       "\xDF\x81\x05\x00"
#define TAG_PayassDataNeeded                      "\xDF\x81\x06\x00"
#define TAG_CDOL1RelatedData                      "\xDF\x81\x07\x00"
#define TAG_DSACType                              "\xDF\x81\x08\x00"
#define TAG_DSInputTerm                           "\xDF\x81\x09\x00"
#define TAG_DSODSInfoReader                       "\xDF\x81\x0A\x00"
#define TAG_DSSummaryStatus                       "\xDF\x81\x0B\x00"
#define TAG_KernelID                              "\xDF\x81\x0C\x00"
#define TAG_DSVNTerm                              "\xDF\x81\x0D\x00"
#define TAG_POSTGACPDS                            "\xDF\x81\x0E\x00"
#define TAG_PREGACPDS                             "\xDF\x81\x0F\x00"
#define TAG_ProfirstWF                            "\xDF\x81\x10\x00"
#define TAG_PDOLData                              "\xDF\x81\x11\x00"
#define TAG_TagsToRead                            "\xDF\x81\x12\x00"
#define TAG_DRDOLRelatedData                      "\xDF\x81\x13\x00"
#define TAG_ReferenceControlParameter             "\xDF\x81\x14\x00"
#define TAG_ErrorIndication                       "\xDF\x81\x15\x00"
#define TAG_UserInterReqData                      "\xDF\x81\x16\x00"
#define TAG_CARDINPUTCAP                          "\xDF\x81\x17\x00"
#define TAG_PAYPASSCVMRequired                    "\xDF\x81\x18\x00"
#define TAG_PAYPASSNOCVMRequired                  "\xDF\x81\x19\x00"
#define TAG_TermUDOL                              "\xDF\x81\x1A\x00"
#define TAG_KernelConfiguration                   "\xDF\x81\x1B\x00"
#define TAG_MaxLifeTimeTornLog                    "\xDF\x81\x1C\x00"
#define TAG_MaxNumTornLog                         "\xDF\x81\x1D\x00"
#define TAG_magcvmrequirer                        "\xDF\x81\x1E\x00"
#define TAG_SecurityCapability                    "\xDF\x81\x1F\x00"
#define TAG_TACDefault                            "\xDF\x81\x20\x00"
#define TAG_TACDenial                             "\xDF\x81\x21\x00"
#define TAG_TACOnline                             "\xDF\x81\x22\x00"
#define TAG_clessofflinelimitamt                  "\xDF\x81\x23\x00"
#define TAG_clessofflineamt                       "\xDF\x81\x24\x00"
#define TAG_ondevRTCL                             "\xDF\x81\x25\x00"
#define TAG_termcvm_limit                         "\xDF\x81\x26\x00"
#define TAG_TimeOutValue                          "\xDF\x81\x27\x00"
#define TAG_IDSStatus                             "\xDF\x81\x28\x00"
#define TAG_OutcomeParameterSet                   "\xDF\x81\x29\x00"
#define TAG_Paypassconverttrack1                  "\xDF\x81\x2A\x00"
#define TAG_Paypassconverttrack2                  "\xDF\x81\x2B\x00"
#define TAG_magnocvmrequirer                      "\xDF\x81\x2C\x00"
#define TAG_MessageHoldTime                       "\xDF\x81\x2D\x00"
#define TAG_HoldTimeValue                         "\xDF\x81\x30\x00"
#define TAG_TornRecord                            "\xFF\x81\x01\x00"
#define TAG_WriteBeforeGAC                        "\xFF\x81\x02\x00"
#define TAG_WriteAfterGAC                         "\xFF\x81\x03\x00"
#define TAG_PaypassDataToSend                     "\xFF\x81\x04\x00"
#define TAG_PaypassDataRecord                     "\xFF\x81\x05\x00"
#define TAG_DiscretionaryData                     "\xFF\x81\x06\x00"
#define TAG_TermDDOL                              "\xFF\x81\x07\x00"
#define TAG_CheckRedundantData                    "\xFF\x81\x08\x00"
#define TAG_TermTDOL                              "\xFF\x81\x09\x00"
#define TAG_Threshold                             "\xFF\x81\x0A\x00"
#define TAG_MaxTargetPercent                      "\xFF\x81\x0B\x00"
#define TAG_TargetPercent                         "\xFF\x81\x0C\x00"
#define TAG_bShowRandNum                          "\xFF\x81\x0D\x00"
#define TAG_bForceOnline                          "\xFF\x81\x0E\x00"
#define TAG_SCRIPT71RESULT                        "\xFF\x81\x71\x00"
#define TAG_SCRIPT72RESULT                        "\xFF\x81\x72\x00"
#define TAG_CDOL2RelatedData                      "\xFF\x81\x73\x00"
#define TAG_SupportTermCheck                      "\xFF\x81\x74\x00"
#define TAG_DDOLRelatedData                       "\xFF\x81\x75\x00"
#define TAG_ICCPIN_EPKModul                       "\xFF\x81\x76\x00"
#define TAG_BLACKLISTSM3_ENABLE                   "\xFF\x81\x77\x00"







//#define TAG_Script7172Data                        "\xFF\x81\x51\x00"  //71 72�Ȳ���




#define TAGUPDATECONDITION_K         0x40
#define TAGUPDATECONDITION_ACT       0x10
#define TAGUPDATECONDITION_RA        0x20


#define TAGFORMAT_N      1
#define TAGFORMAT_B      2
#define TAGFORMAT_ANS    4
#define TAGFORMAT_CN     8
#define TAGFORMAT_AN     0x10


#define TAGTYPE_ALL          0xFFFFFFFF
#define TAGTYPE_PAYPASS      0x01
#define TAGTYPE_PAYWAVE      0x02
#define TAGTYPE_QPBOC        0x04
#define TAGTYPE_PAYPASS_DET  0x08
#define TAGTYPE_EMV          0x10
#define TAGTYPE_NULL         0x80000000


#define TAGNOTEXIST  0
#define TAGEMPTY     1
#define TAGEXIST     2


typedef struct _EMVTAG {
    unsigned char Tag[4];
} EMVTAG;

typedef struct {

    unsigned char Tag[4];
    unsigned char updatecondition;
    unsigned short minlen;
    unsigned short maxlen;
    unsigned char datafomat;
    unsigned int SupAppType;

} EMVTAGITEM;


typedef struct {

    unsigned char Tag[4];
    unsigned char updatecondition;
    unsigned short minlen;
    unsigned short maxlen;
    unsigned char datafomat;
    unsigned int SupAppType;
    unsigned short len;
    unsigned char *data;

} EMVTAGCVLITEM;



extern int EMVB_TagBaseLib_read(unsigned char *tag,unsigned char taglen,int apptype,EMVTAGCVLITEM *emvtagitem);
//extern void emvbtest_EMVB_TagBaseLib_read(void);

//extern void emvbtest_gEMVTagBaseLibdebug_enable(void);


#endif


