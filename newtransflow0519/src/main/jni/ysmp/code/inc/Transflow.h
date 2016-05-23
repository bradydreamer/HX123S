#ifndef AFX_TRANSFLOW_H
#define AFX_TRANSFLOW_H


//======================ָ�����====================================
enum INS_RETURN {
    INS_KEEP = (SDK_APP_RET_OK_MIN + 1),   //ָ��ȴ�
    INS_SUC, //ָ��ִ�гɹ�������ִ����һ��ָ��
            INS_OVERTIME, //ָ��ִ�г�ʱ
            INS_EXIT, //�˳�
            INS_REPEAT, //�ظ�ִ��һ��ָ��
            INS_OUTMESSAGE, //����Ϣ����
            INS_REPEATBEGIN, //���¿�ʼִ�����̴���
            INS_NOSUPPORT,        //��֧�ֵ�ָ��
    INS_PARAM_ERR,    //��������
    INS_OTHER_ERR,    //��������
    INS_NOT_RETURN,  //�����ظ��ֻ���
    INS_SEND_NOT_OUT  //���ر��ĵ����˳���Ϣ����
};

//================�������ݽṹ��===========================
typedef struct {
    u8 EnableFallback;                 //�Ƿ�֧��FALLBACK,01=֧�֣�00=��֧��
    u8 ForceOnline;             //  �Ƿ�ǿ������
    u8 Online2Offline;                   //�Ƿ���������ת�ѻ�
    u8 EnableBlackList;           //�Ƿ�ʹ�ú�����
    u8 EnableQPBOC;                //�Ƿ�֧��Qpboc
    u8 EnableRFPBOC;        //�Ƿ�֧�ַǽ�PBOC
    u8 BIsSupportEc; // 0:��֧�ֵ����ֽ�; 1:֧�ֵ����ֽ�
    u8 BIsPromptChooseEc;  // �Ƿ���ʾѡ������ֽ�
    u8 BIsDefaultEc; // ����ʾѡ������ֽ����ѡ������ֽ�ʱ,ʹ��ȱʡֵ1:ʹ�õ����ֽ�;0:��ʹ�õ����ֽ�
    u8 MerchantIndex;             //ָ��������ʹ�õ��̻���Ϣ������
    u8 OffPrintIndex;           // ָ��������ʹ�õ��ѻ���ӡ�б�
    u8 AidGroupIndex;        //ָ������ʹ�õ�AID��������
    u8 TransType;           //ָ����������
    u8 EcFlowType;           //�����ֽ�����
} _IcTranIniStu;


//=========================�������̽ṹ============================================
typedef struct ST_TRANSFLOW {
    /***************�����������**********************/
    //u8  heSendData[1024];             //�������ݻ���			//0416Send==Recv
    u16 usSendDataLen;                //�������ݳ���

    /***************������������**********************/
    u8 heReceiveData[1536];                 //�������ݻ���
    u16 ReceiveDataLen;                   //�������ݳ���
    u16 usCurrentDealLen;                   //�Ѵ���������ݳ���

    /******************ִ��ָ�����*************************/
    u8 cmdClass;                                //ָ����
    u8 cmdID;                                    //ָ��
    u8 ucInsRunStep;                          //ָ��ִ�в���
    s32 ucFuncRunStep;                       //������������
    u8 ucRepeatTimes;                       //�ظ�����
    u8 bisSwipingCard;                     //ָ���Ƿ�ˢ��
    u32 InsClockTimer;                        //ָ�ʱ��
    u8 heFindCmd;                            //����ָ��
    u8 heFindCmdPos;                        //��ִ�е�ָ���λ��
    u8 heFindCmdTabPos;                        //��ִ�е�ָ����λ��
    u8 asCmdAck[3];                            //Ӧ����

    //================IC����ʼ�����׽ṹ��=================================
    _IcTranIniStu InitIcTranStu;
    u8 bFallback;
    //��ǰ�����Ƿ�Ϊfallback  1=�� 0=��
    u8 bOnline;
    //��ǰ�����Ƿ�Ϊ�ѻ���bOnline 0=�� 1=��
    u8 QpbocOnlineFlag;
    //�ѻ���־��1ΪQPBOC��Ҫ����,2Ϊ�ѻ���׼
    u8 TradeType;        //ic����������
    /******************�������************************/
    u8 heIndicatbit;                                            //ָʾλ
    u8 ucNum;                                                    //ͬ�����

    u8 flag1;                                                    //ִ��ִ�б�ʶ����λ
    u8 flag2;
    u8 flag3;
    //==============��̬�ڴ����================================
    void *TempPointer;                                //��ʱָ��
} ST_TRANSFLOW;


enum ICC_TradeType {
    TRADE_QUERY = 31,            //����ѯ
    TRADE_SALE = 00,            //����
    TRADE_UNSALE = 20,            //���ѳ���
    TRADE_REFUND = 20,            //�˻�
    TRADE_DESGNLOAD = 60,           //ָ���˻�Ȧ��
    TRADE_LOAD = 62,           //��ָ���˻�Ȧ��
    TRADE_ECCASH = 63,            //�����ֽ��ֽ��ֵ
    TRADE_UNECCASH = 17,            //�����ֽ��ֽ��ֵ����
    TRADE_MAGLOAD_AFFIRM = 63,             //�ſ��ֽ��ֵȷ��
    TRADE_MAGLOAD_VERIFY = 33             //�ſ��ֽ��ֵ�˻���֤
};

enum TRADETYPE {
    MAG = 0,            //�ſ�
    INTEGRAL = 1,        //�������
    SIMPLE              //��������
};

//���׽��ܱ���
typedef struct _TradeRevBAG {                    //��������
    u8 ucType;                                                            //��������
    u8 heMsglen[2];                                                     //���ĳ���
    u8 hecmdClass;
    u8 heCmdId;
    u8 heIndicatbit;                                            //ָʾλ
    u8 ucNum;                                                             //ͬ�����
    u8 heData[1048];                                           //����
} ST_TRADEREVBAG;

extern s32 PostTransFlow(u8 *buf, u16 len);

extern void InitTransFlow(ST_TRANSFLOW *TransFlow);

extern void EndTransFlow(ST_TRANSFLOW *TransFlow, u8 Normal);

extern s32 DealTransFlow(SDK_MSG * msg);

extern s32 DealInsFunc(ST_TRANSFLOW *TransFlow);


#endif
