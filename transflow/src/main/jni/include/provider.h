#include <jni.h>

int ProviderGetTagValueLength(char* tag);
int ProviderGetTagValue(char* tag, unsigned char* value);
int ProviderSetTagValue(char* tag, unsigned char* value, int len);
int ProviderUpdateValueToDb();
int ProviderDeleteTradeRecord();
int ProviderDeleteTradeOneRecord(char *voucher);
int ProviderUpdateRecordOneTag(char *voucher, char* tag, unsigned char* value, int len);
int ProviderSetRecordToTagList(int index);
int ProviderGetRecordTotal();



int ProviderCalcMac(unsigned char *heData, int dataLen, unsigned char *heMac, int *pMacLen);
int ProviderUpdateWk(unsigned char *heWkData, int dataLen);
int ProviderSecAuth(char *asRespCode, char *asAuthCode, unsigned char *heRecvF55, unsigned int uiRecvF55Len, unsigned char *pOutData, int *pOutLen);
//int ProviderESign(char *asData, char *asPath);
int ProviderPrintScript(char *asPrintData);
int ProviderSetEmvTagValue(unsigned char *tag, unsigned char *value, int len);
int ProviderGetEmvTagValue(unsigned char *tag, int pathID, unsigned char *value);
int ProviderTrackDataEncrypt(char *trackData, unsigned char *phEncryptData, int *pOutLen);





