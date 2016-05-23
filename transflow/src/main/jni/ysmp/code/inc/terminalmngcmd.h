#ifndef MPOS_TERMINAL_MANAGE_CMD_H
#define MPOS_TERMINAL_MANAGE_CMD_H

//�ն˹���
extern const cmd_Str TsCmdTab[];
#define MPOS_APP_FILE        "/mtd0/K21App.bin"
#define MPOS_TMP_APP_FILE    "/mtd0/temp.bin"
#define MPOS_APP_UPDATEFLG        "/mtd0/updata.flg"


#define START_DOWNLOAD    0x01
#define DOWNLOADING        0x02
#define END_DOWNLOAD    0x03


typedef enum {
    TM_SET_BEEP = 0x01,        //����������
    TM_SET_LED,            //ָʾ����˸ָ��
    TM_SET_DATETIME = 0x04, //����ʱ������
    TM_GET_DATETIME,        //��ȡʱ������
    TM_SET_TER_PARAM,        //�����ն˲���
    TM_GET_TER_PARAM,        //��ȡ�ն˲���
    TM_CANCEL_OPER,        //ȡ��/��λ����
    TM_UPDATE,                //�����̼�/Ӧ��
    TM_ECHO,                //����
    TM_CTRL_DEV,            //�����豸
    TM_GET_STATUS_DEV,        //��ȡ�豸״̬
    TM_CMD_END = 0xFF
} TM_CMD;

typedef struct MposTermPara {
    char merchantNo[16];
    char terminalNo[9];
    char merchantName[21];
} stMposTermpara;

//���ڵ����ĺ궨��
#define BATTERY_ERR        -2
#define NO_POWER        -1
#define LOW_POWER        0
#define ONE_BAR            1
#define TWO_BAR            2
#define THREE_BAR        3
#define FULL_POWER        99

#define NEED_RESET        true
#define NO_NEED_RESET   false


#define DEV_SLEEP        true
#define MPOS_PARA_FILE "/mtd0/MposTermPara.conf"


extern s32 sdkDealPowerOff();

extern s32 OnSetBeep(ST_TRANSFLOW * pstTransFlow);
extern s32 OnSetLED(ST_TRANSFLOW * pstTransFlow);
extern s32 OnSetDateTime(ST_TRANSFLOW * pstTransFlow);
extern s32 OnGetDateTime(ST_TRANSFLOW * pstTransFlow);
extern s32 OnSetTerParam(ST_TRANSFLOW * pstTransFlow);
extern s32 OnGetTerParam(ST_TRANSFLOW * pstTransFlow);
extern s32 OnCancelOper(ST_TRANSFLOW * pstTransFlow);
extern s32 OnUpdate(ST_TRANSFLOW * pstTransFlow);
extern s32 OnEcho(ST_TRANSFLOW * pstTransFlow);
extern s32 OnCtrlDev(ST_TRANSFLOW * pstTransFlow);
extern s32 OnGetDevStatus(ST_TRANSFLOW * pstTransFlow);

#define NEWCANCELCMD_MODE           1        //20150402 ���µĸ�λ��ʽ 

#endif
