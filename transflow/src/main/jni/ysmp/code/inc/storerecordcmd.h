#ifndef MPOS_STORE_RECORD_CMD_H
#define MPOS_STORE_RECORD_CMD_H

//�洢��¼

#define  FileNamePrefix        "/mtd0/mpos/rec_"
#define  RECORD_HEAD_SIZE    50//�洢��¼����Ϣͷ �������ļ�ǰ50byte��
#define  SKIP_LEN            2 //����ƫ���Ƿ�����LL�����ֽ�,�����ֵΪ2,����Ϊ0


typedef enum {
    STORE_INIT_REC = 0x01,        //��ʼ���洢��¼
    STORE_GET_RECCOUNT,            //��ȡ�洢��¼��
    STORE_ADD_REC,                //���Ӵ洢��¼
    STORE_UPDATE_REC,            //���´洢��¼
    STORE_GET_REC,                //��ȡ�洢��¼
    STORE_CMD_END = 0xFF
} Store_CMD;


extern s32 OnInitRecord(ST_TRANSFLOW * pstTransFlow);
extern s32 OnGetRecCount(ST_TRANSFLOW * pstTransFlow);
extern s32 OnAddRecord(ST_TRANSFLOW * pstTransFlow);
extern s32 OnUpdateRecord(ST_TRANSFLOW * pstTransFlow);
extern s32 OnGetRecord(ST_TRANSFLOW * pstTransFlow);


#endif