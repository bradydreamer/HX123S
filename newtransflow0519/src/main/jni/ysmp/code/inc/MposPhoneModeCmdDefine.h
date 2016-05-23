#ifndef MPOS_PHONE_MODE_CMD_DEFINE_H_
#define MPOS_PHONE_MODE_CMD_DEFINE_H_


typedef enum        //������
{
    DISPLAY_CLASS = 0xA1, //��ʾ��
    KEYBOARD_CLASS = 0xB1, //������
    STORE_REC_CLASS = 0xC1, //�洢��¼��
    MAGNETIC_CARD_CLASS = 0xD1, //������
    IC_CARD_CLASS = 0xE1, //IC��
    SECURITY_CLASS = 0xF1, //��ȫ��֤
    PIN_PAD_CLASS = 0x1A, //�������
    PRINT_CLASS = 0x1B, //��ӡ
    PBOC_CLASS = 0x1C, //PBOC
    TER_MANAGE_CLASS = 0x1D, //�ն˹���
    DATA_TRANSMIT_CLASS = 0x1E  //����͸��
} CMD_CLASS;


typedef struct {
    u8 *pData;
    u16 len;
} ParamStr;


typedef struct //��������Ľṹ��, ����ID �������
{
    u8 cmdID;

    s32 (*pDealCmdFun)(ST_TRANSFLOW *pstTransFlow);

    //����ΪNULL ��Ҫ���صĿ��Խ����ݴ���� ͨ��pOutStruct����������
    u8 *pReserve;//���� δʹ��
    //bool isInterruptable;
} cmd_Str;


typedef struct {
    u8 cmdClass;    //�������,ȡ�������ֵ
    cmd_Str *pTab;    //���������������,ͨ���������ҵ����崦����
} CmdClassStr;


//��,��һ���ֽڴ�����λ�õ�xy���겻����,�ĵ��Ƿ�������

//#define SCREEN_WIDTH	320//��Ļ���
//#define SCREEN_HIGH		240//��Ļ�߶�
//#define SCREEN_TYPE		0x01//01����,00�ڰ�



typedef enum        //��Ӧ��
{
    ACK_OK = 0x30,        //����ɹ�
    ACK_CMD_NOTSUPPORT,    //ָ���벻֧��
    ACK_PARAM_ERR,        //��������
    ACK_DATALEN_ERR,    //�ɱ������򳤶ȴ���
    ACK_FRAME_ERR,        //֡��ʽ����
    ACK_LRC_FAIL,        //LRC ����ʧ��
    ACK_OTHER_ERR,        //����
    ACK_TIMEOUT,        //��ʱ
    ACK_CURRENTSTATE    //���ص�ǰ״̬
} ECmdAck;
//const struct CmdClassStr cmdClassTab[];


enum {
    SOP_DATA = 0x02, EOP_DATA = 0x03
};//Start of pack, End of pack


#define  E_MAG_OK  0X00 //ˢ��OK
#define  E_MAG_ERR 0X01//ˢ������
#define  E_NEED_ICC 0X02//��ҪIC������
#define  E_MAG_EXIT 0X03//��ҪIC������
#define  E_MAG_ENP   0x04//���ܳ��� 


#endif
