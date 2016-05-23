#ifndef MPOS_IC_CARD_CMD_H
#define MPOS_IC_CARD_CMD_H


typedef enum {
    IC_CHECK = 0x01,        //IC�����
    IC_SET_TYPE,            //����IC������
    IC_POWER_ON,            //IC���ϵ�
    IC_POWER_OFF,            //IC���µ�
    IC_COMMUN,                //IC��ͨѶ
    IC_CMD_END = 0xFF
} IC_CMD;


s32 OnIccCheck(ST_TRANSFLOW * pstTransFlow);
s32 OnIccSetType(ST_TRANSFLOW * pstTransFlow);
s32 OnIccPowerOn(ST_TRANSFLOW * pstTransFlow);
s32 OnIccPowerOff(ST_TRANSFLOW * pstTransFlow);
s32 OnIccCommum(ST_TRANSFLOW * pstTransFlow);

#endif