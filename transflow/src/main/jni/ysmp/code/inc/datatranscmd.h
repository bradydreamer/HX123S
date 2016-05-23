#ifndef _DATA_TRANSFER_CMD_H_
#define _DATA_TRANSFER_CMD_H_


typedef enum {
    DATA_TRANSMIT = 0x01,        //���ݴ���
    DATA_CMD_END = 0xFF
} DATA_CMD;


extern s32 OnDataTransmit(ST_TRANSFLOW * pstTransFlow);


#endif
