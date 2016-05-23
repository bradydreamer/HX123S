/********************************Copyright ( c ) ***************************
**                  Shenzhen Xinguodu Technology Co., LTD.
**                        http://www.xinguodu.com
**
**
**Created By:     ��ѧ��
**Created Date:       2013-4-15
**Version:            <version >
**Description:  508 APP�ӿ�ʵ��  
**
****************************************************************************/
#ifdef SPI_DDI
#include "devSPIApi.h"
#else

#include "devapi.h"

#endif

#include "javaInterface.h"
#include <stdio.h>
#include <stdarg.h>//sujianzhong 2015.05.20 17:28
#include <unistd.h>//sujianzhong 2015.06.05 8:56
#include<fcntl.h>
#include <time.h>


JAVA_INTERFACE *javaInterface;


#include <android/log.h>

#define  LOG_TAG    "android-ddi"
#ifdef XGD_SDK_DEBUG
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#define  LOGD(...)
#define  LOGE(...)
#define  LOGI(...)
#endif
#define DDI_SHOW_FUN_NAME LOGD((u8*)__FUNCTION__);


/*
// C prototype : void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - ���������
//	 [IN] pbSrc - �ַ���
//	 [IN] nLen - 16���������ֽ���(�ַ����ĳ���/2)
// return value:
// remarks : ���ַ���ת��Ϊ16������
*/
void StrToHex(u8 *pbDest, u8 *pbSrc, int nLen) {
    char h1, h2;
    u8 s1, s2;
    int i;

    for (i = 0; i < nLen; i++) {
        h1 = pbSrc[2 * i];
        h2 = pbSrc[2 * i + 1];

        s1 = toupper(h1) - 0x30;
        if (s1 > 9)
            s1 -= 7;

        s2 = toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;

        pbDest[i] = s1 * 16 + s2;
    }
}


/*
// C prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - ���Ŀ���ַ���
//	 [IN] pbSrc - ����16����������ʼ��ַ
//	 [IN] nLen - 16���������ֽ���
// return value:
// remarks : ��16������ת��Ϊ�ַ���
*/
void HexToStr(u8 *pbDest, u8 *pbSrc, int nLen) {
    char ddl, ddh;
    int i;

    for (i = 0; i < nLen; i++) {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}

// DDI �ӿ�����
//ϵͳ
void U16ToBcd(unsigned char *Bcd, unsigned short const Src, unsigned int Len) {
    unsigned int i;
    unsigned short num = Src;
    for (i = Len; i > 0; i--) {
        *(Bcd + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }
}

s32 ddi_sys_msleep(u32 nMs)//����OK
{
    DDI_SHOW_FUN_NAME;

    return -1;//CoreApi_ddi_sys_msleep(nMs);

    //return -1;
}

s32 ddi_sys_set_time(const u8 *lpTime)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_sys_set_time(lpTime);
    //return -1;
}

s32 ddi_sys_get_time(u8 *lpTime)//����OK
{
    DDI_SHOW_FUN_NAME;
    //zhouxiaoxin
    u8 year, adjust, temp[16] = {0}, ret;
    u16 yearsaved = 0;
    time_t sec;
    struct tm *pTm;

    sec = time(NULL);
    pTm = localtime(&sec);
    year = pTm->tm_year;
    year = year % 100;

        U16ToBcd(&lpTime[0], year, 1);
        U16ToBcd(&lpTime[1], pTm->tm_mon + 1, 1);
        U16ToBcd(&lpTime[2], pTm->tm_mday, 1);
        U16ToBcd(&lpTime[3], pTm->tm_hour, 1);
        U16ToBcd(&lpTime[4], pTm->tm_min, 1);
        U16ToBcd(&lpTime[5], pTm->tm_sec, 1);

    return 1;//CoreApi_ddi_sys_get_time(lpTime);
    //return -1;
}

s32 ddi_sys_get_tick(u32 *nTick)  //ϵͳ�δ�//����OK
{
    DDI_SHOW_FUN_NAME;
//    struct timeval t0;
//    gettimeofday(&t0, 0);
//    *nTick = t0.tv_usec;
//    return 0;
    //zhouxiaoxin
    struct timeval t0;
    gettimeofday(&t0, NULL);
    *nTick = t0.tv_sec * 1000 + t0.tv_usec / 1000;
    return DDI_OK;
}

s32 ddi_sys_read_dsn(u8 *lpOut)//ok
{
    DDI_SHOW_FUN_NAME;
//#ifdef SPI_DDI
//	return spi_ddi_sys_read_dsn(lpOut);
//#endif
    return -1;//CoreApi_ddi_sys_read_dsn(lpOut);
}

s32 ddi_sys_sleep(s32 status)//ok
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_sys_sleep(status);
}

s32 ddi_sys_get_firmwarever(u8 *lpOut, u32 nType)//ok
{
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return ddi_sys_get_firmwarever(lpOut,nType);
#endif
    return -1;//CoreApi_ddi_sys_get_firmwarever (lpOut, nType);
}

s32 ddi_sys_bat_status(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_sys_bat_status();    
}

s32 ddi_sys_poweroff(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_sys_poweroff(); 
}

void *ddi_k_malloc(unsigned nbytes) {
//	DDI_SHOW_FUN_NAME;
    return (void *) malloc(nbytes);
}

void ddi_k_free(void *ap) {
//	DDI_SHOW_FUN_NAME;
    return free(ap);
}

s32 ddi_sys_sleep_with_alarm(s32 sec, s32 type, s32 status) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_sys_sleep_with_alarm(sec, type, status);     
}

s32 ddi_sys_reboot(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_sys_reboot(); 
}

s32 ddi_sys_download(u32 type) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_sys_download( type ); 
}

//����
s32 ddi_com_open(u32 nCom, const strComAttr *lpstrAttr) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_com_open(nCom, lpstrAttr);
    //return -1;
}

s32 ddi_com_close(u32 nCom) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_com_close(nCom);
    //return -1;
}

s32 ddi_com_clear(u32 nCom) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_com_clear(nCom);
    //return -1;
}

s32 ddi_com_read(u32 nCom, u8 *lpOut, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_com_read(nCom, lpOut, nLe);
    //return -1;
}

s32 ddi_com_write(u32 nCom, u8 *lpIn, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_com_write(nCom, lpIn, nLe );
    //return -1;
}

s32 ddi_com_ioctl(u32 nCom, u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_com_ioctl(nCom, nCmd, lParam, wParam);
    //return -1;
}

//�ſ�
s32 ddi_mag_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_mag_open();
    //return -1;
}

s32 ddi_mag_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_mag_close();
    //return -1;
}

s32 ddi_mag_clear(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_mag_clear();
    //return -1;
}

s32 ddi_mag_read(u8 *lpTrack1, u8 *lpTrack2, u8 *lpTrack3) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_mag_read (lpTrack1, lpTrack2, lpTrack3);
    //return -1;
}

s32 ddi_mag_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_mag_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//IC PSAM
s32 ddi_iccpsam_open(u32 nSlot) {
    int ret = 0;
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI

    LOGD("call spi_ddi_iccpsam_open nSlot = %d",nSlot);
    ret = spi_ddi_iccpsam_open(nSlot);
    LOGD("call spi_ddi_iccpsam_open ret = %d",ret);
    return ret;
#endif
/*
    jstring result = (*(javaInterface->myEnv))->CallObjectMethod(javaInterface->myEnv,
                                                                 javaInterface->gJniObj,
                                                                 javaInterface->jniCheckCard);//��java�ӿڣ���Ļ��ʾ
    char *key = (*(javaInterface->myEnv))->GetStringUTFChars((javaInterface->myEnv), result, NULL);
    LOGD ("ddi_iccpsam_open key %s", key);

    switch (*key) {
        case '1': {
            LOGD ("ddi_iccpsam_open key==%d", 1);
            (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                             key);//�ͷ���Դ
            return 1;
        }
        default:
            (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                             key);//�ͷ���Դ
            return -1;
    }
    */
    return -1;
}

s32 ddi_iccpsam_close(u32 nSlot) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_iccpsam_close(nSlot);
#endif
    return -1;//CoreApi_ddi_iccpsam_close(nSlot);
    //return -1;
}

s32 ddi_iccpsam_poweron(u32 nSlot, u8 *lpAtr) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_iccpsam_poweron(nSlot, lpAtr);
#endif
/*
    jstring result = (*(javaInterface->myEnv))->CallObjectMethod(javaInterface->myEnv,
                                                                 javaInterface->gJniObj,
                                                                 javaInterface->jniPowerOnIC);//��java�ӿڣ���Ļ��ʾ
    char *key = (*(javaInterface->myEnv))->GetStringUTFChars((javaInterface->myEnv), result, NULL);
    LOGD ("ddi_iccpsam_poweron key %s", key);
    //zhouxiaoxin
    if (!memcmp(key, "-1", 2)) {
        (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                         key);//�ͷ���Դ
        return -1;
    }
    StrToHex(lpAtr, key, strlen(key) / 2);

    (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                     key);//�ͷ���Դ
    return DDI_OK;
    */
    return -1;
}

s32 ddi_iccpsam_poweroff(u32 nSlot) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_iccpsam_poweroff(nSlot);
#endif
    return -1;
}

s32 ddi_iccpsam_get_status(u32 nSlot) {
    s32 ret = 0;
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    LOGD("spi_ddi_iccpsam_get_status nSlot = %d",nSlot);
    ret = spi_ddi_iccpsam_get_status(nSlot);
    LOGD("spi_ddi_iccpsam_get_status ret=%d",ret);
    return ret;
#endif
/*
    jstring result = (*(javaInterface->myEnv))->CallObjectMethod(javaInterface->myEnv,
                                                                 javaInterface->gJniObj,
                                                                 javaInterface->jniCheckIC);//��java�ӿڣ���Ļ��ʾ
    char *key = (*(javaInterface->myEnv))->GetStringUTFChars((javaInterface->myEnv), result, NULL);
    LOGD ("ddi_iccpsam_get_status key %s", key);
    switch (*key) {
        case '1': {
            LOGD ("ddi_iccpsam_get_status key1==%d", 1);
            (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                             key);//�ͷ���Դ
            return 2;
        }
        default:
            (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                             key);//�ͷ���Դ
            return -1;
    }
    */
    return -1;
}


s32 ddi_iccpsam_exchange_apdu(u32 nSlot, const u8 *lpCApdu, u32 nCApduLen, u8 *lpRApdu,
                              u32 *lpRApduLen, u32 nRApduSize) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_iccpsam_exchange_apdu (nSlot, lpCApdu, nCApduLen, lpRApdu, lpRApduLen, nRApduSize);
#endif
    LOGD ("ddi_iccpsam_exchange_apdu nCApduLen:%d, nSlot:%d", nCApduLen, nSlot);
    //int i;
    u8 temp[128] = {0};
    HexToStr(temp, lpCApdu, nCApduLen);
    LOGD ("ddi_iccpsam_exchange_apdu temp:%s", temp);

    jstring result = (*(javaInterface->myEnv))->CallObjectMethod(javaInterface->myEnv,
                                                                 javaInterface->gJniObj,
                                                                 javaInterface->jniApduIC,
                                                                 (*(javaInterface->myEnv))->NewStringUTF(
                                                                         (javaInterface->myEnv),
                                                                         temp));//��java�ӿ�
    u8 *key = (*(javaInterface->myEnv))->GetStringUTFChars((javaInterface->myEnv), result, NULL);
    LOGD ("ddi_iccpsam_exchange_apdu result:%s", key);
    //zhouxiaoxin
    if (!memcmp(key, "-1", 2)) {
        (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                         key);//�ͷ���Դ
        return -1;
    }

    *lpRApduLen = strlen(key) / 2;
    LOGD ("ddi_iccpsam_exchange_apdu lpRApduLen:%d", *lpRApduLen);

    StrToHex(lpRApdu, key, *lpRApduLen);

    (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,
                                                     key);//�ͷ���Դ
    return DDI_OK;
}

s32 ddi_iccpsam_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_iccpsam_ioctl(nCmd, lParam, wParam);
#endif
    return -1;//CoreApi_ddi_iccpsam_ioctl(nCmd, lParam, wParam);
}


//�ǽӿ�����
s32 ddi_rf_open(void) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_open();
#endif
    return -1;
}

s32 ddi_rf_close(void) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_close();
#endif
    return -1;
}

s32 ddi_rf_poweron(u32 nType) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_poweron(nType);
#endif
    return -1;
}

s32 ddi_rf_poweroff(void) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_poweroff();
#endif
    return -1;
}

s32 ddi_rf_get_status(void) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_get_status();

#endif
    return -1;

}

s32 ddi_rf_activate(void) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_activate();

#endif
    return -1;
}

s32 ddi_rf_exchange_apdu(const u8 *lpCApdu, u32 nCApduLen, u8 *lpRApdu, u32 *lpRApduLen,
                         u32 nRApduSize) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_exchange_apdu(lpCApdu, nCApduLen, lpRApdu, lpRApduLen, nRApduSize);

#endif

    return -1;
}

s32 ddi_rf_remove(void) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_remove();

#endif

    return -1;
}

s32 ddi_rf_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
#ifdef SPI_DDI
    return spi_ddi_rf_ioctl(nCmd, lParam, wParam);
#endif

    return -1;
}

//������ӡ
s32 ddi_thmprn_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_open();
    //return -1;
}

s32 ddi_thmprn_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_close();
    //return -1;
}

s32 ddi_thmprn_feed_paper(u32 nPixels) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_feed_paper(nPixels);
    //return -1;
}

s32 ddi_thmprn_print_image(u32 nOrgLeft, u32 nImageWidth, u32 nImageHeight, const u8 *lpImage) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_print_image(nOrgLeft, nImageWidth, nImageHeight, lpImage);
    //return -1;
}

s32 ddi_thmprn_print_image_file(u32 nOrgLeft, u32 nImageWidth, u32 nImageHeight,
                                const u8 *lpImageName) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_print_image_file(nOrgLeft, nImageWidth, nImageHeight, lpImageName);
    //return -1;
}

s32 ddi_thmprn_print_text(strPrnTextCtrl *lpPrnCtrl, const u8 *lpText) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_print_text (lpPrnCtrl, lpText );
    //return -1;
}

s32 ddi_thmprn_print_comb_text(u32 nNum, const strPrnCombTextCtrl *lpPrnCombTextCtrl[]) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_print_comb_text (nNum,  lpPrnCombTextCtrl );
    //return -1;
}

s32 ddi_thmprn_get_status(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_get_status ();
    //return -1;
}

s32 ddi_thmprn_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_thmprn_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//����
s32 ddi_key_open(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_key_open ();
    //return -1;
}

s32 ddi_key_close(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_key_close ();
    //return -1;
}

s32 ddi_key_clear(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_key_clear ();
    //return -1;
}

s32 ddi_key_read(u32 *lpKey)//����OK
{
    DDI_SHOW_FUN_NAME;
    return 0;//CoreApi_ddi_key_read (lpKey);
    //return -1;
}

s32 ddi_key_ioctl(u32 nCmd, u32 lParam, u32 wParam)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_key_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//��ʾ
s32 ddi_lcd_open(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_open ();
    //return -1;
}

s32 ddi_lcd_close(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_close ();
    //return -1;
}

s32 ddi_lcd_fill_rect(const strRect *lpstrRect, u32 nRGB)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_fill_rect ( lpstrRect,  nRGB);
    //return -1;
}

s32 ddi_lcd_clear_rect(const strRect *lpstrRect)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_clear_rect ( lpstrRect);
    //return -1;
}

s32 ddi_lcd_show_text(u32 nX, u32 nY, const u8 *lpText)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_show_text ( nX, nY, lpText);
    //return -1;
}

s32 ddi_lcd_show_picture(const strRect *lpstrRect, const strPicture *lpstrPic) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_show_picture( lpstrRect,   lpstrPic);
    //return -1;
}

s32 ddi_lcd_show_picture_file(const strRect *lpstrRect, const u8 *lpBmpName)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_show_picture_file( lpstrRect,  lpBmpName);
    //return -1;
}

s32 ddi_lcd_show_pixel(u32 nX, u32 nY)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_show_pixel ( nX,  nY);
    //return -1;
}

s32 ddi_lcd_show_line(const strLine *lpstrLine)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_show_line(lpstrLine );
    //return -1;
}

s32 ddi_lcd_show_rect(const strRect *lpstrRect)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_show_rect(lpstrRect);
    //return -1;
}

s32 ddi_lcd_extract_rect(const strRect *lpstrRect, strPicture *lpstrPic) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_extract_rect(lpstrRect, lpstrPic);
    // return -1;
}

s32 ddi_lcd_ioctl(u32 nCmd, u32 lParam, u32 wParam)//���ֲ���OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_lcd_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//��Ƶ
s32 ddi_audio_open(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audio_open ();
    //return -1;
}

s32 ddi_audio_close(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audio_close ();
    //return -1;
}

s32 ddi_audio_play(const u8 *lpFile)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audio_play (lpFile);
    //return -1;
}

s32 ddi_audio_stop(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audio_stop ();
    //return -1;
}

s32 ddi_audio_pause(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audio_pause ();
    //return -1;
}

s32 ddi_audio_replay(void)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audio_replay ();
    //return -1;
}

s32 ddi_audio_ioctl(u32 nCmd, u32 lParam, u32 wParam)//����OK
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audio_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//�������豸
s32 ddi_tp_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_tp_open ();
    //return -1;
}

s32 ddi_tp_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_tp_close ();
    //return -1;
}

s32 ddi_tp_read_pixels(strTpPixels *lpTpPixels) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_tp_read_pixels(lpTpPixels);
    //return -1;
}

s32 ddi_tp_clear(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_tp_clear ();
    //return -1;
}

s32 ddi_tp_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_tp_ioctl( nCmd, lParam,  wParam);
    //return -1;
}

//����ͨ��
s32 ddi_wireless_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_open ();
    //return -1;
}

s32 ddi_wireless_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_close();
    //return -1;
}

s32 ddi_wireless_destroy(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_destroy();
    //return -1;
}

s32 ddi_wireless_dial(const u8 *lpApn, const u8 *lpUser, const u8 *lpPwd) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_dial (lpApn, lpUser, lpPwd);
    //return -1;
}

s32 ddi_wireless_hangup(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_hangup ();
    //return -1;
}

s32 ddi_wireless_get_status(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_get_status ();
    //return -1;
}

s32 ddi_wireless_sleep(void)//�ڲ�����ʹ��
{
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_sleep ();
    //return -1;
}

s32 ddi_wireless_wakeup(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_wakeup ();
    //return -1;
}

s32 ddi_wireless_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_wireless_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//modem
s32 ddi_modem_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_open ();
    //return -1;
}

s32 ddi_modem_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_close();
    //return -1;
}

s32 ddi_modem_dial(const strModemDialPara *lpstrDialPara) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_dial (lpstrDialPara);
    //return -1;
}

s32 ddi_modem_hangup(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_hangup ();
    //return -1;
}

s32 ddi_modem_get_status(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_get_status ();
    //return -1;
}

s32 ddi_modem_read(u8 *lpOut, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_read (lpOut,  nLe );
    //return -1;
}

s32 ddi_modem_write(u8 *lpIn, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_write (lpIn, nLe );
    //return -1;
}

s32 ddi_modem_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_modem_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//������Կ
s32 ddi_innerkey_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_innerkey_open ();
    //return -1;
}

s32 ddi_innerkey_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_innerkey_close();
    //return -1;
}

s32 ddi_innerkey_inject(u32 nKeyArea, u32 nIndex, const u8 *lpKeyData) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_innerkey_inject (nKeyArea, nIndex,lpKeyData);
    //return -1;
}

s32 ddi_innerkey_encrypt(u32 nKeyArea, u32 nIndex, u32 nLen, const u8 *lpIn, u8 *lpOut) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_innerkey_encrypt(nKeyArea, nIndex, nLen, lpIn, lpOut);
    //return -1;
}

s32 ddi_innerkey_decrypt(u32 nKeyArea, u32 nIndex, u32 nLen, const u8 *lpIn, u8 *lpOut) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_innerkey_decrypt(nKeyArea, nIndex, nLen, lpIn, lpOut);
    //return -1;
}

s32 ddi_innerkey_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_innerkey_ioctl( nCmd, lParam, wParam);
    //return -1;
}


//�����ļ�ϵͳ
s32 ddi_vfs_open(const u8 *lpPath, const u8 *lpMode)//����OK
{
    DDI_SHOW_FUN_NAME;
    s32 i = 0;
    FILE *fp = NULL;
    char temp[128] = {0};
    char *p = NULL;
    LOGD("%s lpPath = %s", __FUNCTION__, lpPath);
    //zhouxiaoxin 之前 sdk 层调的 xgd 接口，与ddi 相同的函数是有带自建文件夹的，这里也补下
    if (access(lpPath, F_OK) != 0) {
        LOGD("file is not exist");
//        for (i = strlen(lpPath); i > 0; i--)
//        {
//            if (lpPath[i - 1] == '/')
//            {
//                memset(temp, 0, sizeof(temp));
//                strncat(temp, lpPath, i - 1);
//                LOGD("%s",temp);
//                mkdir(temp, 0755);
//                fp = fopen(lpPath, "wb+");
//                fclose(fp);
//                break;
//            }
//        }
        i = 0;
        while (1) {
            p = strchr(lpPath + i, '//');
            if (p == NULL) {
                break;
            }
            i = (s32)p - (s32)lpPath;
            i++;
            memset(temp, 0, sizeof(temp));
            memcpy(temp, lpPath, i);
            LOGD("%s",temp);
            if (access(temp, F_OK) != 0) {
                mkdir(temp, 0755);
            }
        }
        fp = fopen(lpPath, "wb+");
        if(fp != NULL) {
            fclose(fp);
        }
    }
    return fopen(lpPath, lpMode);//sujianzhong 2015.05.21 11:27
//    return -1;//CoreApi_ddi_vfs_open (lpPath, lpMode);
    //return -1;
}

s32 ddi_vfs_close(s32 nHandle)//����OK
{
    DDI_SHOW_FUN_NAME;
    return fclose((FILE *) nHandle);//sujianzhong 2015.05.21 11:29
//    return -1;//CoreApi_ddi_vfs_close (nHandle);
    //return -1;
}

s32 ddi_vfs_write(const u8 *lpData, s32 nLen, s32 nHandle)//����OK
{
    s32 ret = 0;
    DDI_SHOW_FUN_NAME;
    //zhouxiaoxin
    ret = fwrite(lpData, nLen, sizeof(unsigned char), (FILE *) nHandle);//sujianzhong 2015.05.21 11:29
    if (ret == 1) {
        return nLen;
    } else {
        return ret;
    }
//    return -1;//CoreApi_ddi_vfs_write (lpData , nLen, nHandle);
    //return -1;
}

s32 ddi_vfs_read(u8 *lpData, s32 nLen, s32 nHandle)//����OK
{
    DDI_SHOW_FUN_NAME;
    //zhouxiaoxin
    int len = 0;
    len = fread(lpData, nLen, sizeof(unsigned char), (FILE *) nHandle);//sujianzhong 2015.05.21 11:34
    if (len == 1) {
        return nLen;
    } else {
        return len;
    }
    return -1;//CoreApi_ddi_vfs_read (lpData, nLen, nHandle);
    //return -1;
}

s32 ddi_vfs_seek(s32 nHandle, s32 nOffset, s32 nWhence)//����OK
{
    DDI_SHOW_FUN_NAME;
    return fseek((FILE *) nHandle, nOffset, nWhence);//sujianzhong 2015.05.21 11:34
    return -1;//CoreApi_ddi_vfs_seek(nHandle, nOffset, nWhence);
    //return -1;
}

s32 ddi_vfs_tell(s32 nHandle)//����OK
{
    DDI_SHOW_FUN_NAME;
    return ftell((FILE *) nHandle);
//    return -1;//CoreApi_ddi_vfs_tell (nHandle);
    //return -1;
}

s32 ddi_vfs_remove(const u8 *lpFileName)//����OK
{
    DDI_SHOW_FUN_NAME;
    return remove(lpFileName);//sujianzhong 2015.05.21 11:36
    return -1;//CoreApi_ddi_vfs_remove (lpFileName);
    //return -1;
}

s32 ddi_vfs_rename(const u8 *lpOldName, const u8 *lpNewName) {
    DDI_SHOW_FUN_NAME;
    return rename(lpOldName, lpNewName);//sujianzhong 2015.05.21 11:38
    return -1;//CoreApi_ddi_vfs_rename(lpOldName, lpNewName);
    //return -1;
}

s32 ddi_vfs_access(const u8 *lpName)//����OK
{
    DDI_SHOW_FUN_NAME;
    if (access(lpName, F_OK) == 0) return DDI_OK;//sujianzhong 2015.05.21 11:38

    return -1;//CoreApi_ddi_vfs_access( lpName);
    //return -1;
}

s32 ddi_vfs_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_vfs_ioctl(nCmd, lParam, wParam);
    //return -1;
}
//����

s32 ddi_bt_open(void) {
    DDI_SHOW_FUN_NAME;

    return -1;//CoreApi_ddi_bt_open();
    //return -1;
}

s32 ddi_bt_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_bt_close();
    //return -1;
}

s32 ddi_bt_disconnect(const u8 *lpMac) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_bt_disconnect(lpMac);
    //return -1;
}

s32 ddi_bt_write(u8 *lpIn, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_bt_write(lpIn, nLe);
    //return -1;
}

s32 ddi_bt_read(u8 *lpOut, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_bt_read(lpOut, nLe);
    //return -1;
}

s32 ddi_bt_get_status(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_bt_get_status ();
    //return -1;
}

s32 ddi_bt_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_bt_ioctl(nCmd, lParam, wParam);
    //return -1;
}


//��Ƶͨ��
s32 ddi_audiocom_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audiocom_open();
    //return -1;
}

s32 ddi_audiocom_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audiocom_close ();
    //return -1;
}

s32 ddi_audiocom_clear(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audiocom_clear ();
    //return -1;
}

s32 ddi_audiocom_read(u8 *lpOut, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audiocom_read (lpOut, nLe );
    //return -1;
}

s32 ddi_audiocom_write(u8 *lpIn, s32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audiocom_write (lpIn, nLe );
    //return -1;
}

s32 ddi_audiocom_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_audiocom_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//LED
s32 ddi_led_open(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_led_open();
    //return -1;
}

s32 ddi_led_close(void) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_led_close();
    //return -1;
}

s32 ddi_led_sta_set(u32 nLed, u32 nSta) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_led_sta_set (nLed, nSta);
    //return -1;
}

s32 ddi_led_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_led_ioctl(nCmd, lParam, wParam);
    //return -1;
}

//usb
s32 ddi_usb_open(u32 nClass) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_usb_open(nClass);
}

s32 ddi_usb_close(u32 nClass) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_usb_close(nClass);
}

s32 ddi_usb_read(u32 nClass, u8 *lpOut, u32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_usb_read(nClass, lpOut, nLe);
}

s32 ddi_usb_write(u32 nClass, u8 *lpIn, u32 nLe) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_usb_write(nClass, lpIn, nLe);
}

s32 ddi_usb_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    DDI_SHOW_FUN_NAME;
    return -1;//CoreApi_ddi_usb_ioctl(nCmd, lParam, wParam);
}

//dukpt

s32 ddi_dukpt_open(void) {
    return -1;//CoreApi_ddi_dukpt_open();
}

s32 ddi_dukpt_close(void) {
    return -1;//CoreApi_ddi_dukpt_close();

}

s32 ddi_dukpt_inject(strDukptInitInfo *lpstrDukptInitInfo) {
    return -1;//CoreApi_ddi_dukpt_inject(lpstrDukptInitInfo);
}

s32 ddi_dukpt_encrypt(u32 nKeyGroup, u32 nKeyIndex, u32 nLen, const u8 *lpIn, u8 *lpOut,
                      u32 nMode) {
    return -1;//CoreApi_ddi_dukpt_encrypt(nKeyGroup, nKeyIndex, nLen, lpIn, lpOut, nMode); 
}

s32 ddi_dukpt_decrypt(u32 nKeyGroup, u32 nKsnIndex, u32 nLen, const u8 *lpIn, u8 *lpOut,
                      u32 nMode) {
    return -1;//CoreApi_ddi_dukpt_decrypt(nKeyGroup, nKsnIndex, nLen, lpIn,  lpOut, nMode);
}

s32 ddi_dukpt_getksn(u32 nKeyGroup, u32 nKeyIndex, u32 *lpLen, u8 *lpOut) {
    return -1;//CoreApi_ddi_dukpt_getksn(nKeyGroup, nKeyIndex, lpLen, lpOut);
}

s32 ddi_dukpt_ioctl(u32 nCmd, u32 lParam, u32 wParam) {
    return -1;//CoreApi_ddi_dukpt_ioctl(nCmd, lParam, wParam);
}


//--------------------
s32 ddi_sys_printf(const s8 *string) /* Put a formatted string to the file */
{
    LOGD("%s", string);
    return 0;
}

s32 ddi_core_printf(const s8 *fmt, ...) /* Put a formatted string to the file */
{
    s32 rvalue = 0;
    va_list ap;
    char *string;

    string = ddi_k_malloc(1024 + 128);

    if (NULL == string) {
        return 0;
    }
    memset(string, 0, 1024 + 128);
    va_start(ap, fmt);
    vsprintf(string, fmt, ap);

    //core->CoreApi_sys_uart_printf(string);
    ddi_sys_printf(string);
    va_end(ap);
    ddi_k_free(string);

    return rvalue;
}

void emv_uart_printf(char *fmt, ...)  //sxl?
{
    int length = 0;
    va_list ap;
    char *string;
    char *pt;

    string = ddi_k_malloc(1024 + 128);

    va_start(ap, fmt);
    vsprintf((char *) string, (const char *) fmt, ap);
    pt = &string[0];
    while (*pt != '\0') {
        length++;
        pt++;
    }
    ddi_sys_printf(string);
    //core->CoreApi_sys_uart_printf(string); //д���ڳɹ�
    va_end(ap);

    ddi_k_free(string);
}

//zhouxiaoxin pin������غ�������
s32 ddi_pin_input(u32 pinlenmin, u32 pinlenmax, u32 timeout, u32 bypassflag) {
#ifdef SPI_DDI
    return spi_ddi_pin_input(pinlenmin,pinlenmax,timeout,bypassflag);
#else
    return -1;
#endif
}

s32 ddi_pin_input_press(u8 *keycode) {
#ifdef SPI_DDI
    return spi_ddi_pin_input_press(keycode);
#else
    return -1;
#endif

}

s32 ddi_pin_input_cancel(void) {
#ifdef SPI_DDI
    return spi_ddi_pin_input_cancel();
#else
    return -1;
#endif

}

s32 ddi_pin_getonlinepinblock(u32 pinAlgorithmMode, u32 keyindex, u32 cardlen, u8 *carddata, u8 *pinblockdata) {
#ifdef SPI_DDI
    return spi_ddi_pin_getonlinepinblock (pinAlgorithmMode,keyindex,cardlen,carddata,pinblockdata);
#else
    return -1;
#endif

}

s32 ddi_pin_getofflineencpin(u8 *offlineencdata, u32 dataLen) {
#ifdef SPI_DDI
    return spi_ddi_pin_getofflineencpin(offlineencdata,dataLen);
#else
    return -1;
#endif

}

s32 ddi_pin_offlinepin_apdu(u32 nSlot, u32 pinAlgorithmMode, u8 *lpRApdu, u32 *lpRApduLen, u32 nRApduSize) {
#ifdef SPI_DDI
    return spi_ddi_pin_offlinepin_apdu(nSlot,pinAlgorithmMode, lpRApdu,lpRApduLen,nRApduSize);
#else
    return -1;
#endif

}
	

