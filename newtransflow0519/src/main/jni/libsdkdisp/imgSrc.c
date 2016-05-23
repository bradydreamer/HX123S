#include "sdkGlobal.h"
#include "iconBitMap.h"

#ifndef  DEBUG_LCD
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


s32 getIconBMPData(u32 bmpId, pMC_BITMAP pBmpOut) {
    FILE *bmpPgFP;
    s32 rslt = 0;

    if (pBmpOut == NULL || bmpId >= ICON_ERROR_FLAG) {
        return SDK_PARA_ERR;
    }
    Trace("bmp", "getIconBMPData: bmpId = %d\r\n", bmpId);
    bmpPgFP = fopen("/mtd1/icon.pck", "rb");

    if (!bmpPgFP) {
        Assert("open file icon.pck file fail\r\n");
        return SDK_ERR;
    }
    fseek(bmpPgFP, bmpId * sizeof(MC_BITMAP), SEEK_SET);
    Trace("bmp", "getIconBMPData: ftell = %d\r\n", ftell(bmpPgFP));
    rslt = fread(pBmpOut, sizeof(MC_BITMAP), 1, bmpPgFP);

    //Trace( "bmp", "getIconBMPData: rslt = %d\r\n", rslt );
    if (rslt != 1) {
        Assert("read MC_BITMAP fail\r\n");
        fclose(bmpPgFP);
        return SDK_ERR;
    }
    //Trace( "bmp", "getIconBMPData: pBmpOut->bmScan = %d\r\n", pBmpOut->bmScan );
    //Trace( "bmp", "getIconBMPData: pBmpOut->bmWidth = %d\r\n", pBmpOut->bmWidth );
    //Trace( "bmp", "getIconBMPData: pBmpOut->bmHeight = %d\r\n", pBmpOut->bmHeight );
    //Trace( "bmp", "getIconBMPData: pBmpOut->bmBitSize = %d\r\n", pBmpOut->bmBitSize );
    //Trace( "bmp", "getIconBMPData: pBmpOut->bmOffset = %d\r\n", pBmpOut->bmOffset );

    pBmpOut->bmBits = (u8 *) sdkGetMem(pBmpOut->bmBitSize);

    if (pBmpOut->bmBits == NULL) {
        if (rslt != 1) {
            Assert(0);
            fclose(bmpPgFP);
            return SDK_ERR;
        }
    }
    fseek(bmpPgFP, pBmpOut->bmOffset, SEEK_SET);
    rslt = fread(pBmpOut->bmBits, pBmpOut->bmBitSize, 1, bmpPgFP);

    //Trace( "bmp", "getIconBMPData: rslt = %d\r\n", rslt );
    if (rslt != 1) {
        Assert(0);
        sdkFreeMem(pBmpOut->bmBits);
        fclose(bmpPgFP);
        return SDK_ERR;
    }
    fclose(bmpPgFP);
    return SDK_OK;
}

