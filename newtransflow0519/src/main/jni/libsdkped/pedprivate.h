#ifndef _PEDPRIVATE_H_
#define _PEDPRIVATE_H_


#define DC_CMD_CTRL_RES "\x11"  //dcָ���еĿ����������
//DCָ����������
#define DC_CMD_EX_DUKPT_INIT  0x1011     //dukpt��ʼ��
#define DC_CMD_EX_DUKPT_GET_PIN  0x1012  //�������
#define DC_CMD_EX_DUKPT_GET_MAC  0x1013  //���mac
#define SDK_DUKPT_KSN_LEN 10
#define SDK_DUKPT_KEY_LEN 16

/*=======BEGIN: taowz 2014.05.29  11:55 modify===========*/
#define  TMK_INDEX_AREA         0//����Կ�洢��
#define  WK_INDEX_AREA          1//������Կ�洢��

typedef struct _strInnerTKeyDEncryptPara {
    const u8 *m_indata; //���ӽ�������
    u8 *m_outdata; //�ӽ��ܽ�� 
    u32 m_len; //���ӽ������ݳ���,����Ϊ8�ı���
} strInnerTKeyDEncryptPara;

typedef struct _strInnerHKeyDEncryptPara {
    u32 m_area; //��Կ����
    u32 m_index; //��Կ����
    u32 m_half; //��Կǰ��벿
    const u8 *m_indata; //����������
    u8 *m_outdata; //���ܽ��
    u32 m_len; //���������ݳ���
} strInnerHKeyDEncryptPara;


#define PED_DATAOK                            0X20        /*��֡��ȷ*/
#define PED_DATAERR                            0X2F        /*��֡�����������·���*/
/***************************������̲���ͨѶ����*************************/
#define POS_PEDDISPSTR                                  0x76
#define PED_KEYINMONEY                    0X83        /*������̷��͵����ѽ��*/
#define POS_CANCELKEY                        0X8C        /*ȡ������*/
#define POS_DOWNPOSNO                    0x9f        //���ػ��ߺ�
#define POS_PEDSAVEDATA                                     0x03         //�������кţ������ڴ�����
#define POS_PEDREADDATA                                     0x01      //��0x03���ʹ��
#define POS_GETPEDSN                                     0x04
#define POS_SETPEDLANGUAGE                          0x75
#define POS_UPLOADPARA                        0x9d
#define POS_DOWNLOADPARA                    0x9e
#define POS_ASK_VERSION                    0xBE        /*��51��������̰汾��Ϣ*/
#define PED_ACK_VERSION                    0xB1         /*51��������̷��ذ汾��Ϣ*/
#define PED_ACK_VERSTAKE                    0xB9        //�����汾
#define POS_ASK_VERSTAKE                    0xBA        //�����汾
#define PED_DENCRYPTTAK                             0xE0        //�ش����ܺ��TAK
#define POS_TRADEAMOUNT                    0x8A        //Pos����������������ѽ��
#define POS_BALANCE                        0x8E        //���
#define POS_DECRYPTWK                          0xF9        //���ܹ�����Կ
#define PED_CHECKWK                        0xF2        //У�鹤����Կ
#define POS_FLUSHWK                        0xF8        //���²��洢������Կ
#define POS_PINMODE                        0xE9        //����PIN���㷨,�����ʺż��ܵȴ���
#define PED_PINCODE                        0xE5        //����PIN������Ľ��
#define POS_ENCRYPTDATA                    0xFA        //�κ����ݵļ���
#define PED_ENCRYPTEDCODE                0xF0        //���ܺ������
#define POS_DECRYPTCODE                    0xED        //��������
#define PED_DECRYPTEDDATA                0xE8        //���ܺ������
#define POS_UPDATETMKMODE                0xEE        //��������Կ��ʽ,���ڿ��Բ������Ĵ���
#define POS_UPDATEONETMK                0xFB        //����һ������Կ
#define POS_UPDATEMULTITMK                0xFC        //���¶������Կ
#define POS_FLUSHTMK                    0xEC        //���²��洢����Կ(ֻ���Ϻ���Կ���ز���)
#define POS_DELETETMK                    0xEA        //ɾ������Կ
#define POS_CANCEL                        0x8C        //ȡ��������̲���
#define PED_FLUSHWKERROR                0xF3        //���¹�����Կʧ��
#define PED_FLUSHWKOK                    0xF4        //���¹�����Կ�ɹ�
#define PED_ENCRYPTERROR                0xF6        //��(��)��ʧ��
#define PED_DELETETMKOK                    0xE1        //ɾ������Կ�ɹ�
#define PED_DELETETMKERROR                0xE2        //ɾ������Կʧ��
#define PED_PINCODERROR                    0xE4        //PIN����ʧ��
#define POS_VERIFYAMOUNT                0xBB        //POS��ʵ���
#define PED_VERIFYAMOUNT                0xBC        //������̺�ʵ���
#define PED_REQUESTRANDOM                0x82        //�������������
#define POS_RANDOM                        0x8B        //���������
#define POS_DD_COMMAND                    0xDD        //����0xDDָ��
#define POS_DB_COMMAND                    0xDB        //����0xDBָ��
#define POS_DC_COMMAND                    0xDC        //����0xDCָ��
#define POS_NOPHRASEREMOTE                0xB0        //Զ������͸����ռ(����ָ��)
#define POS_TDA8023                        0xA0        //��ѯ�����Ƿ���Tda8023(����ָ��)
#define POS_INPUTAMT                                0x8d
#define PED_RESPONSEDATA                0x71        //����IC������Ӧ������
#define PED_CARDIN                        0x76        //�п�����ĳ�ſ���
#define PED_CARDOUT                    0x77        //�п���ĳ�ſ����γ�
#define PED_RESETDATA                    0x78        //��λIC����Ӧ����
#define PED_RESETFAIL                    0x72        //��λʧ��
#define POS_RESETCARD                    0x7B        //��λIC������
#define POS_OPERATECARD                0x7C        //����IC��
#define POS_MEMORY                      0xCA        //����memory�� //huxiang add 091201
#define PED_OPERATEEND                    0x7D        //����IC������   //sxl 20061128
#define PED_CARDSTATE                0x73        //��ѯ�����ϵ�״̬
#define POS_DOWNTMK                         0x69        //������Կ
#define POS_INITKEYIC                           0xfd
#define PED_INITKEYICOK                            0xf7
//��չЭ��֡��Э����
#define MODEMINIEX                      0x3D        //Modem��չ����Э��
//��չЭ��֡Ӧ��������
#define MODEMCFGEXDAMAGE                0x35        //Modem��չ����������Э��
#define REMOTEDOWNLOAD                  0x19        //Զ������
#define NOLINE                          0X33        /*û�ӵ绰�߻�绰�߶�*/
#define ENGAGED                         0X34        /*�绰��æ��ռ��*/
/******************huxiang add 100109**********************/
#define POS_PCIMKDES                    0x20        //����Կ�ӽ���
#define POS_PCIWKDES                    0x21        //������Կ�ӽ���
#define POS_PCIWKE99                    0x22        //����E99�㷨,���֧��500�ֽڱ���
#define POS_PCIMKFLUSH                    0x23        //ָ��������ʱ����������Կ
#define POS_PCIWKFLUSH                    0x24        //ָ��������ʱ������������Կ
#define POS_PCITMPDES                    0x25        //��ʱ������Կ�ӽ���
#define POS_PCIWKE919                    0x26        //������ԿE919�㷨
#if 0 /*Modify by ���ƽ at 2014.06.26  11:29 */
/*=======BEGIN: taowz 2014.05.23  14:20 modify===========*/
#define POS_PCIEXENCRYPT                0x41        //��չ�����㷨����
#define POS_PCIEXDECRYPT                0x42        //��չ�����㷨����
#endif /* if 0 */
/*====================== END======================== */
/**********************************************************/
/***********************����Կ����************************/
//0-99 ��DES����,100Ϊ������Կ�� 101Ϊ����Կ��ʱ��
//102-201 3DES���� 202Ϊ3DES��ʱ��

/***********************������Կ����************************/
//2009/12/31��ʼ,�µ���Կ��֧�����100�鹤����Կ.

//0-19 150-254��DES���� ���128��, 20,21Ϊ��ʱ������,
//22-41 44-149 Ϊ3DES����,42,43Ϊ3DES��ʱ����
//��ʱ��������������ڹ�������ķ�Χ,������������������ѡ��15,16
/*******************************3DES������Կ�洢���****************************/
#if SDK_IS_UCOS() || SDK_IS_DDI_PLAM() /*Modify by ���ƽ at 2014.02.25  14:27 */
#define MK_DESTHRESHOLD				0										//DES����Կ��ƫ��λ�� 0-99 (100)
#define MK_3DESTHRESHOLD				102										//3DES����Կ��ƫ��λ�� 102-201 (100)

#define WK_DESTHRESHOLD				0										//DES������Կ��ʼλ�� 0-19 150-254  (128)
#define WK_3DESTHRESHOLD				22										//3DES������Կ��ʼλ�� 22-41 44-148 (128)

#define WK_DESTHRESHOLDEX			150										//DES������Կ��չ��ʼλ�� 0-19 150-254  (128)
#define WK_3DESTHRESHOLDEX			44										//3DES������Կ��չ��ʼλ�� 22-41 44-148 (128)
/*=======BEGIN: taowz 2014.05.22  14:8 modify===========*/
#define WK_DESTHRESHOLD_DDI			0										//DES������Կ��չ��ʼλ�� 0-127  (128)
#define WK_3DESTHRESHOLD_DDI			128										//3DES������Կ��չ��ʼλ�� 128-255 (128)

/*====================== END======================== */
/*******************************������Կ�洢���****************************/
#define TPK_STORENO					0x00									//�洢TPK��������̵ı��
#define TAK_STORENO					0x01									//�洢TAK��������̵ı��
#define TPK_3DESSTORENO				(TPK_STORENO+WK_3DESTHRESHOLD)		//�洢TPK��������̵ı��,ƫ����22
#define TAK_3DESSTORENO				(TAK_STORENO+WK_3DESTHRESHOLD)		//�洢TPK��������̵ı��,ƫ����22

#define TPK_STORENOEX					(TPK_STORENO+WK_DESTHRESHOLDEX)		//�洢TPK��������̵ı��
#define TAK_STORENOEX					(TAK_STORENO+WK_DESTHRESHOLDEX)		//�洢TAK��������̵ı��
#define TPK_3DESSTORENOEX			(TPK_STORENO+WK_3DESTHRESHOLDEX)	//�洢TPK��������̵ı��,ƫ����22
#define TAK_3DESSTORENOEX			(TAK_STORENO+WK_3DESTHRESHOLDEX)	//�洢TPK��������̵ı��,ƫ����22

/********************************�ݴ湤����Կ�洢���***********************/
#define TPK_TEMPSTORENO				20										//�ݴ�TPK��������̵ı��(С������ָ��)
#define TAK_TEMPSTORENO				21										//�ݴ�TAK��������̵ı��
#define TPK_3DESTEMPSTORENO			(TPK_TEMPSTORENO+WK_3DESTHRESHOLD)	//�ݴ�TPK��������̵ı��(С¬ָ��)
#define TAK_3DESTEMPSTORENO			(TAK_TEMPSTORENO+WK_3DESTHRESHOLD)	//�ݴ�TPK��������̵ı��(С¬ָ��)

#define PCI_DESTEMPSTORENO			0                                   //��������ʱ������ 0-15 ֧������Կ�͹�����Կ
#define PCI_3DESTEMPSTORENO			16                                  //��������ʱ������ 16-31֧������Կ�͹�����Կ

#define DISPAGE1 0
#define DISPAGE2 1
#define DISPAGE3 2
#define DISPAGE4 3
#define DISPAGE5 4

typedef enum _PARSESTEP
{
    STEP_SOH = 0,               // ��������ͷ/��ʶ
    STEP_FID,
    STEP_ORDER,                         // ��������/����֡
    STEP_LEN1,                               // ���ճ��ȵ�һλ
    STEP_LEN2,                        // ���ճ��ȵڶ�λ
    STEP_DATA,                                // ��������
    STEP_ETX,                                  // ���ս�����
    STEP_BCC                                    // ����У���
}PARSESTEP;
#endif /* if 0 */
// ����λ��ͨѶ����
#define PED_SOH                         0x01
#define PED_ETX                          0x03

#define PED_KEY_TEXT                0X12               // ���̵�����
#define PED_KEY_ORDER            0X13               // ���̵�����
#define PED_KEY_TEXTEX           0x15

#define PED_SIM_TEXT            0x16                //Sim������ͨ��Э��
#define PED_SIM_ORDER            0x17                //Sim������ͨ��Э��
#define PED_SIM_TEXTEX            0x24                //Sim������ͨ����չЭ��


#define RECVBUFMAXLEN       1024
#define RECVCMDMAXNUM       5

union _DataType {
    u8 Uni8[4];
    u16 Uni16[2];
    u32 Uni32;
};
typedef struct _RECVCFG {
    u8 ucFrameId;
    //u8 ucCmd;               // ���յ�������
    u16 uiCmd;               // ���յ������� //zhouzhihua 2013.07.25 13:57
    u8 ucStep;
    u8 ucBCC;                      // У���
    u8 heRecvBuf[RECVBUFMAXLEN];            // ����
    u16 usLen;                 // ���ݳ���
    u16 usParseLen;           // ���ݽ��յ��ĳ���
    //u8 heExpectRecvCmd[RECVCMDMAXNUM];                      //  �����յ��Ļظ�����
    u16 heuiExpectRecvCmd[RECVCMDMAXNUM];
    //zhouzhihua 2013.07.24 17:43Ϊ�˼���DCָ���е���չָ��
    s32 iTimeOut;
} RECVCFG;

typedef struct _SENDCFG {
    u8 ucCmd;                                             // ���͵�����
    u8 *pheSendBuf;                                  // ���͵�����
    u16 usLen;                                            // �������ݵĳ���
} SENDCFG;

typedef struct _UARTCFG {
    SENDCFG stSendData;
    RECVCFG stRecvData;           // ���մ������ݽṹ��
} UARTCFG;

#endif
