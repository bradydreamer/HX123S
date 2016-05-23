#ifndef _DOWNLOADPMK_H_
#define _DOWNLOADPMK_H_

extern s32 LoadPMKStart(u8 *indata,u32 len,u8 *outdata,u32 *rlen);
extern s32 LoadPMKParse8583(u8 *indata,u32 len,u8 * outdata,u32 rlen);

#endif