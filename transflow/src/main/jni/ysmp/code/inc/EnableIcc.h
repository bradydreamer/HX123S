#ifndef ENABLEICC_H_
#define ENABLEICC_H_

//�������� ��֯55��IC�����ݵ�ʱ��Ҫ��
#define SALE_MSG        0x01    //���ѱ���
#define QUERY_MSG            0x02    //��ѯ����
#define PREAUTH_MSG        0x03    //Ԥ��Ȩ����
#define OFFLINE_SALE_MSG 0x04    //�ѻ����ѱ���
#define SCRIPT_NOTICE_RLT   0x05    //IC�����׽ű�֪ͨ����������
#define REVERSAL_MSG        0x06    //�������ױ���
#define SPECIALTC_MSG       0x07    //�������������TC
#define ECUNLOAD_CASH_MSG       0x08    //EC�ֽ��ֵ�������� //wzl_unioupay_two 20110512

#define CAPKFILE    1
#define NORMALTRADE 2
#define SYSINIFILE  3

#define ICCONLINEPIN                            0                                       //PIN for IC card request online 
#define ICCOFFLINEPIN                           1                                       //PIN for IC card request offline

#define OK                    0
#define ERR                1

extern void UnionMangeMenu(void);

extern u8 ZipPagData(u8 type, u8 *data, u16 *datalen);

extern s32 InputCreditPwd(const u8 *pTradeAmount, u8 PINTryCount, u8 IccEncryptWay, u8 *pCreditPwd);

extern void InitPadRfPara(u8 type);

extern void DispInputCreditPwd(u8 PINTryCount, u8 IccEncryptWay);

extern u8 GetInterCode(u8 * InterCode, u8 * rid);

extern void UnionMangeQueryCapk(); //������������ʾ��Կzjz20091207
extern void EmvSelAid(u8 enSet);

extern void DispCapk(u8 index, u8 isDown);

#endif

