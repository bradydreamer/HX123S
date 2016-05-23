#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#ifndef SDK_TEST_H
#define SDK_TEST_H


/*��������*/
#define     TEST_YES        1
#define     TEST_NO         0

#define     TEST_MATH_MODULE    TEST_YES//��ѧģ�����ʹ��

#define     TEST_KEY_MODULE    TEST_YES//����ģ�����ʹ��

#define     TEST_TOOL_MODULE    TEST_YES//����ģ�����ʹ��

#define     TEST_PRINTER_MODULE    TEST_NO//��ӡģ�����ʹ��

#define     TEST_LCD_SHOW_MODULE    TEST_YES//��ʾģ�����ʹ��

#define     TEST_IME_MODULE    TEST_YES//���뷨ģ�����ʹ��

#define     TEST_IC_MODULE    TEST_YES//IC ��ģ�����ʹ��

#define     TEST_MIFARE_MODULE    TEST_YES//Mifareģ�����ʹ��

#define     TEST_MEMCARD_MODULE    TEST_YES//MemCardģ�����ʹ��

#define     TEST_COMM_MODULE    TEST_NO//ͨ��ģ�����ʹ��

#define     TEST_SYSTEM_MODULE    TEST_YES//ϵͳģ�����ʹ��

#define     TEST_SCANNER_MODULE    TEST_NO//ɨ��ǹģ�����ʹ��

#define     TEST_8538_MODULE    TEST_NO//8583ģ�����ʹ��

#define     TEST_HISLIB_MODULE    TEST_YES//��ʷ��ģ�����ʹ��

#define     TEST_LINK_MODULE    TEST_YES//����ģ�����ʹ��

#define     TEST_QUEUE_MODULE    TEST_YES//����ģ�����ʹ��

#define     TEST_XML_MODULE    TEST_NO//XMLģ�����ʹ��

#define     TEST_DUKPT_MODULE    TEST_NO//DUKPTģ�����ʹ��

#define     TEST_MULTI_LANG_MODULE    TEST_NO//������ģ�����ʹ��

#define     TEST_FILESYS_MODULE    TEST_YES//�ļ�ϵͳģ�����ʹ��

#define     TEST_PED_MODULE    TEST_YES//�������ģ�����ʹ��

#define     TEST_AUDIO_MODULE    TEST_YES//��Ƶģ�����ʹ��


#define     CHECK_MODULE_IS_NEED_TEST(moduleID, moduleCode) do{   \
moduleCode;  \
}  \
while(moduleID)    \



#define SDK_TEST_OK         2
#define SDK_TEST_REPEAT     1
#define SDK_TEST_FALSE      0


//�������ͳ������
//��ʼ������ͳ������,�ϵ����һ��
extern void sdkTestTraceInit(void);

//  һ�㶼����һ���������̿�ʼǰ����
extern void sdkTestClrStatisticsData(void);

//��ӡ����ͳ��������
// һ���ڲ�����ɺ��ӡ���Խ��
extern int sdkTestPrintStatisticsData(void);

//����ʧ�ܴ���
// ����ڵ��Ժ����Զ�����
extern void skdTestIncFailed(const u8 *pfile, int line, const u8 *pInfo, const u8 *pstr);

//	�����ɹ�����
//        ����ڵ��Ժ����Զ�����
extern void skdTestIncOk(void);

extern void Test_sdkListAuto(void);


#if 1
#ifdef __cplusplus
#define TestLog(ptag, arg...)  SdkDebugTrace((u8 const*)ptag, (u8 const*)__FILE__, (u8 const*)__FUNCTION__, __LINE__, ##arg)
#else
#if(MACHINEMODEL == SDK_SYS_MACHINE_K501AM)
#define TestLog(ptag, ...)  SdkDebugTrace((u8 const*)ptag, (u8 const*)__FILE__, (u8 const*)"char", __LINE__, ##__VA_ARGS__)
#else
#define TestLog(ptag,  ...)  SdkDebugTrace((u8 const*)ptag, (u8 const*)__FILE__, (u8 const*)__FUNCTION__, __LINE__, __VA_ARGS__)
#endif
#endif

#if(MACHINEMODEL == SDK_SYS_MACHINE_K501AM)
#define  TestLogHex(pasTag, pasInfo, pheDate, uiLen)   sdkDebugTraceHex((u8 const*)pasTag, (u8 const*)pasInfo, pheDate, uiLen,(u8 const*)"fuction",__LINE__)
#else
#define  TestLogHex(pasTag, pasInfo, pheDate, uiLen)   sdkDebugTraceHex((u8 const*)pasTag, (u8 const*)pasInfo, pheDate, uiLen,(u8 const*)__FUNCTION__,__LINE__)
#endif
#else
#define TestLog(ptag,...)
#define  TestLogHex(pasTag, pasInfo, pheDate, uiLen) 
#endif



//������൱ ���ã������˶�Ҫ��ϸ��һ��sdkTestPrint.c ��д��demo

#define TEST_IS_TRUE_WITH_INFO(info, var) {if((var) == false){skdTestIncFailed(__FILE__, __LINE__,info,#var);TestLog("testResult","����ʧ��:%s ���ʽ:%s\r\n",info,#var);}else{skdTestIncOk();TestLog("testResult","���Գɹ�:%s ���ʽ:%s\r\n",info,#var);}}

typedef void (*SdkTestFun)();

typedef struct {
    char *pAsInfo;
    SdkTestFun pAutoFun;//
} TEST_MAP;

typedef s32 (*SdkManualTestFun)(char *pasOutInfo, char const *pTitle);

typedef struct {
    char *pAsInfo;
    SdkManualTestFun pAutoFun;//
} TEST_MANUAL_MAP;

typedef struct {
    TEST_MANUAL_MAP const *pMap;
    int size;
} TEST_MAP_INFO;


//��ʾ��Ϣģ�� �ڲ��Թ����������õ��û����� �� ��ʾ�û���Ϣ
extern s32 sdkTestMsgBox(u8 const
*pasInfo,
s32 keyMask
);

//�ڲ���ʱ��������õ��û������ַ���

s32 sdkTestMsgBox(u8 const
*pasInfo,
s32 keyMask
);
s32 sdkTestEditBox(u8 const
*pasTitle,
u8 const *pasInfo, u8
*pheOut,
const s32 siMinLen,
const s32 siMaxLen,
const u32 uiMode
);
u8 sdkTestInputU8(u8 const
*pasTitle,
u8 const *pasInfo
);
char sdkTestInputChar(u8 const
*pasTitle,
u8 const *pasInfo
);
u16 sdkTestInputU16(u8 const
*pasTitle,
u8 const *pasInfo
);
s16 sdkTestInputS16(u8 const
*pasTitle,
u8 const *pasInfo
);
u32 sdkTestInputU32(u8 const
*pasTitle,
u8 const *pasInfo
);
s32 sdkTestInputs32(u8 const
*pasTitle,
u8 const *pasInfo
);
s32 sdkTestInputBCD(u8 const
*pTitle,
u8 *pOut, u8 const
*pasInfo);
s32 sdkTestInputASC(u8 * pOut, u8 const
*pasTitle,
u8 const *pasInfo, s32
siMin,
s32 siMax
);

s32 sdkTestPintHex(u8 *pDst, u8 *pSrc, s32 siLen);

void sdkTestShowProcessing(const u8 *pastr);

void Test_sdkPrintTestInfo(u8 * pTitle, u8 * pInfo);

s32 Test_sdkPrintShowErr(s32 codeErr);

extern s32 sdkTestInputTestIndex(u8 const
*pasTitle,
u8 const *pasInfo
);

extern TEST_MAP_INFO GetSysManualTestInfo(void);

extern TEST_MAP_INFO GetMathManualTestInfo(void);

extern TEST_MAP_INFO GetMenuManualTestInfo(void);

extern TEST_MAP_INFO GetMPedTestInfo(void);

extern TEST_MAP_INFO GetInputManualTestInfo(void);

extern TEST_MAP_INFO GetIccManualTestInfo(void);

extern TEST_MAP_INFO GetPrintManualTestInfo(void);

extern TEST_MAP_INFO GetDispManualTestInfo(void);

extern TEST_MAP_INFO GetCommUartManualTestInfo(void);

extern TEST_MAP_INFO GetIcdealDemonTestInfo(void);

extern TEST_MAP_INFO GetIcdealFunctionTestInfo(void);

extern TEST_MAP_INFO GetMifareManualTestInfo(void);

extern TEST_MAP_INFO GetMemoryManualTestInfo(void);

extern TEST_MAP_INFO GetScannerManualTestInfo(void);

extern TEST_MAP_INFO GetMagManualTestInfo(void);

void Test_sdkToolsAuto(void);

void Test_sdkAudioAuto(void);

void Test_sdkPrintAuto(void);

void Test_sdkDispAuto(void);

void Test_sdkIccAuto(void);

void Test_sdkPedMain(void);

void Test_InputAuto(void);

void TestSdkIccMain(void);

void TestSdkMemoryMain(void);

void TestSdkMifareMain(void);

void Test_sdkSysauto(void);

void TestSdkScannerMain(void);

void TestSdk8583Main(void);

void TestsdkRecordLibMain(void);

void Test_sdkIniAuto(void);

void Test_sdkListAuto(void);

void Test_sdkQueueAuto(void);

void Test_sdkXMLAuto(void);

//shijianglong 2013.05.06 15:45
bool sdkTestIsUcos(void);

void Test_sdkDukptAuto(void);//zhouzhihua 2013.08.12 17:18

extern void Test_sdkSelectCase(u8 *pTital, TEST_MAP_INFO const *pInfo);

extern s32 sdkTestPresetInputASC(u8 * pOut, u8 const
*pasTitle,
u8 const *pasInfo, u8
*pasContent,
s32 Minlen, s32
MaxLen);

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

