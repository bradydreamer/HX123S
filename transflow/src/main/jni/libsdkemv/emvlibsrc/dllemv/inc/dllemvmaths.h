#ifndef EMVMATH_H
#define  EMVMATH_H

#define ENCRYPT 1
#define DECRYPT 0


void EMVIntToByteArray(unsigned long var, unsigned char *buf, unsigned char bufLen);

unsigned char EMVCalcBcc(unsigned char *src, unsigned int num);

unsigned long EMVByteArrayToInt(unsigned char *buf, unsigned char bufLen);

void EMVBcdToU32(unsigned int *Dest, unsigned char *Bcd, unsigned char Len);

void EMVU32ToBcd(unsigned char *Bcd, unsigned int const Src, unsigned char Len);

void EMVU16ToBcd(unsigned char *Bcd, unsigned short const Src, unsigned char Len);

void GetLibEmvVersion(unsigned char *kernelVer);

unsigned char EMVIsAscii(unsigned char *Src);

void EMVAscToBcd(unsigned char *Dest, unsigned char *Src, unsigned short Len);

unsigned char EMVasc_to_byte(unsigned char *asc);

void EMVBcdToAsc(unsigned char *Dest, unsigned char *Src, unsigned short Len);

void EMVBcdToU16(unsigned short *Dest, unsigned char *Bcd, unsigned char Len);

void EMVbcd_to_asc(unsigned char *asc, unsigned char *bcd, unsigned int asc_len);

unsigned char ParseExtLen(unsigned char *ParseData, unsigned short *index, unsigned short *DataLen);

void Emv_srand(unsigned short rand);

unsigned short Emv_rand(void);

void Emv_PIN_asc_to_bcd(unsigned char *bcd, unsigned char *asc, unsigned char asc_len);
void Hash(unsigned char* bb,unsigned long len,unsigned char* Message_Digest);
unsigned char RSARecover(unsigned char *m, unsigned int mLen, unsigned char *e, unsigned int eLen,unsigned char *input,unsigned char *output);

extern void Packbertlvdata(unsigned char *dstdata,unsigned int *dstdatalen,unsigned char *tag,unsigned char taglen,unsigned char *srcdata,unsigned int srcdatalen);


#endif

