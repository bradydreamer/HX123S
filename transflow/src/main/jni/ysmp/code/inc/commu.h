#ifndef AFX_COMMU_H
#define AFX_COMMU_H


extern bool JudgeCommuDevIsok(void);

extern s32 AppCommSendData(const u8 *pSendData, const u32 iSendDataLen, const u32 iTimerOver);

extern s32 AppCommRecvData(const u8 *pRecvData, const u32 iRecvDataLen, const u32 iTimerOver);

extern s32 AppCommPollQuery(const u8 *pQueryData);

extern s32 AppGetCommuStatus(void);

extern s32 SetBtName(void);

extern s32 SetBtPwd(void);

extern u8 SysetDevice(SDK_MSG * lpMsg);

extern void AppCommPollQueryAndExe(void);

extern s32 AppCommMakeBagAndSend(ST_TRANSFLOW * Transflow);

//20150402 �������ݳ�ʱʱ��궨�壬�ݶ�Ϊ500ms
#define SENDDATA_TIMEOUT        500                    //���ͳ�ʱʱ��

#endif
