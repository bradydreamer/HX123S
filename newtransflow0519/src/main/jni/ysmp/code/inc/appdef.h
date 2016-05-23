#ifndef AFX_APPDEF_H
#define AFX_APPDEF_H

#define FALSE    (0)
#define    TRUE    (1)

struct _SYS                                                                         //ϵͳ������Ҫ����
{
    u8 ucYear;                                                                      //��Χ(1949��2050),>=0x50ʱ����ʾ19xx,<0x50 ��ʾ20xx
    u8 ucMonth;                                                                     //��Χ(1��12)
    u8 ucDay;                                                                       //��Χ(1��31)
    u8 ucHour;                                                                      //��Χ(0��23)
    u8 ucMinute;                                                                    //��Χ(0��59)
    u8 ucSecond;                                                                    //��Χ(0��59)
#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304
    u8 asVoucherNo[6 + 1];                                                          //ϵͳ��ˮ��
    u8 asBatchNo[6 + 1];                                                            //ϵͳ���κ�
#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304    
};


struct _CommuParam {
    u8 ucCommMode;                                                                 //���õ�ͨ��ģʽ

    u8 asBTPin[6 + 1];                                            //��������

};
struct _Comsetting                                                                   //��������
{
    bool bisBeep;                                                                                          //��������
    bool bisExPinPad;  // 1��� 0����                                                                                             //���������
    u8 IfInOrOutRF;  // 2���� 1���       0����Ӧ                                                                                      //���ǽ�
    bool bisEL;                                                                                                                   //�����
    u8 uiIdleCntDown;                                                                                  //�͹��ĵ���ʱ( 0:������͹���, >= 5 ��λ:��)
};

struct _AppSetting                                                                              //����
{
    struct _Comsetting stComsetting;                   //��������
    struct _CommuParam stCommuParam;                    //POSͨѶ����
};


struct _TradeData {
    u8 ucTradeId;                   //��ϢID
};


typedef struct {
    SDK_ICC_PARAM stCardParam;
    SDK_ICC_CARDDATA stCardData;       //������
} STCARDINFO;
//add by Sun.xz 20150108
struct _INPUTMAP {
    u8 asTag[128];                                                         //��������tag // wuzhonglin, 2012/12/22
    bool bIsJudge;                                                        //�жϳ��Ȳ���
    u8 ucMinLen;                                                          //��С��������
    u8 ucMaxLen;                                                          //��󳤶�����
    u32 uiMask;                                                           //������
    u8 ucRowNo;                                                           //�����к�(0-4)
};


#endif

