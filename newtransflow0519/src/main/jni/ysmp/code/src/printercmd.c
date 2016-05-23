#if    PRINT_ABLE                //��ӡ����: �������ε���ӡ��صĴ���

static u8 ghePrtFontSize = 24;		//����ֻ�� 24��16 ����ֵ
static u8 ghePrtDensity = 1;		//�� G2 û�ã�G2��������Ũ��
static s32  gsiRowGap = 0;

//��4λΪF��ʾ��˫����,��4λΪF��ʾ��˫����,������
static u8 ghePrtAscFontAttr = 0x00;			
static u8 ghePrtChnFontAttr = 0x00;

bool gIsEndOfOrder=false;//�Ƿ���һ���Ľ���

enum{

	LINEKERN_ENUM = 0x0,
	DENSITY_ENUM	
};

enum{

	PRINTER_INIT_OK = 0x1,
	PRINTER_INIT_NONE	
};

static u8 printerInitStatus = 0;




cmd_Str const PrinterCmdTab[] = {

	{ INITPRINT_ENUM,   	OnInitPrinter, 			NULL },	
	{ STATUSPRINT_ENUM, 	OnGetPrintStatus, 		NULL },
	{ TESTPRINT_ENUM, 	    OnPrintTest, 			NULL }, 
	{ SETTINGPRINT_ENUM, 	OnPrintSetting, 		NULL },
	{ DATAPRINT_ENUM, 	    OnPrintData, 	    	NULL },
	{ PRINTER_END_ENUM, 	NULL, 	   		 	NULL },
};

static SDK_PRINT_FONT GetPrintFont(void)
{
    SDK_PRINT_FONT font = { 0, SDK_PRN_ASCII12X24, SDK_PRN_ZOOM_N, \
		SDK_PRN_CH24X24, SDK_PRN_ZOOM_N, 0 };
  
	if(ghePrtFontSize == 16)
	{
		font.uiAscFont = SDK_PRN_ASCII8X16;
		font.uiChFont = SDK_PRN_CH16X16;
	}
	else
	{
		font.uiAscFont = SDK_PRN_ASCII12X24;
		font.uiChFont = SDK_PRN_CH24X24;
	}

	//ASCII
	if(ghePrtAscFontAttr == 0xFF)
	{
		font.uiAscZoom = SDK_PRN_ZOOM_AB;
	}
	else if(ghePrtAscFontAttr == 0xF0)
	{
		font.uiAscZoom = SDK_PRN_ZOOM_A;
	}
	else if(ghePrtAscFontAttr == 0x0F)
	{
		font.uiAscZoom = SDK_PRN_ZOOM_B;
	}
	else
	{
		font.uiAscZoom = SDK_PRN_ZOOM_N;
	}

	//Chn
	if(ghePrtChnFontAttr == 0xFF)
	{
		font.uiChZoom = SDK_PRN_ZOOM_AB;
	}
	else if(ghePrtChnFontAttr == 0xF0)
	{
		font.uiChZoom = SDK_PRN_ZOOM_A;
	}
	else if(ghePrtChnFontAttr == 0x0F)
	{
		font.uiChZoom = SDK_PRN_ZOOM_B;
	}
	else
	{
		font.uiChZoom = SDK_PRN_ZOOM_N;
	}
	
    return font;
}



/*****************************************************************************
** Descriptions:		��ʼ����ӡ��
** Parameters:    		ST_TRANSFLOW
** Returned value:	INS_SUC	:	success initial the printer
					INS_OTHER_ERR:	failure to initial the printer
** Created By:		dongChangQiang  2014.07.25
*****************************************************************************/
s32 OnInitPrinter(ST_TRANSFLOW *pstTransFlow)
{
	u8 data = 0;
	u8 *pRecv = NULL;
	s32 ret = 0;

	pRecv = pstTransFlow->heReceiveData;
	data = pRecv[0];

Trace("dcq", "initPrint: %x\r\n", data);

	if (printerInitStatus == PRINTER_INIT_OK)
		return INS_SUC;
	
	if (data == 0x30)
	{
		/*
			��G2, ������ӡ���Ի᷵�ش�ӡ��״̬.
		*/
		
Trace("dcq", "begin initial\n");

		ret = sdkPrintInit();

Trace("dcq", "printer error status: %d\n", ret);

		if (ret != SDK_OK)
		{
			Trace("dcq", "initPrint error\n");
			return INS_OTHER_ERR;
		}
		
		printerInitStatus = PRINTER_INIT_OK;
		
		return INS_SUC;
	}
	else
		return INS_OTHER_ERR;
}

//������ӡ��DEMO

void PintDemo(void)
{
        sdkPrintInit();
		
        sdkPrintStr("����0", GetPrintFont(), SDK_PRINT_LEFTALIGN, 0, 0);
        sdkPrintStr("����1", GetPrintFont(), SDK_PRINT_LEFTALIGN, 0, 0);
        sdkPrintStr("����2", GetPrintFont(), SDK_PRINT_LEFTALIGN, 0, 0);
        sdkPrintStr("����3", GetPrintFont(), SDK_PRINT_LEFTALIGN, 0, 0);
        sdkPrintStr("����4", GetPrintFont(), SDK_PRINT_LEFTALIGN, 0, 0);
		
        sdkPrintStart();       
}

/*****************************************************************************
** Descriptions:		��ȡ��ӡ��״̬
** Parameters:    		ST_TRANSFLOW
** Returned value:	INS_SUC	:	
					INS_OTHER_ERR:	
** Created By:		dongChangQiang  2014.07.19
*****************************************************************************/

s32 OnGetPrintStatus(ST_TRANSFLOW *pstTransFlow)
{
	s32 ret = -1;
	u8 *pSend = NULL;

	pSend = pstTransFlow->heReceiveData;           //0416Send==Recv

	ret = sdkPrintInit();
	switch (ret)
	{
		case SDK_OK:
			pSend[0] = 0;
			break;
			
		case SDK_PRINT_OUTOF_PAPER:
			pSend[0] = 4;
			break;
			
		case SDK_PRINT_DEV_FAIL:
			pSend[0] = 8;
			break;	

		default:
			pSend[0] = 0;
			break;	
	}
	pstTransFlow->usSendDataLen = 1;
	
	return INS_SUC;
}


/*****************************************************************************
** Descriptions:		��ֽ
** Parameters:    		ST_TRANSFLOW
** Returned value:	INS_SUC	:	
					INS_OTHER_ERR:	
** Created By:		dongChangQiang  2014.07.19
*****************************************************************************/
s32 OnPrintTest(ST_TRANSFLOW *pstTransFlow)
{
	s32 ret = -1;
	u8 printRange = 0, printLen = 0;
	u8 *pRecv = NULL;
        
	#define HEAD_PRINTER_LATTICE (0.125)

	pRecv = pstTransFlow->heReceiveData;

	printRange = pRecv[1];
	
	printLen = (u8)(printRange / HEAD_PRINTER_LATTICE);

	#undef HEAD_PRINTER_LATTICE

	if (printerInitStatus != PRINTER_INIT_OK)
	{
		ret = sdkPrintInit();
		if (ret != SDK_OK)
		{
			Trace("dcq", "initPrint error\n");
			return INS_OTHER_ERR;
		}
		
		printerInitStatus = PRINTER_INIT_OK;
	}
	
	ret = sdkPrintFeedPaper(SDK_PRINT_ROLL_FORWARD, printLen);
Trace("dcq", "feedpaper ret: %d\n", ret);
	if (ret != SDK_OK)
		return INS_OTHER_ERR;   
    
	return INS_SUC;
}


/*****************************************************************************
** Descriptions:		�м������ָ��
					��ӡŨ������ָ��
					�ֿ�����ָ��			δʵ��
					��������ָ��			δʵ��
					
** Parameters:    		ST_TRANSFLOW
** Returned value:	INS_SUC	:	
					INS_OTHER_ERR:	
** Created By:		dongChangQiang  2014.07.19
** Modify By:			Maihaoqing 2014.12.15
** Notice:			G2��ӡŨ�Ȳ�������
*****************************************************************************/
s32 OnPrintSetting(ST_TRANSFLOW *pstTransFlow)
{
	u8 data = 0;
	u8 *pRecv = NULL;

	pRecv = pstTransFlow->heReceiveData;
	data = pRecv[0];
	
	if(pstTransFlow->ReceiveDataLen > 3 || pstTransFlow->ReceiveDataLen < 2)
		return INS_PARAM_ERR;
	
	switch(data)
	{
		case 'D':
			if(pRecv[1] == '2')
				ghePrtFontSize = 16;
			else if(pRecv[1] == '1')
				ghePrtFontSize = 24;
			else
				return INS_PARAM_ERR;
			break;
	
		case 'E':
			gsiRowGap = pRecv[1];
			Trace("dcq", "printsetting linekern: %d\n", gsiRowGap);
			return INS_SUC;
	
		case 'B':
			ghePrtDensity = pRecv[1];
			Trace("dcq", "printsetting density: %d\n", ghePrtDensity);
			return INS_SUC;

		case 'A':
			if(pRecv[1] == 'W')
				ghePrtAscFontAttr = pRecv[1] == 'D'? (ghePrtAscFontAttr|0xF0): (ghePrtAscFontAttr&0x0F);
			else if(pRecv[1] == 'H')
				ghePrtAscFontAttr = pRecv[1] == 'D'? (ghePrtAscFontAttr|0x0F): (ghePrtAscFontAttr&0xF0);
			else
				return INS_PARAM_ERR;
                        break;

		case 'C':
			if(pRecv[1] == 'W')
				ghePrtChnFontAttr = pRecv[1] == 'D'? (ghePrtChnFontAttr|0xF0): (ghePrtChnFontAttr&0x0F);
			else if(pRecv[1] == 'H')
				ghePrtChnFontAttr = pRecv[1] == 'D'? (ghePrtChnFontAttr|0x0F): (ghePrtChnFontAttr&0xF0);
			else
				return INS_PARAM_ERR;
                        break;

		default:
			return INS_PARAM_ERR;
		
	}
        
    return INS_SUC;
}

//ljh2015-01-12
u8* getline(u8 *pInData,u8 *str)
{
	u8 *ptmp=NULL;
	
	if(pInData==NULL)
		return NULL;

	ptmp = strstr(pInData,"\n");

	if(ptmp)
	{
		memcpy(str,pInData,ptmp-pInData+1);
		str[ptmp-pInData+1]='\0';//0aҲ���ƽ�ȥ
		return ptmp+1;
	}
}

#if 0
const u8* PrintCmdTab[]=
{
	//!����������� Ӱ������Ĵ�ӡ
	"!hz",
	"!asc",
	"!gray", //Ũ�ȼӴ�
	"!yspace", //�м��
	"!barcode",
	"!qrcode",

	//*����͵����� ����ͼƬ....
	"*text",
	"*feedline",
	"*line",
	"*barcode",
	"*image",
	"*pause"
};
#endif

//����ȫ�ֱ��� ���ӡ���õ�

typedef struct PrintSettingStr
{
	SDK_PRINT_FONT font;
	SDK_PRINT_ALIGNMODE eAlign;
    s32 siOffset;//
    s32 siRowGap;//:�м��
    u8  chTextHigh;
	u8  asiTextHigh;
	u8  chTextWide;
	u8  asiTextWide;
}PRINT_SETTING_STR;

PRINT_SETTING_STR gPrintSetting;

//ljh2015-01-12 ��ʼ����ӡ���õĲ���
void InitFont()
{
	gPrintSetting.font.uiAscFont=  SDK_PRN_ASCII12X24;  
    gPrintSetting.font.uiAscZoom=SDK_PRN_ZOOM_N;  
    gPrintSetting.font.uiChFont=SDK_PRN_CH24X24;    
	gPrintSetting.font.uiChZoom=SDK_PRN_ZOOM_N;  
	

	gPrintSetting.eAlign=SDK_PRINT_LEFTALIGN;
    gPrintSetting.siOffset=0;//
    gPrintSetting.siRowGap=0;//:�м��
    gPrintSetting.chTextHigh=24;
	gPrintSetting.asiTextHigh=24;
	gPrintSetting.chTextWide=24;
	gPrintSetting.asiTextWide=12;
	
}

//ljh2015-01-12 ��ӡ����
u8 PrintData(u8 *pInData)
{
	u8 *p=pInData,*pTmp=NULL;
	u8 cmdStr[20];
	u8 i=0;
	s32 iLen;
	SDK_PRINT_ALIGNMODE Align=SDK_PRINT_LEFTALIGN;	
	u8 parm1[10],parm2[10],parm3[10];
	u8 sepLine[50];
	u8 changeLine[10];
	u8 *pchangeLine;
	u8 *printStr = NULL;
	u8 *str = sdkGetMem(512);

	if(!str)
		return 0;
	memset(str,0,512);
	printStr = sdkGetMem(512);
	if(!printStr)
	{
		sdkFreeMem(str);
		return 0;
	}
	memset(printStr,0,512);
	
	while( (pTmp=getline(p,str))!=NULL )
	{
		p = pTmp;
		Trace("Lai","=====>line:%s\n",str);

		memset(cmdStr,0,sizeof(cmdStr));
		sscanf(str,"%s",cmdStr);

		if(strcmp("!hz",cmdStr)==0)
		{
			memset(parm1,0,sizeof(parm1));
			sscanf(str,"%s %s",cmdStr,parm1);
			Trace("Lai","*** hz parm1:%s\n",parm1);
			if(strcmp("s",parm1)==0)//(s,n,l,sn,sl,nl) 
			{	
				gPrintSetting.font.uiChFont = SDK_PRN_CH16X16;
				gPrintSetting.font.uiChZoom = SDK_PRN_ZOOM_N;
				gPrintSetting.chTextHigh=16;
			}
			else if(strcmp("n",parm1)==0)
			{
				gPrintSetting.font.uiChFont = SDK_PRN_CH24X24;
				gPrintSetting.font.uiChZoom = SDK_PRN_ZOOM_N;
				gPrintSetting.chTextHigh=24;
			}
			else if(strcmp("l",parm1)==0)
			{
				gPrintSetting.font.uiChFont = SDK_PRN_CH32X32;
				gPrintSetting.font.uiChZoom = SDK_PRN_ZOOM_N;
				gPrintSetting.chTextHigh=32;
			}
			else if(strcmp("sn",parm1)==0)//16x24 16x32
			{
				gPrintSetting.font.uiChFont = SDK_PRN_CH16X16;
				gPrintSetting.font.uiChZoom = SDK_PRN_ZOOM_B;
				gPrintSetting.chTextHigh=24;
			}
			else if(strcmp("sl",parm1)==0)//16x32
			{
				gPrintSetting.font.uiChFont = SDK_PRN_CH16X16;
				gPrintSetting.font.uiChZoom = SDK_PRN_ZOOM_B;
				gPrintSetting.chTextHigh=32;
			}
			else if(strcmp("nl",parm1)==0)//24x32  24x48
			{
				gPrintSetting.font.uiChFont = SDK_PRN_CH24X24;
				gPrintSetting.font.uiChZoom = SDK_PRN_ZOOM_B;
				gPrintSetting.chTextHigh=32;
			}
		}
		else if(strcmp("!asc",cmdStr)==0)
		{
			memset(parm1,0,sizeof(parm1));
			sscanf(str,"%s %s",cmdStr,parm1);
			Trace("Lai","*** asc parm1:%s\n",parm1);
			
			if(strcmp("s",parm1)==0)//(s,n,l,sn,sl,nl) 
			{	
				gPrintSetting.font.uiAscFont = SDK_PRN_ASCII8X16;
				gPrintSetting.font.uiAscZoom = SDK_PRN_ZOOM_N;
				gPrintSetting.asiTextHigh=16;
				gPrintSetting.asiTextWide = 8;
			}
			else if(strcmp("n",parm1)==0)
			{
				gPrintSetting.font.uiAscFont = SDK_PRN_ASCII12X24;
				gPrintSetting.font.uiAscZoom = SDK_PRN_ZOOM_N;
				gPrintSetting.asiTextHigh=24;
				gPrintSetting.asiTextWide = 12;
			}
			else if(strcmp("l",parm1)==0)
			{
				gPrintSetting.font.uiAscFont = SDK_PRN_ASCII16X32;
				gPrintSetting.font.uiAscZoom = SDK_PRN_ZOOM_N;
				gPrintSetting.asiTextHigh=32;
				gPrintSetting.asiTextWide = 16;
			}
			else if(strcmp("sn",parm1)==0)//
			{
				gPrintSetting.font.uiAscFont = SDK_PRN_ASCII8X16;
				gPrintSetting.font.uiAscZoom = SDK_PRN_ZOOM_B;
				gPrintSetting.asiTextHigh=24;
				gPrintSetting.asiTextWide = 8;
			}
			else if(strcmp("sl",parm1)==0)//
			{
				gPrintSetting.font.uiAscFont = SDK_PRN_ASCII8X16;
				gPrintSetting.font.uiAscZoom = SDK_PRN_ZOOM_B;
				gPrintSetting.asiTextHigh=32;
				gPrintSetting.asiTextWide = 8;
			}
			else if(strcmp("nl",parm1)==0)//
			{
				gPrintSetting.font.uiAscFont = SDK_PRN_ASCII12X24;
				gPrintSetting.font.uiAscZoom = SDK_PRN_ZOOM_B;
				gPrintSetting.asiTextHigh=32;
				gPrintSetting.asiTextWide = 12;
			}
		}
		else if(strcmp("!gray",cmdStr)==0)
		{
			Trace("Lai","*** !gray.................\n");
			//�Ƿ�֧��?
		}
		else if(strcmp("!yspace",cmdStr)==0)
		{
			Trace("Lai","*** !yspace.................\n");
			//memset(parm1,0,sizeof(parm1));
			sscanf(str,"%s %d",cmdStr,&gPrintSetting.siRowGap);
		}
		else if(strcmp("!barcode",cmdStr)==0)
		{
			Trace("Lai","*** !barcode.................\n");
		}
		else if(strcmp("!qrcode",cmdStr)==0)
		{
			Trace("Lai","*** !qrcode.................\n");
		}
		else if(strcmp("*text",cmdStr)==0)
		{
			Trace("Lai","*** *text.................\n");
			memset(printStr,0,sizeof(printStr));
			sscanf(str,"%s %c",cmdStr,parm1);

			if( (pchangeLine=strstr(str,"\n")) != NULL )
			{
				iLen = strlen("*text l ");
				memcpy(printStr, str+iLen, pchangeLine-(str+iLen)+1);
			}
			
			if(parm1[0]=='l')
				Align = SDK_PRINT_LEFTALIGN;
			else if(parm1[0]=='r')
				Align = SDK_PRINT_RIGHTALIGN;
			else if(parm1[0]=='c')
				Align = SDK_PRINT_MIDDLEALIGN;

			//strcat(printStr,"\n");//sscanfȥ���˻��з�
			sdkPrintStr(printStr, gPrintSetting.font, Align, 0, gPrintSetting.siRowGap);
		}
		else if(strcmp("*feedline",cmdStr)==0)
		{
			Trace("Lai","*** *feedline.................\n");
			memset(changeLine,0,sizeof(changeLine));
			sscanf(str,"%s %d",cmdStr,parm1);//��֧�ֱߴ����ֽ,ֱ�Ӵ����
			
			for(i=0;i<parm1[0];i++)
				strcat(changeLine,"\n");

			strcat(changeLine," ");
			sdkPrintStr(changeLine, gPrintSetting.font, SDK_PRINT_LEFTALIGN, 0, gPrintSetting.siRowGap);
			//sdkPrintFeedPaper(SDK_PRINT_ROLL_FORWARD,gPrintSetting.chTextHigh*parm1[0]);
			if(parm1[0]>=4)
				gIsEndOfOrder = true;
				//���ȸ����Ƿ��4�����ж��Ƿ���һ���Ľ��� �����ٵ���
		}
		else if(strcmp("*line",cmdStr)==0)
		{
			Trace("Lai","*** *line.................\n");
			memset(sepLine,0,sizeof(sepLine));
			memset(sepLine,'-', 384 / gPrintSetting.asiTextWide);
			sdkPrintStr(sepLine, gPrintSetting.font, Align, 0, gPrintSetting.siRowGap);
		}
		else if(strcmp("*barcode",cmdStr)==0)
		{
			Trace("Lai","*** *barcode.................\n");
		}
		else if(strcmp("*image",cmdStr)==0)
		{
			Trace("Lai","*** *image.................\n");
			memset(parm1,0,sizeof(parm1));
			memset(parm2,0,sizeof(parm2));
			memset(parm3,0,sizeof(parm3));
			
			sscanf(str,"%s %c %s %s",cmdStr,parm1,parm2,parm3);
			
			if(parm1[0]=='l')
				Align = SDK_PRINT_LEFTALIGN;
			else if(parm1[0]=='r')
				Align = SDK_PRINT_RIGHTALIGN;
			else if(parm1[0]=='c')
				Align = SDK_PRINT_MIDDLEALIGN;

			if(strcmp( "#ums",parm3)==0)
			{
				if(sdkAccessFile("/mtd0/ums.bmp"))
					sdkPrintBitMap("/mtd0/ums.bmp", Align, 0);
			}
		}
		else if(strcmp("*pause",cmdStr)==0)
		{
			Trace("Lai","*** *pause.................\n");
			sscanf(str,"%s %d",cmdStr,parm1);
			sdkmSleep(parm1[0]*1000);
		}
		else
		{
			
		}

		//for(i=0;i<cmdCount;i++)
		//{
		//	if(strcmp(PrintCmdTab[i],cmdStr)==0)
		//	{
		//		
		//	}
		//}
		memset(str,0,512);
		memset(printStr,0,512);
	}

	sdkFreeMem(str);
	sdkFreeMem(printStr);
	return 1;
}



/*****************************************************************************
** Descriptions:		��ӡָ��
** Parameters:    		ST_TRANSFLOW
** Returned value:	INS_SUC	:	
					INS_OTHER_ERR:	
					
** Created By:		dongChangQiang  2014.07.19
*****************************************************************************/

/*
	ID		������				����			��ע

	1		��ӡ����			  1				0x5A ��ӡ�ַ�
													0x4A �ű���ӡģʽ

	2		�㷨��ʶ			  1				0xff ��ʶ����֤MAC
													�����㷨����
	
	3		��Կ����			  1				0-255, ����Կ����/ ������Կ����,��Ϊ
													����Կ������ID4, ID5 ��ָ��������Կ
													, ��Ϊ������Կ������ID4 Ϊ0 ��������
													ID4.

	4		������Կ			LL...24			

	5		MAC DATA				  8				����㷨��ʶΪ0xff, ����ֶ���0xFF ���

	6		DATA					LL...1024
	
*/

s32 OnPrintData(ST_TRANSFLOW *pstTransFlow)
{
	s32 ret = -1;
	u16 count = 0;
	u8 *pRecv = NULL, bitMapInfo[6];//, *databuf;//[1025] = {0};
	u16 width = 0, hight = 0, offset = 0;


        sdkIccTransFreeAllMem();

	printcmd_struct printcmd;
	memset(&printcmd,0,sizeof(printcmd));
	printcmd_struct *pPrintCmd = &printcmd;
	
	pRecv = pstTransFlow->heReceiveData;

	//databuf = sdkGetMem(1026);
	//if(!databuf)
	//	return INS_OTHER_ERR;
	//memset(databuf,0,1026);
	
	ret = sdkPrintInit();
	if (ret != SDK_OK)
	{
		//sdkFreeMem(databuf);
		return INS_OTHER_ERR;
	}
	printerInitStatus = PRINTER_INIT_OK;

	pPrintCmd->printType = pRecv[count];
	count += 1;
	
	pPrintCmd->algrithmFlag = pRecv[count];
	count += 1;
	
	pPrintCmd->cryptIndex = pRecv[count];
	count += 1;

	sdkBcdToU16(&pPrintCmd->wkLen, &pRecv[count], 2);
	count += 2;

	if (pPrintCmd->printType == 0x4D)
		memcpy(bitMapInfo, &pRecv[count], pPrintCmd->wkLen);

	count += pPrintCmd->wkLen;
	memcpy(&pPrintCmd->macData, &pRecv[count], 8);
	count += 8;

	sdkBcdToU16(&pPrintCmd->dataLen, &pRecv[count], 2);
	count += 2;
	if(pPrintCmd->dataLen > 1023)
	{
		//sdkFreeMem(databuf);
		return INS_PARAM_ERR;
	}

	pPrintCmd->pPrintData = &pRecv[count];
	
	if (pPrintCmd->printType == 0x5A)
	{
		//memcpy(databuf, pPrintCmd->pPrintData, pPrintCmd->dataLen);
		if(pPrintCmd->dataLen <= 0)
                    return INS_OTHER_ERR;
		if(pPrintCmd->pPrintData[pPrintCmd->dataLen - 1] != '\n')       //zcl ��ӡ�����һ������û��\n �˴��Լ���һ��
                {
                    pPrintCmd->pPrintData[pPrintCmd->dataLen] = '\n';
                    pPrintCmd->dataLen ++;
                }
		pPrintCmd->pPrintData[pPrintCmd->dataLen] = '\0';

#if 0
		ret = sdkPrintStr(databuf, GetPrintFont(), SDK_PRINT_LEFTALIGN, 0, gsiRowGap);
		Trace("dcq", "printstr: %d", ret);
		if (ret != SDK_OK)
			return INS_OTHER_ERR;
	
		sdkPrintFeedPaper(SDK_PRINT_ROLL_FORWARD, 200);	
#endif
		if(!PrintData(pPrintCmd->pPrintData))
		{
			Trace("Lai","PrintData �ڴ治��.............\n");
			//sdkFreeMem(databuf);
			return INS_OTHER_ERR;
		}
	}
	else if (printcmd.printType == 0x4D)
	{
		sdkHexToU16(&width, bitMapInfo, 2);				// ���
		sdkHexToU16(&hight, &bitMapInfo[2], 2);			// �߶�
		sdkHexToU16(&offset, &bitMapInfo[4], 2);		// ͼ�δ�ӡƫ��
              
		if (width + offset > 384)
		{
			width = width - ((width + offset) - 384) / 2;
			offset = offset - ((width + offset) - 384) / 2 - 1;
		}
#if 0 		
		if (pstTransFlow->ReceiveDataLen >= sizeof(pstTransFlow->heReceiveData))
		{
			pstTransFlow->heReceiveData[sizeof(pstTransFlow->heReceiveData)-1] = '\0';
		}
		else
		{
			pRecv[count + printcmd.dataLen] = '\0';
		}
				
		//num = pPrintCmd->dataLen / lineNum;
		//numLeft = pPrintCmd->dataLen % lineNum;
		for (i=0; i<num; i++)	
		{
			memcpy(databuf, pPrintCmd->pPrintData+len, lineNum);
			len += lineNum;
			databuf[lineNum] = '\0';
			ret = sdkPrintBitMap(databuf, SDK_PRINT_LEFTALIGN, offset);
			if (ret != SDK_OK)
				return INS_OTHER_ERR;
		}
		if (numLeft)
		{
			ret = sdkPrintBitMap(&pRecv[len], SDK_PRINT_LEFTALIGN, offset);
			if (ret != SDK_OK)
				return INS_OTHER_ERR;
		}
#endif	
		ret = sdkPrintBitMap(NULL, SDK_PRINT_LEFTALIGN, offset);
		if (ret != SDK_OK)
		{
			return INS_OTHER_ERR;
		}
		sdkPrintFeedPaper(SDK_PRINT_ROLL_FORWARD, 200);	
		
	}
	else 
	{
		//sdkFreeMem(databuf);
		return INS_OTHER_ERR;
	}

	Trace("dcq", "sdkPrintStart...........");

	if(gIsEndOfOrder)
	{
		gIsEndOfOrder = false;
		sdkPrintStart();
	}
	else
		sdkPrintStartNoRollPaper();
	//sdkPrintStartNoRollPaper();
	//sdkFreeMem(databuf);
	return INS_SUC;
	
}




#endif	//PRINT_ABLE	��ӡ����: �������ε���ӡ��صĴ���

