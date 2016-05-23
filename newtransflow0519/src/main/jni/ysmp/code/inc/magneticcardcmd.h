#ifndef MPOS_MAGNETIC_CARD_CMD_H
#define MPOS_MAGNETIC_CARD_CMD_H


//������

typedef enum {
    MC_OPEN_READER = 0x01,    //����������
    MC_CLOSE_READER,        //�رն�����
    MC_READ_CARD_P = 0x04,    //�������������ģ�
    MC_READ_CARD_C,            //�������������ģ�
    MC_READ_CARD_SAVE,        //�����������ڲ����棩
    MC_READ_CARD_AUDIO,        //ˢ������������������Ƶ�ӿڣ�
    MC_CMD_END = 0xFF
} MAGNETIC_CARD_CMD;


#endif