#ifndef APP_PRINT_H_
#define APP_PRINT_H_

#ifndef HEATROWDOT
#define HEATROWDOT 384

#endif

enum {

    INITPRINT_ENUM = 0x49,
    STATUSPRINT_ENUM = 0x47,
    TESTPRINT_ENUM = 0x46,
    SETTINGPRINT_ENUM = 0x53,        //�����ֿ⡢��ࡢŨ�Ⱥ���������
    DATAPRINT_ENUM = 0x50,
    PRINTER_END_ENUM = 0xff
};

#if    PRINT_ABLE                //��ӡ����: �������ε���ӡ��صĴ���

typedef struct STRUCT_PRINTCMD
{
	u8 printType;
	u8 algrithmFlag;
	u8 cryptIndex;
	u8 wkLen;
	u8 macData[8];
	u16 dataLen;
	u8 *pPrintData;
	
}printcmd_struct;

s32 OnInitPrinter(ST_TRANSFLOW *pstTransFlow);
s32 OnGetPrintStatus(ST_TRANSFLOW *pstTransFlow);
s32 OnPrintTest(ST_TRANSFLOW *pstTransFlow);
s32 OnPrintData(ST_TRANSFLOW *pstTransFlow);
s32 OnPrintSetting(ST_TRANSFLOW *pstTransFlow);

void InitFont();

#endif

#endif

