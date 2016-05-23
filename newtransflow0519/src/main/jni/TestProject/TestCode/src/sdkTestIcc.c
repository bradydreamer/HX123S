#include "sdkGlobal.h"
#include "SdkTest.h"

//#include <unistd.h>

/*=======BEGIN: sujianzhong 2013.02.01  10:34 modify===========*/
static u8 gasTradeAmount[13] = {0};
/*====================== END======================== */

#define ICCONLINEPIN                            0                                       //IC����������PIN
#define ICCOFFLINEPIN                           1                                       //IC����������PIN
#define APPNUM                                  10                                                              //���Ӧ�ø���(��Daemon��ϵͳ����,����ܳ���30)
#define TPK_STORENO                                     0x00                                                                    //�洢TPK��������̵ı��
#define SDK_DISP_MDEFAULT    SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL


#define SDK_ICC_TEST_DEBUG
#undef SDK_ICC_TEST_DEBUG

#ifdef SDK_ICC_TEST_DEBUG
#define FILE_OK                         0                                               //�ļ������ɹ�
#define FILE_EOF                        1                                               //������ʱ�����ļ�β
#define FILE_SEEK_ERROR  2                                              //д�ļ���λԽ��
#define FILE_ERROR              3                                               //�ļ��򿪻򴴽�ʧ��
#define FILE_CRCERR             4                                               //�ļ�У���
//extern int isdigit(char c);



/*******************************************************************
   Name:		u8 TestGetPID(u8 *ProcessName,u32 *PID)

   Description :        Get PID of a specific process

   Parameters  :
                        u8 *ProcessName:
                                name of the process

                        u32 *PID:
                                pointer of the PID

   Return value:
                        RSLT_OK		successful
                        FILE_EOF,	Read end of file , can't find the process
                        FILE_ERROR	a failure when creating a file


   Remark:  //chenbilun 20111116   //20140414
 ********************************************************************/
u8 TestGetPID(u8 *ProcessName, u32 *PID)
{
    FILE *fp;
    u8 buf[30] = {0};
    u8 temp[30] = {0};
    u8 cmd[128] = {0};          //system cmd buffer
    u8 ret = 0;                         //result
    u8 flag = 0;                        // if flag == 1 ,find the PID.
    u32 i = 0;                          //PID counter
    u8 filename[30] = {0};       //filename buffer

    //init check
    sprintf(filename, "/mtd0/%s.txt", ProcessName);

    if(access(filename, F_OK) == 0)  //�ļ�����,ɾ��Ӧ�ö�Ӧ������Ŀ¼
    {
        strcat(filename, "1");
    }
    //PS
#ifdef cbl_debug
    sprintf(cmd, "ps |grep %s", ProcessName);
    system(cmd);
#endif
    sprintf(cmd, "ps |grep %s >%s", ProcessName, filename);
    system(cmd);

    //analyze the log
    if(access(filename, F_OK) != 0)                                                             //�ļ�������
    {
        for(i = strlen(filename); i > 0; i--)
        {
            if(filename[i - 1] == '/')
            {
                memset(temp, 0, sizeof(temp));
                strcpy(temp, "mkdir -p ");                                                              //�ݹ鴴���ļ�
                strncat(temp, filename, i - 1);
                system(temp);
                fp = fopen(filename, "wb+");                                                            //�������ļ�
                fclose(fp);
                break;
            }
        }
    }
    fp = fopen(filename, "rb");                                                                         //���ļ�
//    fp = XgdOpenFile(filename, "rb"); //�Ѷ�/д�ķ�ʽ��(����)�ļ�

    if(fp == NULL)
    {
        return FILE_EOF; //�ļ��������ʧ��
    }
    i = 0;
    flag = 0;

    while(1)
    {
        ret = fread(temp, sizeof(u8), 1, fp);              //read 1 byte every time.

        if(ret != 1)
        {
            if(feof(fp))
            {
                fclose(fp);
                memset(cmd, 0, sizeof(cmd));
                sprintf(cmd, "rm -f %s", filename);
                system(cmd);
                return FILE_EOF;                                                                //�����ļ�β
            }
            else
            {
                fclose(fp);
                memset(cmd, 0, sizeof(cmd));
                sprintf(cmd, "rm -f %s", filename);
                system(cmd);
                return FILE_ERROR;                                                      //������Ī��ʧ��,��Ϊ�����ļ�β
            }
        }
        else
        {
            if(isdigit(temp[0]))                                                //get to PID
            {
                buf[i] = temp[0];
                i++;
                flag = 1;                                                       //find the PID
            }
            else
            {
                if(flag)                                                        //PID finish
                {
                    break;
                }
            }
        }
    }

    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "rm -f %s", filename);
    system(cmd);
#ifdef cbl_debug
    printf("%s %d\r\n", __FUNCTION__, __LINE__);
    printf("buf=%s\r\n", buf);
#endif
    *PID = atoi(buf);
#ifdef cbl_debug
    printf("%s %d\r\n", __FUNCTION__, __LINE__);
    printf("PID=%d\r\n", *PID);
#endif
    return 0;
}

/*******************************************************************
   Name:		u8 TestKillProcess(u8 mode,u8 *ProcessName)

   Description :        kill a process

   Parameters  :
                        u8 mode:
                                0       use kill command
                                1	use killall command

                        u8 *ProcessName
                                name of the process

   Return value:
                        RSLT_OK		successful
                        other		return value from TestGetPID


   Remark:  //chenbilun 20111116  //20140414
 ********************************************************************/
u8 TestKillProcess(u8 mode, u8 *ProcessName)
{
    u8 temp[128] = {0};
    u32 PID;
    u8 ret = 0;

#ifdef cbl_debug
    printf("ProcessName=%s\r\n", ProcessName);
#endif

    if(!mode)
    {
        ret = TestGetPID(ProcessName, &PID);
#ifdef cbl_debug
        printf("TestGetPID ret = %d\r\n", ret);
#endif

        if(!ret)
        {
            sprintf(temp, "kill -9 %d", PID);
            system(temp);
            return 0;
        }

        else
        {
            return 1;
        }
    }
    else
    {
        sprintf(temp, "killall -HUP %s", ProcessName);
        system(temp);
        return 0;
    }
}

/*******************************************************************
   Name:		u8 TestKillAllProcess(void)

   Description :        kill all process

   Parameters  :

   Return value:

   Remark:  //chenbilun 20111116  //20140414
 ********************************************************************/
void TestKillAllProcess()
{
    u8 key;

    sdkDispClearScreen();
    sdkDispFillRowRam(1, 0, "Kill all process?", SDK_DISP_FDISP | SDK_DISP_CDISP);
    sdkDispFillRowRam(3, 0, "OK            CNL", SDK_DISP_FDISP | SDK_DISP_CDISP);
    sdkDispBrushScreen();

    while(1)
    {
        key = sdkKbGetKey();
        TestLog("", "key = %d\r\n", key);

        if(SDK_KEY_9 == key)
        {
            TestKillProcess(1, "manage.so");
            TestKillProcess(1, "panel.so");
            TestKillProcess(1, "daemon.bin");
            TestKillProcess(0, "gsmppp.so");
            TestKillProcess(0, "gsmmux.so");
            //TestKillProcess(0,"wpa_supplicant");
            //TestKillProcess(0,"dhcpcd");
            TestLog("", "OK to Quit");
            exit(0);
        }
        else if(key)
        {
            break;
        }
    }
}

#endif

const SDK_ICC_EmvSysIni_STRUCT sdkTestAID[] =
        {
                { //1
                        0x08, {0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x00, {0x00, 0x20},
                                            {0xD8, 0x40, 0x00, 0xA8, 0x00},
                                                                            {0xD8, 0x40, 0x04, 0xF8, 0x00},
                                                                                                            {0x00, 0x10, 0x00, 0x00, 0x00},
                                                                                                                                            {0x00, 0x00, 0x00, 0x01}, {0x00, 0x00, 0x00, 0x00},
                                                                                                                                                                                                0x99, 0x99, 0x03,
                        {0x9F, 0x37, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                                                 0x01, {0x00, 0x00, 0x00, 0x10, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        {0x00, 0x00, 0x00, 0x10, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        0x00, 0x00, 0x00,
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00
                },

                { //2
                        0x08, {0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x00, {0x00, 0x20},
                                            {0xD8, 0x40, 0x00, 0xA8, 0x00}, {0xD8, 0x40, 0x04, 0xF8, 0x00}, {0x00, 0x10, 0x00, 0x00, 0x00},
                                                                                                                                            {0x00, 0x00, 0x00, 0x01}, {0x00, 0x00, 0x00, 0x00},
                                                                                                                                                                                                0x99, 0x99, 0x03,
                        {0x9F, 0x37, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                                                 0x01, {0x00, 0x00, 0x00, 0x10, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        {0x00, 0x00, 0x00, 0x10, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        0x00, 0x00, 0x00,
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00
                },

                { //3
                        0x08, {0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x00, {0x00, 0x20}, {0xD8, 0x40, 0x00, 0xA8, 0x00}, {0xD8, 0x40, 0x04, 0xF8, 0x00}, {0x00, 0x10, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x01}, {0x00, 0x00, 0x00, 0x00},
                                                                                                                                                                                                0x99, 0x99, 0x03,
                        {
                         0x9F, 0x37, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        },
                                                                                 0x01,
                                                                                       {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                                                                                                             {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        0x00, 0x00, 0x00,
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00
                },

                { //4
                        0x08, {0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x00, {0x00, 0x20},
                                            {0xD8, 0x40, 0x00, 0xA8, 0x00},
                                                                            {0xD8, 0x40, 0x04, 0xF8, 0x00},
                                                                                                            {0x00, 0x10, 0x00, 0x00, 0x00},
                                                                                                                                            {0x00, 0x00, 0x00, 0xA0},
                                                                                                                                                                      {0x00, 0x00, 0x00, 0x00}, 0x99, 0x99, 0x03,
                        {0x9F, 0x37, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x01,
                                                                                       {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                                                                                                             {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                        0x00, 0x00, 0x00,
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00
                },

                { //5
                        0x07, {0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x01, {0x00, 0x8C},
                                            {0xD8, 0x40, 0x00, 0xA8, 0x00},
                                                                            {0xD8, 0x40, 0x00, 0xF8, 0x00},
                                                                                                            {0x00, 0x10, 0x00, 0x00, 0x00},
                                                                                                                                            {0x00, 0x00, 0x00, 0x00},
                                                                                                                                                                      {0x00, 0x00, 0x00, 0x00}, 0x25, 0x23, 0x03,
                        {0x9F, 0x37, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x01,
                                                                                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                                                                                             {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x00, 0x00, 0x00,
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00
                },

                { //6
                        0x07, {0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x00, {0x00, 0x8C},
                                            {0xD8, 0x40, 0x00, 0xA8, 0x00},
                                                                            {0xD8, 0x40, 0x00, 0xF8, 0x00},
                                                                                                            {0x00, 0x10, 0x00, 0x00, 0x00},
                                                                                                                                            {0x00, 0x00, 0x00, 0x00},
                                                                                                                                                                      {0x00, 0x00, 0x00, 0x00},
                                                                                                                                                                                                0x25, 0x23, 0x03,
                        {
                         0x9F, 0x37, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        },                                                       0x01,
                                                                                       {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                                                                                             {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                        0x00, 0x00, 0x00,
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00
                },
        };


const SDK_ICC_CAPK_STRUCT sdkTestCAPK[] =  //slx? ���Գ������в�����
        {
                { //1
                        {0xA0, 0x00, 0x00, 0x03, 0x33}, 0x04, 0x01, 0x01,
                        0xF8, {0xBC, 0x85, 0x3E, 0x6B, 0x53, 0x65, 0xE8,
                                      0x9E, 0x7E, 0xE9, 0x31, 0x7C, 0x94, 0xB0, 0x2D,
                                      0x0A, 0xBB, 0x0D, 0xBD, 0x91, 0xC0, 0x5A, 0x22,
                                      0x4A, 0x25, 0x54, 0xAA, 0x29, 0xED, 0x9F, 0xCB,
                                      0x9D, 0x86, 0xEB, 0x9C, 0xCB, 0xB3, 0x22, 0xA5,
                                      0x78, 0x11, 0xF8, 0x61, 0x88, 0xAA, 0xC7, 0x35,
                                      0x1C, 0x72, 0xBD, 0x9E, 0xF1, 0x96, 0xC5, 0xA0,
                                      0x1A, 0xCE, 0xF7, 0xA4, 0xEB, 0x0D, 0x2A, 0xD6,
                                      0x3D, 0x9E, 0x6A, 0xC2, 0xE7, 0x83, 0x65, 0x47,
                                      0xCB, 0x15, 0x95, 0xC6, 0x8B, 0xCB, 0xAF, 0xD0,
                                      0xF6, 0x72, 0x87, 0x60, 0xF3, 0xA7, 0xCA, 0x7B,
                                      0x97, 0x30, 0x1B, 0x7E, 0x02, 0x20, 0x18, 0x4E,
                                      0xFC, 0x4F, 0x65, 0x30, 0x08, 0xD9, 0x3C, 0xE0,
                                      0x98, 0xC0, 0xD9, 0x3B, 0x45, 0x20, 0x10, 0x96,
                                      0xD1, 0xAD, 0xFF, 0x4C, 0xF1, 0xF9, 0xFC, 0x02,
                                      0xAF, 0x75, 0x9D, 0xA2, 0x7C, 0xD6, 0xDF, 0xD6,
                                      0xD7, 0x89, 0xB0, 0x99, 0xF1, 0x6F, 0x37, 0x8B,
                                      0x61, 0x00, 0x33, 0x4E, 0x63, 0xF3, 0xD3, 0x5F,
                                      0x32, 0x51, 0xA5, 0xEC, 0x78, 0x69, 0x37, 0x31,
                                      0xF5, 0x23, 0x35, 0x19, 0xCD, 0xB3, 0x80, 0xF5,
                                      0xAB, 0x8C, 0x0F, 0x02, 0x72, 0x8E, 0x91, 0xD4,
                                      0x69, 0xAB, 0xD0, 0xEA, 0xE0, 0xD9, 0x3B, 0x1C,
                                      0xC6, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x00}, 0x14,
                        {0xF5, 0x27, 0x08, 0x1C, 0xF3, 0x71, 0xDD, 0x7E, 0x1F, 0xD4, 0xFA, 0x41, 0x4A, 0x66, 0x50, 0x36, 0xE0, 0xF5, 0xE6, 0xE5},
                        {0x20, 0x17, 0x12, 0x31}
                },


                { //2
                        {0xA0, 0x00, 0x00, 0x03, 0x33}, 0x03, 0x01, 0x01,
                        0xB0, {0xB0, 0x62, 0x7D, 0xEE, 0x87, 0x86, 0x4F,
                                      0x9C, 0x18, 0xC1, 0x3B, 0x9A, 0x1F, 0x02, 0x54,
                                      0x48, 0xBF, 0x13, 0xC5, 0x83, 0x80, 0xC9, 0x1F,
                                      0x4C, 0xEB, 0xA9, 0xF9, 0xBC, 0xB2, 0x14, 0xFF,
                                      0x84, 0x14, 0xE9, 0xB5, 0x9D, 0x6A, 0xBA, 0x10,
                                      0xF9, 0x41, 0xC7, 0x33, 0x17, 0x68, 0xF4, 0x7B,
                                      0x21, 0x27, 0x90, 0x7D, 0x85, 0x7F, 0xA3, 0x9A,
                                      0xAF, 0x8C, 0xE0, 0x20, 0x45, 0xDD, 0x01, 0x61,
                                      0x9D, 0x68, 0x9E, 0xE7, 0x31, 0xC5, 0x51, 0x15,
                                      0x9B, 0xE7, 0xEB, 0x2D, 0x51, 0xA3, 0x72, 0xFF,
                                      0x56, 0xB5, 0x56, 0xE5, 0xCB, 0x2F, 0xDE, 0x36,
                                      0xE2, 0x30, 0x73, 0xA4, 0x4C, 0xA2, 0x15, 0xD6,
                                      0xC2, 0x6C, 0xA6, 0x88, 0x47, 0xB3, 0x88, 0xE3,
                                      0x95, 0x20, 0xE0, 0x02, 0x6E, 0x62, 0x29, 0x4B,
                                      0x55, 0x7D, 0x64, 0x70, 0x44, 0x0C, 0xA0, 0xAE,
                                      0xFC, 0x94, 0x38, 0xC9, 0x23, 0xAE, 0xC9, 0xB2,
                                      0x09, 0x8D, 0x6D, 0x3A, 0x1A, 0xF5, 0xE8, 0xB1,
                                      0xDE, 0x36, 0xF4, 0xB5, 0x30, 0x40, 0x10, 0x9D,
                                      0x89, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x00}, 0x14,
                        {0x87, 0xF0, 0xCD, 0x7C, 0x0E, 0x86, 0xF3, 0x8F, 0x89, 0xA6, 0x6F, 0x8C, 0x47, 0x07, 0x1A, 0x8B, 0x88, 0x58, 0x6F, 0x26},
                        {0x20, 0x17, 0x12, 0x31}
                },

                { //3
                        {0xA0, 0x00, 0x00, 0x03, 0x33}, 0x02, 0x01, 0x01,
                        0x90, {0xA3, 0x76, 0x7A, 0xBD, 0x1B, 0x6A, 0xA6,
                                      0x9D, 0x7F, 0x3F, 0xBF, 0x28, 0xC0, 0x92, 0xDE,
                                      0x9E, 0xD1, 0xE6, 0x58, 0xBA, 0x5F, 0x09, 0x09,
                                      0xAF, 0x7A, 0x1C, 0xCD, 0x90, 0x73, 0x73, 0xB7,
                                      0x21, 0x0F, 0xDE, 0xB1, 0x62, 0x87, 0xBA, 0x8E,
                                      0x78, 0xE1, 0x52, 0x9F, 0x44, 0x39, 0x76, 0xFD,
                                      0x27, 0xF9, 0x91, 0xEC, 0x67, 0xD9, 0x5E, 0x5F,
                                      0x4E, 0x96, 0xB1, 0x27, 0xCA, 0xB2, 0x39, 0x6A,
                                      0x94, 0xD6, 0xE4, 0x5C, 0xDA, 0x44, 0xCA, 0x4C,
                                      0x48, 0x67, 0x57, 0x0D, 0x6B, 0x07, 0x54, 0x2F,
                                      0x8D, 0x4B, 0xF9, 0xFF, 0x97, 0x97, 0x5D, 0xB9,
                                      0x89, 0x15, 0x15, 0xE6, 0x6F, 0x52, 0x5D, 0x2B,
                                      0x3C, 0xBE, 0xB6, 0xD6, 0x62, 0xBF, 0xB6, 0xC3,
                                      0xF3, 0x38, 0xE9, 0x3B, 0x02, 0x14, 0x2B, 0xFC,
                                      0x44, 0x17, 0x3A, 0x37, 0x64, 0xC5, 0x6A, 0xAD,
                                      0xD2, 0x02, 0x07, 0x5B, 0x26, 0xDC, 0x2F, 0x9F,
                                      0x7D, 0x7A, 0xE7, 0x4B, 0xD7, 0xD0, 0x0F, 0xD0,
                                      0x5E, 0xE4, 0x30, 0x03, 0x26, 0x63, 0xD2, 0x7A,
                                      0x57, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x00}, 0x14,
                        {0x03, 0xBB, 0x33, 0x5A, 0x85, 0x49, 0xA0, 0x3B, 0x87, 0xAB, 0x08, 0x9D, 0x00, 0x6F, 0x60, 0x85, 0x2E, 0x4B, 0x80, 0x60},
                        {0x20, 0x14, 0x12, 0x31}
                },


                { //4
                        {0xA0, 0x00, 0x00, 0x03, 0x33}, 0x01, 0x01, 0x01,
                        0x80, {0xBB, 0xE9, 0x06, 0x6D, 0x25, 0x17, 0x51,
                                      0x1D, 0x23, 0x9C, 0x7B, 0xFA, 0x77, 0x88, 0x41,
                                      0x44, 0xAE, 0x20, 0xC7, 0x37, 0x2F, 0x51, 0x51,
                                      0x47, 0xE8, 0xCE, 0x65, 0x37, 0xC5, 0x4C, 0x0A,
                                      0x6A, 0x4D, 0x45, 0xF8, 0xCA, 0x4D, 0x29, 0x08,
                                      0x70, 0xCD, 0xA5, 0x9F, 0x13, 0x44, 0xEF, 0x71,
                                      0xD1, 0x7D, 0x3F, 0x35, 0xD9, 0x2F, 0x3F, 0x06,
                                      0x77, 0x8D, 0x0D, 0x51, 0x1E, 0xC2, 0xA7, 0xDC,
                                      0x4F, 0xFE, 0xAD, 0xF4, 0xFB, 0x12, 0x53, 0xCE,
                                      0x37, 0xA7, 0xB2, 0xB5, 0xA3, 0x74, 0x12, 0x27,
                                      0xBE, 0xF7, 0x25, 0x24, 0xDA, 0x7A, 0x2B, 0x7B,
                                      0x1C, 0xB4, 0x26, 0xBE, 0xE2, 0x7B, 0xC5, 0x13,
                                      0xB0, 0xCB, 0x11, 0xAB, 0x99, 0xBC, 0x1B, 0xC6,
                                      0x1D, 0xF5, 0xAC, 0x6C, 0xC4, 0xD8, 0x31, 0xD0,
                                      0x84, 0x87, 0x88, 0xCD, 0x74, 0xF6, 0xD5, 0x43,
                                      0xAD, 0x37, 0xC5, 0xA2, 0xB4, 0xC5, 0xD5, 0xA9,
                                      0x3B, 0x7A, 0xE7, 0x4B, 0xD7, 0xD0, 0x0F, 0xD0,
                                      0x5E, 0xE4, 0x30, 0x03, 0x26, 0x63, 0xD2, 0x7A,
                                      0x57, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x00}, 0x14,
                        {0xE8, 0x81, 0xE3, 0x90, 0x67, 0x5D, 0x44, 0xC2, 0xDD, 0x81, 0x23, 0x4D, 0xCE, 0x29, 0xC3, 0xF5, 0xAB, 0x22, 0x97, 0xA0},
                        {0x20, 0x16, 0x12, 0x31}
                },

                { //5
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x50, 0x01, 0x01,
                        0x80, {0xD1, 0x11, 0x97, 0x59, 0x00, 0x57, 0xB8,
                                      0x41, 0x96, 0xC2, 0xF4, 0xD1, 0x1A, 0x8F, 0x3C,
                                      0x05, 0x40, 0x8F, 0x42, 0x2A, 0x35, 0xD7, 0x02,
                                      0xF9, 0x01, 0x06, 0xEA, 0x5B, 0x01, 0x9B, 0xB2,
                                      0x8A, 0xE6, 0x07, 0xAA, 0x9C, 0xDE, 0xBC, 0xD0,
                                      0xD8, 0x1A, 0x38, 0xD4, 0x8C, 0x7E, 0xBB, 0x00,
                                      0x62, 0xD2, 0x87, 0x36, 0x9E, 0xC0, 0xC4, 0x21,
                                      0x24, 0x24, 0x6A, 0xC3, 0x0D, 0x80, 0xCD, 0x60,
                                      0x2A, 0xB7, 0x23, 0x8D, 0x51, 0x08, 0x4D, 0xED,
                                      0x46, 0x98, 0x16, 0x2C, 0x59, 0xD2, 0x5E, 0xAC,
                                      0x1E, 0x66, 0x25, 0x5B, 0x4D, 0xB2, 0x35, 0x25,
                                      0x26, 0xEF, 0x09, 0x82, 0xC3, 0xB8, 0xAD, 0x3D,
                                      0x1C, 0xCE, 0x85, 0xB0, 0x1D, 0xB5, 0x78, 0x8E,
                                      0x75, 0xE0, 0x9F, 0x44, 0xBE, 0x73, 0x61, 0x36,
                                      0x6D, 0xEF, 0x9D, 0x1E, 0x13, 0x17, 0xB0, 0x5E,
                                      0x5D, 0x0F, 0xF5, 0x29, 0x0F, 0x88, 0xA0, 0xDB,
                                      0x47, 0x7A, 0xE7, 0x4B, 0xD7, 0xD0, 0x0F, 0xD0,
                                      0x5E, 0xE4, 0x30, 0x03, 0x26, 0x63, 0xD2, 0x7A,
                                      0x57, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x03, {0x01, 0x00, 0x01}, 0x14,
                        {0xB7, 0x69, 0x77, 0x56, 0x68, 0xCA, 0xCB, 0x5D, 0x22, 0xA6, 0x47, 0xD1, 0xD9, 0x93, 0x14, 0x1E, 0xDA, 0xB7, 0x23, 0x7B},
                        {0x20, 0x49, 0x02, 0x31}
                },

                { //6
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x51, 0x01, 0x01,
                        0x90, {0xDB, 0x5F, 0xA2, 0x9D, 0x1F, 0xDA, 0x8C,
                                      0x16, 0x34, 0xB0, 0x4D, 0xCC, 0xFF, 0x14, 0x8A,
                                      0xBE, 0xE6, 0x3C, 0x77, 0x20, 0x35, 0xC7, 0x98,
                                      0x51, 0xD3, 0x51, 0x21, 0x07, 0x58, 0x6E, 0x02,
                                      0xA9, 0x17, 0xF7, 0xC7, 0xE8, 0x85, 0xE7, 0xC4,
                                      0xA7, 0xD5, 0x29, 0x71, 0x0A, 0x14, 0x53, 0x34,
                                      0xCE, 0x67, 0xDC, 0x41, 0x2C, 0xB1, 0x59, 0x7B,
                                      0x77, 0xAA, 0x25, 0x43, 0xB9, 0x8D, 0x19, 0xCF,
                                      0x2C, 0xB8, 0x0C, 0x52, 0x2B, 0xDB, 0xEA, 0x0F,
                                      0x1B, 0x11, 0x3F, 0xA2, 0xC8, 0x62, 0x16, 0xC8,
                                      0xC6, 0x10, 0xA2, 0xD5, 0x8F, 0x29, 0xCF, 0x33,
                                      0x55, 0xCE, 0xB1, 0xBD, 0x3E, 0xF4, 0x10, 0xD1,
                                      0xED, 0xD1, 0xF7, 0xAE, 0x0F, 0x16, 0x89, 0x79,
                                      0x79, 0xDE, 0x28, 0xC6, 0xEF, 0x29, 0x3E, 0x0A,
                                      0x19, 0x28, 0x2B, 0xD1, 0xD7, 0x93, 0xF1, 0x33,
                                      0x15, 0x23, 0xFC, 0x71, 0xA2, 0x28, 0x80, 0x04,
                                      0x68, 0xC0, 0x1A, 0x36, 0x53, 0xD1, 0x4C, 0x6B,
                                      0x48, 0x51, 0xA5, 0xC0, 0x29, 0x47, 0x8E, 0x75,
                                      0x7F, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x01}, 0x14,
                        {0xB9, 0xD2, 0x48, 0x07, 0x5A, 0x3F, 0x23, 0xB5, 0x22, 0xFE, 0x45, 0x57, 0x3E, 0x04, 0x37, 0x4D, 0xC4, 0x99, 0x5D, 0x71},
                        {0x20, 0x49, 0x12, 0x31}
                },

                { //7
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x99, 0x01, 0x01,
                        0x80, {0xAB, 0x79, 0xFC, 0xC9, 0x52, 0x08, 0x96,
                                      0x96, 0x7E, 0x77, 0x6E, 0x64, 0x44, 0x4E, 0x5D,
                                      0xCD, 0xD6, 0xE1, 0x36, 0x11, 0x87, 0x4F, 0x39,
                                      0x85, 0x72, 0x25, 0x20, 0x42, 0x52, 0x95, 0xEE,
                                      0xA4, 0xBD, 0x0C, 0x27, 0x81, 0xDE, 0x7F, 0x31,
                                      0xCD, 0x3D, 0x04, 0x1F, 0x56, 0x5F, 0x74, 0x73,
                                      0x06, 0xEE, 0xD6, 0x29, 0x54, 0xB1, 0x7E, 0xDA,
                                      0xBA, 0x3A, 0x6C, 0x5B, 0x85, 0xA1, 0xDE, 0x1B,
                                      0xEB, 0x9A, 0x34, 0x14, 0x1A, 0xF3, 0x8F, 0xCF,
                                      0x82, 0x79, 0xC9, 0xDE, 0xA0, 0xD5, 0xA6, 0x71,
                                      0x0D, 0x08, 0xDB, 0x41, 0x24, 0xF0, 0x41, 0x94,
                                      0x55, 0x87, 0xE2, 0x03, 0x59, 0xBA, 0xB4, 0x7B,
                                      0x75, 0x75, 0xAD, 0x94, 0x26, 0x2D, 0x4B, 0x25,
                                      0xF2, 0x64, 0xAF, 0x33, 0xDE, 0xDC, 0xF2, 0x8E,
                                      0x09, 0x61, 0x5E, 0x93, 0x7D, 0xE3, 0x2E, 0xDC,
                                      0x03, 0xC5, 0x44, 0x45, 0xFE, 0x7E, 0x38, 0x27,
                                      0x77, 0xC0, 0x1A, 0x36, 0x53, 0xD1, 0x4C, 0x6B,
                                      0x48, 0x51, 0xA5, 0xC0, 0x29, 0x47, 0x8E, 0x75,
                                      0x7F, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x01}, 0x14,
                        {0x4A, 0xBF, 0xFD, 0x6B, 0x1C, 0x51, 0x21, 0x2D, 0x05, 0x55, 0x2E, 0x43, 0x1C, 0x5B, 0x17, 0x00, 0x7D, 0x2F, 0x5E, 0x6D},
                        {0x20, 0x43, 0x02, 0x28}
                },

                { //8
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x98, 0x01, 0x01,
                        0x70, {0xCA, 0x02, 0x6E, 0x52, 0xA6, 0x95, 0xE7,
                                      0x2B, 0xD3, 0x0A, 0xF9, 0x28, 0x19, 0x6E, 0xED,
                                      0xC9, 0xFA, 0xF4, 0xA6, 0x19, 0xF2, 0x49, 0x2E,
                                      0x3F, 0xB3, 0x11, 0x69, 0x78, 0x9C, 0x27, 0x6F,
                                      0xFB, 0xB7, 0xD4, 0x31, 0x16, 0x64, 0x7B, 0xA9,
                                      0xE0, 0xD1, 0x06, 0xA3, 0x54, 0x2E, 0x39, 0x65,
                                      0x29, 0x2C, 0xF7, 0x78, 0x23, 0xDD, 0x34, 0xCA,
                                      0x8E, 0xEC, 0x7D, 0xE3, 0x67, 0xE0, 0x80, 0x70,
                                      0x89, 0x50, 0x77, 0xC7, 0xEF, 0xAD, 0x93, 0x99,
                                      0x24, 0xCB, 0x18, 0x70, 0x67, 0xDB, 0xF9, 0x2C,
                                      0xB1, 0xE7, 0x85, 0x91, 0x7B, 0xD3, 0x8B, 0xAC,
                                      0xE0, 0xC1, 0x94, 0xCA, 0x12, 0xDF, 0x0C, 0xE5,
                                      0xB7, 0xA5, 0x02, 0x75, 0xAC, 0x61, 0xBE, 0x7C,
                                      0x3B, 0x43, 0x68, 0x87, 0xCA, 0x98, 0xC9, 0xFD,
                                      0x39, 0x61, 0x5E, 0x93, 0x7D, 0xE3, 0x2E, 0xDC,
                                      0x03, 0xC5, 0x44, 0x45, 0xFE, 0x7E, 0x38, 0x27,
                                      0x77, 0xC0, 0x1A, 0x36, 0x53, 0xD1, 0x4C, 0x6B,
                                      0x48, 0x51, 0xA5, 0xC0, 0x29, 0x47, 0x8E, 0x75,
                                      0x7F, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x01}, 0x14,
                        {0xE7, 0xAC, 0x9A, 0xA8, 0xEE, 0xD1, 0xB5, 0xFF, 0x1B, 0xD5, 0x32, 0xCF, 0x14, 0x89, 0xA3, 0xE5, 0x55, 0x75, 0x72, 0xC1},
                        {0x20, 0x49, 0x12, 0x31}
                },

                { //9
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x97, 0x01, 0x01,
                        0x60, {0xAF, 0x07, 0x54, 0xEA, 0xED, 0x97, 0x70,
                                      0x43, 0xAB, 0x6F, 0x41, 0xD6, 0x31, 0x2A, 0xB1,
                                      0xE2, 0x2A, 0x68, 0x09, 0x17, 0x5B, 0xEB, 0x28,
                                      0xE7, 0x0D, 0x5F, 0x99, 0xB2, 0xDF, 0x18, 0xCA,
                                      0xE7, 0x35, 0x19, 0x34, 0x1B, 0xBB, 0xD3, 0x27,
                                      0xD0, 0xB8, 0xBE, 0x9D, 0x4D, 0x0E, 0x15, 0xF0,
                                      0x7D, 0x36, 0xEA, 0x3E, 0x3A, 0x05, 0xC8, 0x92,
                                      0xF5, 0xB1, 0x9A, 0x3E, 0x9D, 0x34, 0x13, 0xB0,
                                      0xD9, 0x7E, 0x7A, 0xD1, 0x0A, 0x5F, 0x5D, 0xE8,
                                      0xE3, 0x88, 0x60, 0xC0, 0xAD, 0x00, 0x4B, 0x1E,
                                      0x06, 0xF4, 0x04, 0x0C, 0x29, 0x5A, 0xCB, 0x45,
                                      0x7A, 0x78, 0x85, 0x51, 0xB6, 0x12, 0x7C, 0x0B,
                                      0x29, 0xA5, 0x02, 0x75, 0xAC, 0x61, 0xBE, 0x7C,
                                      0x3B, 0x43, 0x68, 0x87, 0xCA, 0x98, 0xC9, 0xFD,
                                      0x39, 0x61, 0x5E, 0x93, 0x7D, 0xE3, 0x2E, 0xDC,
                                      0x03, 0xC5, 0x44, 0x45, 0xFE, 0x7E, 0x38, 0x27,
                                      0x77, 0xC0, 0x1A, 0x36, 0x53, 0xD1, 0x4C, 0x6B,
                                      0x48, 0x51, 0xA5, 0xC0, 0x29, 0x47, 0x8E, 0x75,
                                      0x7F, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x01}, 0x14,
                        {0x80, 0x01, 0xCA, 0x76, 0xC1, 0x20, 0x39, 0x55, 0xE2, 0xC6, 0x28, 0x41, 0xCD, 0x6F, 0x20, 0x10, 0x87, 0xE5, 0x64, 0xBF},
                        {0x20, 0x49, 0x12, 0x31}
                },

                { //10
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x96, 0x01, 0x01,
                        0x80, {0xB7, 0x45, 0x86, 0xD1, 0x9A, 0x20, 0x7B,
                                      0xE6, 0x62, 0x7C, 0x5B, 0x0A, 0xAF, 0xBC, 0x44,
                                      0xA2, 0xEC, 0xF5, 0xA2, 0x94, 0x2D, 0x3A, 0x26,
                                      0xCE, 0x19, 0xC4, 0xFF, 0xAE, 0xEE, 0x92, 0x05,
                                      0x21, 0x86, 0x89, 0x22, 0xE8, 0x93, 0xE7, 0x83,
                                      0x82, 0x25, 0xA3, 0x94, 0x7A, 0x26, 0x14, 0x79,
                                      0x6F, 0xB2, 0xC0, 0x62, 0x8C, 0xE8, 0xC1, 0x1E,
                                      0x38, 0x25, 0xA5, 0x6D, 0x3B, 0x1B, 0xBA, 0xEF,
                                      0x78, 0x3A, 0x5C, 0x6A, 0x81, 0xF3, 0x6F, 0x86,
                                      0x25, 0x39, 0x51, 0x26, 0xFA, 0x98, 0x3C, 0x52,
                                      0x16, 0xD3, 0x16, 0x6D, 0x48, 0xAC, 0xDE, 0x8A,
                                      0x43, 0x12, 0x12, 0xFF, 0x76, 0x3A, 0x7F, 0x79,
                                      0xD9, 0xED, 0xB7, 0xFE, 0xD7, 0x6B, 0x48, 0x5D,
                                      0xE4, 0x5B, 0xEB, 0x82, 0x9A, 0x3D, 0x47, 0x30,
                                      0x84, 0x8A, 0x36, 0x6D, 0x33, 0x24, 0xC3, 0x02,
                                      0x70, 0x32, 0xFF, 0x8D, 0x16, 0xA1, 0xE4, 0x4D,
                                      0x8D, 0xC0, 0x1A, 0x36, 0x53, 0xD1, 0x4C, 0x6B,
                                      0x48, 0x51, 0xA5, 0xC0, 0x29, 0x47, 0x8E, 0x75,
                                      0x7F, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x01}, 0x14,
                        {0x76, 0x16, 0xE9, 0xAC, 0x8B, 0xE0, 0x14, 0xAF, 0x88, 0xCA, 0x11, 0xA8, 0xFB, 0x17, 0x96, 0x7B, 0x73, 0x94, 0x03, 0x0E},
                        {0x20, 0x49, 0x12, 0x31}
                },

                { //11
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x95, 0x01, 0x01,
                        0x90, {0xBE, 0x9E, 0x1F, 0xA5, 0xE9, 0xA8, 0x03,
                                      0x85, 0x29, 0x99, 0xC4, 0xAB, 0x43, 0x2D, 0xB2,
                                      0x86, 0x00, 0xDC, 0xD9, 0xDA, 0xB7, 0x6D, 0xFA,
                                      0xAA, 0x47, 0x35, 0x5A, 0x0F, 0xE3, 0x7B, 0x15,
                                      0x08, 0xAC, 0x6B, 0xF3, 0x88, 0x60, 0xD3, 0xC6,
                                      0xC2, 0xE5, 0xB1, 0x2A, 0x3C, 0xAA, 0xF2, 0xA7,
                                      0x00, 0x5A, 0x72, 0x41, 0xEB, 0xAA, 0x77, 0x71,
                                      0x11, 0x2C, 0x74, 0xCF, 0x9A, 0x06, 0x34, 0x65,
                                      0x2F, 0xBC, 0xA0, 0xE5, 0x98, 0x0C, 0x54, 0xA6,
                                      0x47, 0x61, 0xEA, 0x10, 0x1A, 0x11, 0x4E, 0x0F,
                                      0x0B, 0x55, 0x72, 0xAD, 0xD5, 0x7D, 0x01, 0x0B,
                                      0x7C, 0x9C, 0x88, 0x7E, 0x10, 0x4C, 0xA4, 0xEE,
                                      0x12, 0x72, 0xDA, 0x66, 0xD9, 0x97, 0xB9, 0xA9,
                                      0x0B, 0x5A, 0x6D, 0x62, 0x4A, 0xB6, 0xC5, 0x7E,
                                      0x73, 0xC8, 0xF9, 0x19, 0x00, 0x0E, 0xB5, 0xF6,
                                      0x84, 0x89, 0x8E, 0xF8, 0xC3, 0xDB, 0xEF, 0xB3,
                                      0x30, 0xC6, 0x26, 0x60, 0xBE, 0xD8, 0x8E, 0xA7,
                                      0x8E, 0x90, 0x9A, 0xFF, 0x05, 0xF6, 0xDA, 0x62,
                                      0x7B, 0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C,
                                      0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC,
                                      0x8A, 0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81,
                                      0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3,
                                      0x3D, 0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77,
                                      0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D,
                                      0x97, 0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06,
                                      0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57,
                                      0x87, 0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D,
                                      0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84,
                                      0x73, 0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD,
                                      0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC,
                                      0x8E, 0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA,
                                      0xF1}, 0x01, {0x03, 0x00, 0x01}, 0x14,
                        {0xEE, 0x15, 0x11, 0xCE, 0xC7, 0x10, 0x20, 0xA9, 0xB9, 0x04, 0x43, 0xB3, 0x7B, 0x1D, 0x5F, 0x6E, 0x70, 0x30, 0x30, 0xF6},
                        {0x20, 0x49, 0x12, 0x31}
                },

                { //12
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x94, 0x01, 0x01,
                        0xF8, {0xD1, 0xBE, 0x39, 0x61, 0x5F, 0x39, 0x5A,
                                      0xC9, 0x33, 0x7E, 0x33, 0x07, 0xAA, 0x5A, 0x7A,
                                      0xC3, 0x5E, 0xAE, 0x00, 0x36, 0xBF, 0x20, 0xB9,
                                      0x2F, 0x9A, 0x45, 0xD1, 0x90, 0xB2, 0xF4, 0x61,
                                      0x6A, 0xBF, 0x9D, 0x34, 0x0C, 0xBF, 0x5F, 0xBB,
                                      0x3A, 0x2B, 0x94, 0xBD, 0x8F, 0x2F, 0x97, 0x7C,
                                      0x0A, 0x10, 0xB9, 0x0E, 0x59, 0xD4, 0x20, 0x1A,
                                      0xA3, 0x26, 0x69, 0xE8, 0xCB, 0xE7, 0x53, 0xF5,
                                      0x36, 0x11, 0x9D, 0xF4, 0xFB, 0x5E, 0x63, 0xCE,
                                      0xD8, 0x7F, 0x11, 0x53, 0xCE, 0x91, 0x4B, 0x12,
                                      0x4F, 0x3E, 0x6B, 0x64, 0x8C, 0xD5, 0xC9, 0x76,
                                      0x55, 0xF7, 0xAB, 0x4D, 0xF6, 0x26, 0x07, 0xC9,
                                      0x5D, 0xA5, 0x05, 0x17, 0xAB, 0x8B, 0xE3, 0x83,
                                      0x66, 0x72, 0xD1, 0xC7, 0x1B, 0xCD, 0xE9, 0xBA,
                                      0x72, 0x93, 0xFF, 0x34, 0x82, 0xF1, 0x24, 0xF8,
                                      0x66, 0x91, 0x13, 0x0A, 0xB0, 0x81, 0x77, 0xB0,
                                      0x2F, 0x45, 0x9C, 0x02, 0x5A, 0x1F, 0x3D, 0xFF,
                                      0xE0, 0x88, 0x4C, 0xE7, 0x81, 0x22, 0x54, 0x2E,
                                      0xA1, 0xC8, 0xEA, 0x09, 0x2B, 0x55, 0x2B, 0x58,
                                      0x69, 0x07, 0xC8, 0x3A, 0xD6, 0x5E, 0x0C, 0x6F,
                                      0x91, 0xA4, 0x00, 0xE4, 0x85, 0xE1, 0x11, 0x92,
                                      0xAA, 0x4C, 0x17, 0x1C, 0x5A, 0x1E, 0xF5, 0x63,
                                      0x81, 0xF4, 0xD0, 0x91, 0xCC, 0x7E, 0xF6, 0xBD,
                                      0x86, 0x04, 0xCB, 0xC4, 0xC7, 0x4D, 0x5D, 0x77,
                                      0xFF, 0xA0, 0x7B, 0x64, 0x1D, 0x53, 0x99, 0x8C,
                                      0xDB, 0x5C, 0x21, 0xB7, 0xBC, 0x65, 0xE0, 0x82,
                                      0xA6, 0x51, 0x3F, 0x42, 0x4A, 0x4B, 0x25, 0x2E,
                                      0x0D, 0x77, 0xFA, 0x40, 0x56, 0x98, 0x6A, 0x0A,
                                      0xB0, 0xCD, 0xA6, 0x15, 0x5E, 0xD9, 0xA8, 0x83,
                                      0xC6, 0x9C, 0xC2, 0x99, 0x2D, 0x49, 0xEC, 0xBD,
                                      0x47, 0x97, 0xDD, 0x28, 0x64, 0xFF, 0xC9, 0x6B,
                                      0x8D},
                                             0x03, {0x01, 0x00, 0x01}, 0x14,
                        {0x1D, 0x19, 0x31, 0x49, 0xFD, 0x83, 0xDC, 0xB3, 0x37, 0xF4, 0xA1, 0xB5, 0xCF, 0xAA, 0x14, 0x4D, 0x06, 0x6B, 0x66, 0xFB},
                        {0x20, 0x49, 0x12, 0x31}
                },

                { //13
                        {0xA0, 0x00, 0x00, 0x00, 0x03}, 0x90, 0x01, 0x01,
                        0x40, {0xC2, 0x6B, 0x3C, 0xB3, 0x83, 0x3E, 0x42,
                                      0xD8, 0x27, 0x0D, 0xC1, 0x0C, 0x89, 0x99, 0xB2,
                                      0xDA, 0x18, 0x10, 0x68, 0x38, 0x65, 0x0D, 0xA0,
                                      0xDB, 0xF1, 0x54, 0xEF, 0xD5, 0x11, 0x00, 0xAD,
                                      0x14, 0x47, 0x41, 0xB2, 0xA8, 0x7D, 0x68, 0x81,
                                      0xF8, 0x63, 0x0E, 0x33, 0x48, 0xDE, 0xA3, 0xF7,
                                      0x80, 0x38, 0xE9, 0xB2, 0x1A, 0x69, 0x7E, 0xB2,
                                      0xA6, 0x71, 0x6D, 0x32, 0xCB, 0xF2, 0x60, 0x86,
                                      0xF1, 0x11, 0x9D, 0xF4, 0xFB, 0x5E, 0x63, 0xCE,
                                      0xD8, 0x7F, 0x11, 0x53, 0xCE, 0x91, 0x4B, 0x12,
                                      0x4F, 0x3E, 0x6B, 0x64, 0x8C, 0xD5, 0xC9, 0x76,
                                      0x55, 0xF7, 0xAB, 0x4D, 0xF6, 0x26, 0x07, 0xC9,
                                      0x5D, 0xA5, 0x05, 0x17, 0xAB, 0x8B, 0xE3, 0x83,
                                      0x66, 0x72, 0xD1, 0xC7, 0x1B, 0xCD, 0xE9, 0xBA,
                                      0x72, 0x93, 0xFF, 0x34, 0x82, 0xF1, 0x24, 0xF8,
                                      0x66, 0x91, 0x13, 0x0A, 0xB0, 0x81, 0x77, 0xB0,
                                      0x2F, 0x45, 0x9C, 0x02, 0x5A, 0x1F, 0x3D, 0xFF,
                                      0xE0, 0x88, 0x4C, 0xE7, 0x81, 0x22, 0x54, 0x2E,
                                      0xA1, 0xC8, 0xEA, 0x09, 0x2B, 0x55, 0x2B, 0x58,
                                      0x69, 0x07, 0xC8, 0x3A, 0xD6, 0x5E, 0x0C, 0x6F,
                                      0x91, 0xA4, 0x00, 0xE4, 0x85, 0xE1, 0x11, 0x92,
                                      0xAA, 0x4C, 0x17, 0x1C, 0x5A, 0x1E, 0xF5, 0x63,
                                      0x81, 0xF4, 0xD0, 0x91, 0xCC, 0x7E, 0xF6, 0xBD,
                                      0x86, 0x04, 0xCB, 0xC4, 0xC7, 0x4D, 0x5D, 0x77,
                                      0xFF, 0xA0, 0x7B, 0x64, 0x1D, 0x53, 0x99, 0x8C,
                                      0xDB, 0x5C, 0x21, 0xB7, 0xBC, 0x65, 0xE0, 0x82,
                                      0xA6, 0x51, 0x3F, 0x42, 0x4A, 0x4B, 0x25, 0x2E,
                                      0x0D, 0x77, 0xFA, 0x40, 0x56, 0x98, 0x6A, 0x0A,
                                      0xB0, 0xCD, 0xA6, 0x15, 0x5E, 0xD9, 0xA8, 0x83,
                                      0xC6, 0x9C, 0xC2, 0x99, 0x2D, 0x49, 0xEC, 0xBD,
                                      0x47, 0x97, 0xDD, 0x28, 0x64, 0xFF, 0xC9, 0x6B,
                                      0x8D}, 0x01, {0x03, 0x00, 0x01}, 0x14,
                        {0xB3, 0xAE, 0x2B, 0xC3, 0xCA, 0xFC, 0x05, 0xEE, 0xEF, 0xAA, 0x46, 0xA2, 0xA4, 0x7E, 0xD5, 0x1D, 0xE6, 0x79, 0xF8, 0x23},
                        {0x31, 0x27, 0x01, 0x21}
                },
        };


/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:sdkIccUserTransInit
   ��������:�û��Զ����ʼ��EMV����
   �������:
   �������:
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��:2012.05.25 18:54:25   //20140414
*******************************************************************/
void sdkTestIccUserTransInitCb() {
    TestLog("emv", "\r\n----------sdkTestIccUserTransInitCb ---------------\r\n");
//    memcpy(TermInfo.TermCapab , "\xE0\xB0\xC8" , 3);
//    memcpy(TermInfo.TermAddCapab , "\x60\x00\xF0\xF0\x01" , 5);
//    memcpy(TermInfo.MerchCateCode , "\x00\x01" , 2);
//    memset(TermInfo.MerchID , 0 , 15);
//    TermInfo.TermType = 0x22;      //attended,operated by merchant,offline with online capability
//    memcpy(TermInfo.AppVer , "\x00\x8C" , 2); //VIS140-0x008C(140);VIS132-0x0084(132);VIS131-0x0083(131);
//    memcpy(TermInfo.CountryCode , "\x01\x56" , 2); //ʵ������156
//    memcpy(TermInfo.TransCurcyCode , "\x01\x56" , 2); //USD-0x0840;RMB-0x0156;KRW-0x0410
//    memcpy(TermInfo.TransReferCurcyCode , "\x01\x56" , 2); //USD-0x0840;RMB-0x0156;KRW-0x0410
//    TermInfo.TransCurcyExp = 0x02;      //USD,RMB,HKD-0x02;KRW-0x00
//    TermInfo.TransReferCurcyExp = 0x02;          //USD,RMB,HKD-0x02;KRW-0x00
//    TermInfo.bForceOnline = 0;
//    TermInfo.TermDDOLLen = 3;
//    memcpy(TermInfo.TermDDOL , "\x9F\x37\x04" , 3);        //according to VIS140.
//    TermInfo.TermTDOLLen = 15;
//    memcpy(TermInfo.TermTDOL , "\x9F\x02\x06\x5F\x2A\x02\x9A\x03\x9C\x01\x95\x05\x9F\x37\x04" , 15);
//    memcpy(TermInfo.CountryCode , "\x08\x40" , 2);        //set for match with ICC when testing.
//    memcpy(TermInfo.TransCurcyCode , "\x08\x40" , 2);        //USD-0x0840;RMB-0x0156;KRW-0x0410
//    memcpy(TermInfo.TransReferCurcyCode , "\x08\x40" , 2);        //USD-0x0840;RMB-0x0156;KRW-0x0410
//    ConfigTransType(CASH);
//    sdkIccConfig9C("\x01");
//    ConfigTransType(CASHBACK);
//    sdkIccConfig9C("\x09");
//    ConfigTransType(GOODS);
//    sdkIccConfig9C("\x00");
//    sdkIccConfigTLV("\x9C","\x00",1);
//    sdkIccConfigTLV("\x9F\x03","\x00\x00\x00\x00\x10\x00",6);
//    TestLogHex("emv" , "--TermInfo.TermCapab--" , TermInfo.TermCapab , 3);
//    TermInfo.TermCapab[1] |= 0x08;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:
   ��������:	��ʾ����
   �������: 1.����Ķ���ģʽ
   �������:
   ��   ��  ֵ: SDK_PARA_ERR
                        SDK_OK
   �޸ı�ע:
   ����ʱ��:2012.06.25 14:58:05   //20140414
*******************************************************************/
s32 sdkTestReadCardDisp(u8 ucCardModeEn) {
    u8 temp[64], text[64];

    if ((ucCardModeEn & (SDK_ICC_MAG | SDK_ICC_ICC | SDK_ICC_RF | SDK_ICC_MANUAL)) == 0) {
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    memset(text, 0, sizeof(text));
    TestLog("emv", "ucCardModeEn=%d\r\n", ucCardModeEn);

    strcpy(temp, "��");

    if (ucCardModeEn & SDK_ICC_MAG) {
        strcat(temp, "ˢ����");
    }

    if (ucCardModeEn & SDK_ICC_ICC) {
        strcat(temp, "�忨��");
    }

    if (ucCardModeEn & SDK_ICC_RF) {
        strcat(temp, "�ӿ���");
    }

    if (ucCardModeEn & SDK_ICC_MANUAL) {
        strcat(temp, "���䡢");
    }

    if (strlen(temp) > 20) //����ƣ���Ҫ����390
    {
        memcpy(text, &temp[strlen(temp) - 6], 6);
        temp[strlen(temp) - 6] = 0;
        memset(&text[4], 0, 2);
        text[4] = ':';
    }
    else {
        memset(&temp[strlen(temp) - 2], 0, 2);
        temp[strlen(temp)] = ':';
    }
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, temp, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, text, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestIccInputAmountCb
   ��������:	������
   �������:
   �������:
   ��   ��  ֵ:EMV_OK:�ɹ�
                    EMV_ERR:ʧ��
   �޸ı�ע:
   ����ʱ��:2012.05.23 19:05:43    //20140414
*******************************************************************/
s32 sdkTestIccInputAmountCb(u8 *amount) //sjz 20120726
{
    s32 retCode;
    u8 buf[64], amount_ascii[64];
    u32 i;


    memset(buf, 0, sizeof(buf));
    memset(amount_ascii, 0, sizeof(amount_ascii));
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��������:",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����밴[���]��",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    retCode = sdkKbGetScanf(SDK_ICC_TIMER_AUTO, buf, 1, 9, SDK_MMI_NUMBER | SDK_MMI_POINT,
                            SDK_DISP_LINE3);

    TestLog("emv", "sdkKbGetScanf retcode %d\r\n", retCode);

    if (retCode == SDK_KEY_ENTER) {
        TestLogHex("emv", "input amount_ASC:", &buf[1], buf[0]);
        i = buf[0];

        if (i > 9) {
            i = 9;
        }
        buf[1 + i] = 0;

#if 1
        memcpy(amount, buf + 1, buf[0] + 1); //���һ���ֽ�Ϊ������'\0'
#else

        for(j = 0; j < 12 - i; j++)
        {
            amount_ascii[j] = 0x30;
        }

        memcpy(&amount_ascii[j], &buf[1], i);
        amount_ascii[12] = 0;

        sdkAscToBcdR(amount, amount_ascii, 6); //sjl 2012.08.09 14:55
#endif

        //    sdkAscToBcdR(amount, amount_ascii, 6); //sjl 2012.08.09 14:55
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���Ժ�...",
                          SDK_DISP_FDISP | SDK_DISP_INCOL | SDK_DISP_LDISP);
        sdkDispBrushScreen();
    }
    TestLogHex("emv", "input amount:", amount, 6);

    if (retCode == SDK_KEY_ENTER) {
        retCode = SDK_OK;
    }
    else                                         // ȡ���������
    {
        retCode = SDK_ERR;
    }
    return retCode;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestIccDispSelAgainCb
   ��������:	���Ժ���:��ʾ����ѡ��
   �������:
   �������:
   ��   ��  ֵ:EMV_OK:�ɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.23 19:09:36  //20140414
*******************************************************************/
s32 sdkTestIccDispSelAgainCb() {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "������",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();

    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestIccDispCardOutCb
   ��������:	���Ժ���:��ʾ���ƿ���Ƭ
   �������:
   �������:
   ��   ��  ֵ:EMV_OK:�ɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.23 19:10:34   //20140414
*******************************************************************/
s32 sdkTestIccDispCardOutCb() {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�뽫��Ƭ�ƿ�",
                      SDK_DISP_FDISP | SDK_DISP_INCOL | SDK_DISP_LDISP);
    sdkDispBrushScreen();
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestIccDispCardNoCb
   ��������:	���Ժ���:��ʾ����
   �������:
   �������:
   ��   ��  ֵ:EMV_OK:�ɹ�
                    EMV_ERR:ȡ��
   �޸ı�ע:
   ����ʱ��:2012.05.23 19:11:19    //20140414
*******************************************************************/
s32 sdkTestIccDispCardNoCb(u8 const *pasPAN) {
    s32 key;
    s32 ret;

    //SDK_ICC_CARDDATA *CardData;

    //CardData = (SDK_ICC_CARDDATA *)emvb_malloc(sizeof(SDK_ICC_CARDDATA));

    //memset(CardData, 0, sizeof(SDK_ICC_CARDDATA));

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, pasPAN, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��ȷ��...",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();

    while (1) {
        key = sdkKbGetKey();

        if (key != 0) {
            if (key == SDK_KEY_ENTER) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                ret = SDK_OK;
                break;
            }
            else if (key == SDK_KEY_ESC) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                ret = SDK_ERR;
                break;
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }
    }


    return ret;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTesticcSendSaleBag
   ��������:	���Ժ���:�������ݰ�
   �������:
   �������:
   ��   ��  ֵ:EMV_OK:�ɹ�
                    EMV_ERR:�����в���ȡ��
   �޸ı�ע:
   ����ʱ��:2012.05.23 19:12:19   //20140414
*******************************************************************/
s32 sdkTesticcSendBagCb() {
    u8 OnlineResult;
    u8 RspCode_Ascii[2];
    u8 IssuerAuthData[] = {0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x2A, 0x2B, 0x30, 0x30};
    u8 Script[] =
            {
                    0x71, 0x16, 0x9F, 0x18, 0x04, 0x11, 0x22, 0x33,
                    0x44, 0x86, 0x0D, 0x84, 0x16, 0x00, 0x00, 0x08,
                    0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44,
                    0x71, 0x16, 0x9F, 0x18, 0x04, 0x44, 0x33, 0x22,
                    0x11, 0x86, 0x10, 0x84, 0x16, 0x00, 0x00, 0x08,
                    0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44,
                    0x72, 0x12, 0x9F, 0x18, 0x04, 0x55, 0x66, 0x77,
                    0x88, 0x86, 0x09, 0x84, 0x24, 0x00, 0x00, 0x04,
                    0xAA, 0xBB, 0xCC, 0xDD
            };
    u32 IssuerAuthDataLen, ScriptLen;

//������Լ�ȥ�飬���Ե���sdkIccReadTLV��ȡemv���׹����е��������
//����Ҳ���Լ�ȥ��,����ֻ���������ɹ���Ȼ������������·���emv�ں�
    OnlineResult = SDK_OK;
    memcpy(RspCode_Ascii, "00", 2);
    memset(IssuerAuthData, 0, sizeof(IssuerAuthData));
    IssuerAuthDataLen = sizeof(IssuerAuthData);
    memset(Script, 0, sizeof(Script));
    ScriptLen = sizeof(Script);

    memset(IssuerAuthData, 0, sizeof(IssuerAuthData));
    IssuerAuthDataLen = 0;
    memset(Script, 0, sizeof(Script));
    ScriptLen = 0;

//���������ݷ���emv�ں�
    Verify(sdkIccGetOnlineData(OnlineResult, RspCode_Ascii, IssuerAuthData, IssuerAuthDataLen,
                               Script, ScriptLen) == SDK_OK);

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestIccDispRandNum
   ��������:	���Ժ���:��ʾ�����
   �������:
   �������:
   ��   ��  ֵ:SDK_OK:�ɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.23 19:15:08   //20140414
*******************************************************************/
s32 sdkTestIccDispOnlineRandNumCb(u8 randNum) {
    char tempbuf[32];

    sdkDispClearScreen();
    sprintf(tempbuf, "rand num: %d", (int) randNum);
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, tempbuf, SDK_DISP_FDISP | SDK_DISP_INCOL | SDK_DISP_LDISP);
    sdkDispBrushScreen();
    sdkmSleep(100);
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:
   ��������:
   �������:
   �������:
   ��   ��  ֵ:SDK_OK
                        SDK_ESC
                        SDK_TIME_OUT
   �޸ı�ע:
   ����ʱ��:2012.06.05 09:09:28   //20140414
*******************************************************************/
s32 sdkTestIccChooseEcCb() {
    s32 key = 0;
    u32 time_ID;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�ÿ��߱������ֽ���",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�Ƿ�ʹ�õ����ֽ���:",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "1-��  0-��",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    time_ID = sdkTimerGetId();

    while (1) {
        key = sdkKbGetKey();

        if (key != 0) {
            if (key == SDK_KEY_1) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                return SDK_OK;
            }
            else if (key == SDK_KEY_0) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                return SDK_ESC;
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }

        if (sdkTimerIsEnd(time_ID, SDK_ICC_TIMER_AUTO)) {
            return SDK_TIME_OUT;
        }
    }
}

s32 sdkTestIccDispCandAppListCb(u8 ucCandAppNum, u8 **pheCandApp)  //20140414
{
    s32 key;
    u8 curPage;
    u8 totalPage;
    u8 i, j;
    u8 dispData[64];
    SDK_ICC_AIDLIST **pAidListTemp;

    TestLog("", "goto->sdkTestIccDispCandAppListCb,number = %d,address=%x\r\n", ucCandAppNum,
            pheCandApp);

    if (ucCandAppNum < 1 || pheCandApp == NULL) {
        return SDK_PARA_ERR;
    }
    pAidListTemp = (SDK_ICC_AIDLIST **) pheCandApp;
    totalPage = (ucCandAppNum - 1) / 4;
    curPage = 0;
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Ӧ��ѡ��",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

    for (i = 0; i < 4; i++) {
        j = (curPage * 4) + i;

        if (j >= ucCandAppNum) { break; }
        memset(dispData, 0, sizeof(dispData));
        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAppLabel);
        sdkDispFillRowRam(i + SDK_DISP_LINE2, 0, dispData,
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    }

    sdkDispBrushScreen();
    sdkKbKeyFlush();

    while (1) {
        key = sdkKbGetKey();

        if (key != 0) {
            switch (key) {
                case SDK_KEY_1:
                case SDK_KEY_2:
                case SDK_KEY_3:
                case SDK_KEY_4: {
                    if ((curPage * 4 + key - '0') > ucCandAppNum) {
                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                    }
                    else {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        sdkDispClearScreen();
                        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���Ժ�...",
                                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                        sdkDispBrushScreen();
                        return (s32)(curPage * 4 + key - '0');
                    }
                }
                    break;

                case SDK_KEY_ESC: {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    sdkDispClearScreen();
                    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���Ժ�...",
                                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                    sdkDispBrushScreen();
                    return SDK_ESC;
                }

                case SDK_KEY_UP:                           //�Ϸ�
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);

                    if (curPage > 0) {
                        curPage--;
                    }
                    sdkDispClearScreen();
                    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Ӧ��ѡ��",
                                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

                    for (i = 0; i < 4; i++) {
                        j = (curPage * 4) + i;

                        if (j >= ucCandAppNum) { break; }
                        memset(dispData, 0, sizeof(dispData));
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAid);
                        sdkDispFillRowRam(i + SDK_DISP_LINE2, 0, dispData,
                                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                    }

                    sdkDispBrushScreen();
                }
                    break;

                case SDK_KEY_DOWN:                      //�·�
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);

                    if (curPage < totalPage) {
                        curPage++;
                    }
                    sdkDispClearScreen();
                    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Ӧ��ѡ��",
                                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

                    for (i = 0; i < 4; i++) {
                        j = (curPage * 4) + i;

                        if (j >= ucCandAppNum) { break; }
                        memset(dispData, 0, sizeof(dispData));
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAid);
                        sdkDispFillRowRam(i + SDK_DISP_LINE2, 0, dispData,
                                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                    }

                    sdkDispBrushScreen();
                }
                    break;

                default: {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
                    break;
            }
        }
    }
}

s32 sdkTestIccDispPromptDataCb(u8 ePromptType) //20140414
{
    u8 *value = NULL;
    s32 value_size = 256;
    s32 len = 0;

    TestLog("", "-------------goto->sdkTestIccDispPromptDataCb, input=%d\r\n", ePromptType);
    value = (u8 *) sdkGetMem(value_size);

    if (NULL == value) {
        Assert(0);
        return SDK_ERR;
    }
    memset(value, 0, value_size);

    switch (ePromptType) {
        case SDK_ICC_WAIT:
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���Ժ�...",
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
            break;

        case SDK_ICC_OFFLINE_PIN_ERR:
            sdkSysBeep(SDK_SYS_BEEP_ERR);
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "PIN�������!",
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
            sdkmSleep(1000);
            break;

        case SDK_ICC_OFFLINE_PIN_SUC:
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������ȷ",
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
            sdkmSleep(1000);
            break;

        case SDK_ICC_INPUT_PIN_AGAIN:
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "PIN�������!",
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "������",
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            memset(value, 0, value_size);

            if (SDK_OK == sdkIccReadTLV("\x9F\x17", value, &len)) {
                if (1 == value[0]) {
                    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "( ���һ�� )",
                                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                }
            }
            sdkDispBrushScreen();
            sdkKbWaitKey(SDK_KEY_MASK_ALL, 1500);
            break;
    }

    sdkFreeMem(value);
    value = NULL;

    return SDK_OK;
}

#if 1

/*****************************************************************************
** Descriptions:   input pin
** Parameters:     const u8 *pTradeAmount
                                        u8 PINTryCount
                                        u8 IccEncryptWay
                                        u8 *pCreditPwd
** Returned value:
** Created By:		sujianzhong 20121101
** Remarks:  //20140414
*****************************************************************************/
s32 sdkTestInputCreditPwd(const u8 *pasTradeAmount, u8 PINTryCount, u8 IccEncryptWay,
                          u8 *pCreditPwd) {
    SDK_PED_PIN_CFG PinCfg;
    u8 bcAmount[6] = {0}; //sujianzhong 2013.01.10 14:25
    u8 *temp = NULL, row = SDK_DISP_LINE3;
    s32 temp_size = 256;
    u8 tempCurFifoNo;
    SDK_ICC_CARDDATA *cardData;
    s32 cardNoLen, ret;

    tempCurFifoNo = sdkSysGetCurFifoNo();
    cardData = (SDK_ICC_CARDDATA *) emvb_malloc(sizeof(SDK_ICC_CARDDATA));
    memset(cardData, 0, sizeof(SDK_ICC_CARDDATA));
    sdkIccGetCardData(cardData); //��ȡ�Ѷ����Ŀ�����Ϣ
    cardNoLen = (s32) cardData->ucCardLen; //sujianzhong 2013.07.16
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "  ���:",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return SDK_ERR;
    }
    memset(temp, 0, temp_size);

    if (pasTradeAmount != NULL) //sujianzhong 2013.01.10 14:26
    {
        Verify(sdkAscToBcdR(bcAmount, pasTradeAmount, 6) >= 0);
        TestLogHex("sdkTestInputCreditPwd", "bcAmount\r\n", bcAmount, 6);
    }
    sdkRegulateAmount(temp, bcAmount);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, temp, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);

    if (IccEncryptWay == ICCOFFLINEPIN) {
        sdkDispFillRowRam(row, 0, "�������ѻ�����",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

        if (PINTryCount == 1) {
            sdkDispFillRowRam(row + 1, 0, "��ֻ�����һ�λ���",
                              SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
        }
    }
    else {
        sdkDispFillRowRam(row, 0, "��ֿ�����������",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    }
    Trace("emv", "Input Password Reminded.\r\nsdkDispBrushScreen();\r\n");
    sdkDispBrushScreen();
    Trace("emv", "memset(&PinCfg, 0, %d);\r\n", sizeof(SDK_PED_PIN_CFG));
    memset(&PinCfg, 0, sizeof(SDK_PED_PIN_CFG));

    if (ICCONLINEPIN == IccEncryptWay) {
        Trace("emv", "ICCONLINEPIN == IccEncryptWay; PinCfg.ePinMode = SDK_PED_IC_ONLINE_PIN;\r\n");
        PinCfg.ePinMode = SDK_PED_IC_ONLINE_PIN;
    }
    else {
        Trace("emv", "PinCfg.ePinMode = SDK_PED_IC_OFFLINE_PIN;\r\n");
        PinCfg.ePinMode = SDK_PED_IC_OFFLINE_PIN;
    }
    PinCfg.eKeyType = SDK_PED_DES_SINGLE;

//    PinCfg.ucTpkIndex = TPK_STORENO + ((tempCurFifoNo) % APPNUM) * 2;
    PinCfg.ucTpkIndex = 0;

    Trace("emv", "cardNoLen = %d;\r\n", cardNoLen);

    if (cardNoLen >= 13) {
        Trace("emv", "sdkAscToBcd(PinCfg.hePan + 2, cardData->asCardNO + cardNoLen - 13, 12);\r\n");
        sdkAscToBcd(PinCfg.hePan + 2, cardData->asCardNO + cardNoLen - 13, 12);
    }
    PinCfg.ucRow = SDK_DISP_LINE5;
    PinCfg.iTimeOut = SDK_ICC_TIMER_AUTO;
    Trace("emv", "ret = sdkPEDInputPIN(&PinCfg, bcAmount, pCreditPwd);\r\n");
    ret = sdkPEDInputPIN(&PinCfg, bcAmount, pCreditPwd);
    Trace("emv", "ret = %d;\r\n", ret);
//    {
//        int i;
//        u8 buf[64]={0};
//        sprintf(buf, "�������뷵��ֵ:%x",ret);
//        sdkDispClearScreen();
//        sdkDispFillRowRam(SDK_DISP_LINE1, 0, buf, SDK_DISP_LEFT_DEFAULT);
//        memset(buf,0,sizeof(buf));
//        sprintf(buf, "PINLEN:%02x",pCreditPwd[0]);
//        sdkDispFillRowRam(SDK_DISP_LINE2, 0, buf, SDK_DISP_LEFT_DEFAULT);
//        memset(buf,0,sizeof(buf));
//        sprintf(buf, "PIN:");
//        for(i=0;i < 8; i++) sprintf(buf+i*2+4, "%02x",pCreditPwd[i+1]);
//        sdkDispFillRowRam(SDK_DISP_LINE3, 0, buf, SDK_DISP_LEFT_DEFAULT);
//        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��������˳�!", SDK_DISP_LEFT_DEFAULT);
//        sdkDispBrushScreen();
//        sdkKbWaitKey(SDK_KEY_MASK_ALL, SDK_ICC_TIMER_AUTO);
//    }
    TestLog("sdkTestInputCreditPwd", "sdkPEDInputPIN ret=%d, pcreditpwd=%d\r\n", ret,
            pCreditPwd[0]);
    TestLogHex("sdkTestInputCreditPwd", "creditpwd", pCreditPwd + 1, pCreditPwd[0]);
//    sdkPEDCancel();
    emvb_free(cardData);

    sdkFreeMem(temp);
    temp = NULL;

    return ret;
}

#endif

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestIccDispText
   ��������:	���Ժ���:��ʾ�ı�
   �������:
   �������:
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��:2012.05.23 19:24:32  //20140414
*******************************************************************/
void sdkTestIccDispText(u8 const *Text) {
    sdkSysBeep(SDK_SYS_BEEP_OK);
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, Text, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��������˳�",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 200 * 100);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestIccGetCardModeEn
   ��������:	���Ի�ȡ����Ķ���ģʽ
   �������:
   �������:
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��:2012.05.30 16:56:35  //20140414
*******************************************************************/
s32 sdkTestIccGetCardModeEn(u8 *cardModeEn) {
    u32 ret = SDK_OK;

    *cardModeEn = SDK_ICC_MAG | SDK_ICC_ICC | SDK_ICC_RF | SDK_ICC_MANUAL;
    return ret;
}

/*****************************************************************************
** Descriptions:    deal QPBOC return value
** Parameters:      s32 ret
** Returned value:
** Created By:      sujianzhong   2012.11.01
** Remarks:   //20140414
*****************************************************************************/
void sdkTestIccDealqPbocRet(s32 ret) {
    switch (ret) {
        case SDK_TransOfflineApprove:
            sdkTestIccDispText("�ѻ���׼");
            break;

        case SDK_OnlineWait:
            sdkTestIccDispText("�����ȴ�");
            break;

        case SDK_TransOfflineDeclineErr:
            sdkTestIccDispText("���ױ��ܾ�");
            break;

//         case qPBOC_DDA_AUTH_FAILURE:
//           sdkTestIccDispText("DDA��֤ʧ��");
//           break;

        case SDK_QpbocNoCard:
            sdkTestIccDispText("δ��ʾIC��");
            break;

        case SDK_QpbocMoreCards:
            sdkTestIccDispText("���ſ�");
            break;

        case SDK_QpbocAuthNoPerformed:
            sdkTestIccDispText("δ����˫����֤");
            break;

        case SDK_ECNotSupportErr:
            sdkTestIccDispText("��֧�ִ������ֽ�");
            break;

        case SDK_EcLackBalanceErr:
            sdkTestIccDispText("����,�������ֽ�");
            break;

        case SDK_UsePbocErr:
            sdkTestIccDispText("��ʹ��PBOC����");
            break;

        default:
            sdkTestIccDispText("����ʧ��");
            break;
    }
}

/*****************************************************************************
** Descriptions:    test "read card"
** Parameters:      u8 ucCardModeEn
                    SDK_ICC_TRADE_PARAM  *pstIccTradeParam
                    SDK_ICC_PARAM *pstIccParam
                    SDK_ICC_CARDDATA *pstCardData
                    const u32 eDispRow
** Returned value:
** Created By:      sujianzhong   2012.11.01
** Remarks:  //20140107   //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/
s32 sdkTestIccReadCard(u8 ucCardModeEn, SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                       SDK_ICC_PARAM *pstIccParam, SDK_ICC_CARDDATA *pstCardData,
                       const u32 eDispRow) {
    u8 heUID[64] = {0}; //heUID[0]:UID�ĳ��ȣ�heUID[1~x]:ΪUID������
    u8 temp[64] = {0};
    u8 handLen = 0, handData[128] = {0};
    SDK_SYS_BEEP_TYPE flag;
    s32 key;
    u32 timerID;
    s32 rslt;

//emv������ʼ������sdkIccGetCardStatus��sdkIccPowerOnAndSeek֮ǰ��ʼ��������Ϊ�����п���
//���ı䣬�����������������һ�£������һ�£����ϵ�ǰ���¶Զ�������ֵ
    if (pstIccTradeParam != NULL) {
        Trace("emv", "sdkIccTransInit(pstIccTradeParam);\r\n");

        if (sdkIccTransInit(pstIccTradeParam) != SDK_OK) {
            return SDK_ERR;
        }
    }

    if (ucCardModeEn & SDK_ICC_MAG) {
        sdkIccOpenMag();
    }

    /*=======BEGIN: ���ٲ� 2014.03.23  10:48 add===========*/
    if (ucCardModeEn & SDK_ICC_ICC) {
        Trace("emv", "sdkIccOpenIcDev();\r\n");
        sdkIccOpenIcDev();
    }

    if (ucCardModeEn & SDK_ICC_RF) {
        sdkIccOpenRfDev();
    }

    /*====================== END======================== */
    if (ucCardModeEn & SDK_ICC_MANUAL) {
        sdkDispClearRow(eDispRow);
        sdkDispFillRowRam(eDispRow, 0, "_", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    }
    timerID = sdkTimerGetId();

    while (1) {
        if (ucCardModeEn & SDK_ICC_MAG) {
            Trace("emv", "rslt = sdkIccGetMagData(pstIccParam, pstCardData);\r\n");
            rslt = sdkIccGetMagData(pstIccParam, pstCardData);

            if (rslt == SDK_OK) //�ɹ�����
            {
                TestLogHex("emv", "pstCardData", pstCardData, sizeof(SDK_ICC_CARDDATA));
                return rslt;
            }
            else if (rslt == SDK_ICC_BRUSH_ERR) //�ɼ�������
            {
                sdkDispClearRow(SDK_DISP_LINE2);
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "ˢ������!",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
            }
            else if (rslt != SDK_ICC_NOCARD) //ʧ���˳�
            {
                sdkTestIccDispText("����ʧ��!");
                return SDK_ERR;
            }
        }

        if (ucCardModeEn & SDK_ICC_ICC) {
            TestLog("emv", "test begin rslt = %d\r\n", rslt);
            rslt = sdkIccGetCardStatus(pstIccParam, 100);
            TestLog("emv", "tset rslt = %d\r\n", rslt);

            if (rslt == SDK_OK) //�ɹ�����
            {
                return rslt;
            }
            else if (rslt != SDK_ICC_NOCARD) //ʧ���˳�
            {
                sdkTestIccDispText("����ʧ��!");
                return SDK_ERR;
            }
        }

        if (ucCardModeEn & SDK_ICC_RF) //�ǽӶ���
        {
            rslt = sdkIccPowerOnAndSeek(pstIccTradeParam, pstIccParam, heUID);
            TestLog("emv", "\r\nsdkIccPowerOnAndSeek ret = %d \r\n", rslt);

            if (rslt == SDK_OK) {
                return rslt;
            }
            else if (rslt != SDK_ICC_NOCARD && rslt != SDK_ICC_MORECARD)    //�޿���࿨������Ѱ��
            {
                sdkTestIccDispText("����ʧ��!");
                return SDK_ERR;
            }
        }
        key = sdkKbGetKey();

        if (ucCardModeEn & SDK_ICC_MANUAL) //���俨��
        {
            if (key != 0) {
                flag = SDK_SYS_BEEP_ERR;

                if (key == SDK_KEY_ENTER) {
                    if (handLen >= 1) {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        pstIccParam->ucCardMode = SDK_ICC_MANUAL;
                        sprintf(&pstCardData->ucCardLen, "%d", handLen);
                        memcpy(pstCardData->asCardNO, handData, handLen);
                        pstCardData->asCardNO[handLen] = 0;
                        TestLogHex("emv", "���俨��", pstCardData, sizeof(SDK_ICC_CARDDATA));
                        return SDK_OK;
                    }
                }
                else if (key == SDK_KEY_CLEAR || key == SDK_KEY_BACKSPACE ||
                         (key >= SDK_KEY_0 && key <= SDK_KEY_9)) {
                    if (key == SDK_KEY_CLEAR) {
                        flag = SDK_SYS_BEEP_OK;
                        handLen = 0;
                        memset(handData, 0, sizeof(handData));
                    }
                    else if (key == SDK_KEY_BACKSPACE) {
                        if (handLen > 0) {
                            flag = SDK_SYS_BEEP_OK;
                            handData[handLen] = 0;
                            handLen--;
                        }
                    }
                    else if (key >= SDK_KEY_0 && key <= SDK_KEY_9) {
                        if (handLen < 19) {
                            flag = SDK_SYS_BEEP_OK;
                            handData[handLen++] = (u8) key;
                        }
                    }
                    memset(temp, 0, sizeof(temp));

                    if (handLen > 0) {
                        memcpy(temp, handData, handLen);
                    }
                    else {
                        temp[0] = '_';
                    }
                    sdkDispClearRow(eDispRow);
                    sdkDispFillRowRam(eDispRow, 0, temp,
                                      SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
                    sdkDispBrushScreen();
                }
                sdkSysBeep(flag);
            }
        }

        if (key == SDK_KEY_ESC) {
            pstIccParam->ucCardMode = 0;
            return SDK_ESC;
        }

        if (1 == sdkTimerIsEnd(timerID, SDK_ICC_TIMER_AUTO)) {
            TestLog("emv", "\r\n $$$$$$$$time out%%%%%%%%% \r\n", rslt);
            return SDK_TIME_OUT;
        }
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestGoToSelectApp ��ͬ��sdkIccGoToSelectApp
   ��������:	ȥѡ��Ӧ��
   �������:stFixedAid:ѡ��̶�AID
                        pstIccParam:������
   �������:
   ��   ��  ֵ:SDK_FixedAidAbsentErr:ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
                        SDK_OK:�ɹ�
                        SDK_IccReturnErr:���󷵻�
                        SDK_CancelTransErr:ȡ������
                        SDK_ERR:����ʧ��
                        SDK_PARA_ERR
                        SDK_AppSelectTryAgainErr����ѡ��AID
                        SDK_IccCommandErr:����ʧ��
                       SDK_UsePbocErr:��ʹ��PBOC����
                       SDK_AppBlockErr
                       SDK_NoAppSelErr
   �޸ı�ע:
   ����ʱ��:2012.06.08 16:04:10   //20140414  �Ȳ��ͷŶ�̬�������Դ
*******************************************************************/
s32 sdkTestGoToSelectApp(SDK_ICC_FIXED_AID stFixedAid, const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                         const SDK_ICC_PARAM *pstIccParam)   //sjz 20120726
{
    s32 rlt;

    rlt = sdkIccOrganizeAppList(stFixedAid, pstIccTradeParam,
                                pstIccParam);                                                 //��֯Ӧ��ѡ���б�

    if (SDK_FixedAidAbsentErr ==
        rlt)                                               //ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
    {
        return SDK_FixedAidAbsentErr;
    }
    else if (rlt == SDK_UserSelect) {
        rlt = sdkIccUserSelectApp(pstIccTradeParam,
                                  pstIccParam);                                               //�û�ѡ��Ӧ��
    }

    if (rlt != SDK_OK) {
        return rlt;
    }
    return sdkIccFinalSelectedApp(pstIccTradeParam, pstIccParam);
}

/*******************************************************************
   ��          ��:�ս���
   ��������:	sdkTestGoToInitialApp��ͬ��sdkIccGoToInitialApp
   ��������:	ȥ��ʼ��Ӧ��
   �������:pstIccTradeParam:IC�����ײ���
                    pstIccParam:������
   �������:
   ��   ��  ֵ:1.SDK_NeedMagCardErr: FALLBACK
                      2.SDK_AppSelectTryAgainErr��ѡ��AID
                      3.SDK_OK:�ɹ�,EMV/PBOC��������
                      4.SDK_IccCommandErr ����ʧ��
                      5.SDK_IccDataFormatErr  ���ݸ�ʽ��
                      6.SDK_IccReturnErr �������ݴ�
                      7.SDK_ERR:ʧ��
                      8.SDK_ReadECBalance:ȥ�������ֽ����
                      9.SDK_OnlineWait:qPBOC��������
                      10.SDK_OfflineApprove:qPBOC�ѻ���׼����
                      11.SDK_OfflineDecline:qPBOC�ѻ��ܾ�����
                      12.SDK_UsePbocErr:��ʹ��PBOC����
                      SDK_PARA_ERR
                      SDK_ECNotSupportErr:�������ֽ𿨲�֧�ָù���
                      SDK_EcLackBalanceErr:�����ֽ�����
                      SDK_OfflineDecline:�ѻ��ܾ�
                      SDK_TransOnlineApprove:������׼
                      SDK_TransOnlineDeclineErr:�����ܾ�
                      SDK_TransOfflineApprove:�ѻ���׼
                      SDK_TransOfflineDeclineErr:�ѻ��ܾ�
                      SDK_TransNotAcceptErr:���ײ�����
                      SDK_TransTerminateErr:����ʧ��
   �޸ı�ע:
   ����ʱ��:2012.06.08 16:07:37   //20140414  �Ȳ��ͷŶ�̬�������Դ
*******************************************************************/
s32 sdkTestGoToInitialApp(SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                          const SDK_ICC_PARAM *pstIccParam)  //sjz 20120726
{
    s32 ret;
    u8 bcAmount[128] = {0};

    TestLogHex("emv", "asamount:", pstIccTradeParam->pasTradeAmount, 12);

    if ((NULL != pstIccTradeParam->InputAmount) && (NULL != pstIccTradeParam->pasTradeAmount)
        && (strlen(pstIccTradeParam->pasTradeAmount) == 0)) //sujianzhong 2013.01.10 14:26
    {
        ret = (*pstIccTradeParam->InputAmount)(
                pstIccTradeParam->pasTradeAmount); //sujianzhong 2013.01.10 14:24
        TestLog("emv", "pstIccTradeParam->InputAmount  return value = %d\r\n", ret);

        if (strlen(pstIccTradeParam->pasTradeAmount) <= 12) {
            Verify(sdkAscToBcdR(bcAmount, pstIccTradeParam->pasTradeAmount, 6) >= 0);
        }
        else {
            TestLogHex("emv", "���׽���42949672.95Ԫ(�޷�������)", pstIccTradeParam->pasTradeAmount,
                       strlen(pstIccTradeParam->pasTradeAmount));
            return SDK_ERR;
        }

        if (ret != SDK_OK) {
            return ret;
        }
        else if (memcmp(bcAmount, "\x00\x42\x94\x96\x72\x95", 6) > 0) {
            TestLogHex("emv", "���׽���42949672.95Ԫ(�޷�������)", pstIccTradeParam->pasTradeAmount,
                       strlen(pstIccTradeParam->pasTradeAmount));
            return SDK_ERR;
        }
    }
    ret = sdkIccInitialApp(pstIccTradeParam, (SDK_ICC_PARAM *) pstIccParam);             //��ʼ��Ӧ��
    TestLog("emv", "sdkIccInitialApp  return value = %d\r\n", ret);

    if (ret ==
        SDK_NeedMagCardErr)                                       //GPO����"94 81"����fallback����
    {
        if (pstIccTradeParam->bIsFallback) {
            return SDK_NeedMagCardErr;
        }
        else {
            return SDK_ERR;
        }
    }
    else if ((SDK_ReadECBalance == ret) ||
             (SDK_OfflineDecline == ret)) // �������ֽ�ARQC��AAC,ȥ�е����ֽ����
    {
        ret = sdkIccReadEcData(pstIccTradeParam,
                               pstIccParam);                                                   //�������ֽ����
        TestLog("emv", "sdkIccReadEcData return value = %d\r\n", ret);

        if (SDK_OfflineDecline == ret) {
            return sdkIccTransComplete();                                                   //�������
        }
    }
    return ret;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestDealQpbocOnline��ͬ��sdkIccDealQpbocOnline
   ��������:	QPBOC��������
   �������:
   �������:
   ��   ��  ֵ:SDK_PARA_ERR
                      SDK_ERR
                     SDK_TransOnlineApprove:������׼
                     SDK_TransOnlineDeclineErr:�����ܾ�
                     SDK_TransOfflineApprove:�ѻ���׼
                     SDK_TransOfflineDeclineErr:�ѻ��ܾ�
                     SDK_TransNotAcceptErr:���ײ�����
                     SDK_TransTerminateErr:����ʧ��
                      SDK_CancelTransErr  ȡ������
                      SDK_IccCommandErr ��������
                      SDK_IccReturnErr ��Ƭ���ش���
                      SDK_AppTimeOutErr �������ʱ

   �޸ı�ע:
   ����ʱ��:2012.06.06 21:20:52   //20140414  �Ȳ��ͷŶ�̬�������Դ
*******************************************************************/
s32 sdkTestDealQpbocOnline(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                           const SDK_ICC_PARAM *pstIccParam)      //sjz 20120726
{
    s32 ret;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam)) {
        return SDK_PARA_ERR;
    }
    TestLog("emv", " sdkIccDealQpbocOnline\r\n");

    if (NULL != pstIccTradeParam->DispCardOut) {
        ret = (*pstIccTradeParam->DispCardOut)();                   // ��ʾ�ɽ���Ƭ�ƿ�

        if (ret != SDK_OK) {
            return ret;
        }
    }
    return sdkIccCardHolderVerf(pstIccTradeParam, pstIccParam);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkTestDealQpbocOffline��ͬ��sdkIccDealQpbocOffline
   ��������:	QPBOC�ѻ�����
   �������:
   �������:
   ��   ��  ֵ:SDK_PARA_ERR
                      SDK_ERR
                     SDK_TransOnlineApprove:������׼
                     SDK_TransOnlineDeclineErr:�����ܾ�
                     SDK_TransOfflineApprove:�ѻ���׼
                     SDK_TransOfflineDeclineErr:�ѻ��ܾ�
                     SDK_TransNotAcceptErr:���ײ�����
                     SDK_TransTerminateErr:����ʧ��
                      SDK_IccCommandErr ����ʧ��
                      SDK_AppTimeOutErr �������ʱ
                      SDK_IccDataRedundErr �����ظ�
                      SDK_IccDataFormatErr ���ݸ�ʽ����
                      SDK_IccReturnErr ������ʧ��

   �޸ı�ע:
   ����ʱ��:2012.06.06 21:18:40   //20140414  �Ȳ��ͷŶ�̬�������Դ
*******************************************************************/
s32 sdkTestDealQpbocOffline(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                            const SDK_ICC_PARAM *pstIccParam)       //sjz 20120726
{
    s32 ret;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam)) {
        return SDK_PARA_ERR;
    }
    TestLog("emv", " qpboc_offline_trade\r\n");
    ret = sdkIccReadAppData(pstIccTradeParam, pstIccParam);
    TestLog("emv", "sdkIccReadAppData return value = %d\r\n", ret);

    if (SDK_QpbocReadLastLogErr == ret) {
        return SDK_TransOfflineDeclineErr;
    }
    else if ((SDK_OfflineApprove != ret) && (SDK_OnlineWait != ret)) {
        if (ret == SDK_OK) {
            ret = SDK_ERR;
        }
        return ret;
    }

    if (NULL != pstIccTradeParam->DispCardOut) {
        ret = (*pstIccTradeParam->DispCardOut)();                   // ��ʾ�ɽ���Ƭ�ƿ�

        if (ret != SDK_OK) {
            return ret;
        }
    }
    ret = sdkIccCardHolderVerf(pstIccTradeParam, pstIccParam);
    TestLog("emv", "sdkIccCardHolderVerf return value = %d\r\n", ret);

    if (SDK_OfflineApprove != ret) {
        return ret;
    }
    ret = sdkIccProcessRestrict(pstIccTradeParam, pstIccParam);
    TestLog("emv", "sdkIccProcessRestrict return value = %d\r\n", ret);

    if (SDK_AppEffectErr == ret || SDK_AppExpiredErr == ret) {
        return SDK_TransOfflineDeclineErr;
    }
    else if (SDK_OfflineApprove != ret) {
        return ret;
    }
    ret = sdkIccDataAuth(pstIccTradeParam, pstIccParam);
    TestLog("emv", "sdkIccDataAuth return value = %d\r\n", ret);

    if (SDK_OfflineApprove == ret) {
        return SDK_TransOfflineApprove; //qpboc�ѻ���׼�ɹ�
    }
    else if (SDK_OfflineDecline == ret) {
        return SDK_TransOfflineDeclineErr; //qpboc�ѻ��ܾ�
    }
        //==================qpboc�ѻ�������֤ʧ�ܣ�ת��������============
    else {
        return ret; //qpboc�ѻ�������֤ʧ�ܣ�ת��������
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OK
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_AppTimeOutErr
                                SDK_IccDataRedundErr
                                SDK_IccDataFormatErr
                                SDK_IccReturnErr
                                SDK_PARA_ERR
                                SDK_CancelTransErr
                                SDK_TransOnlineApprove
                                SDK_TransOnlineDeclineErr
                                SDK_TransOfflineApprove
                                SDK_TransOfflineDeclineErr
                                SDK_TransNotAcceptErr
                                SDK_TransTerminateErr
                                SDK_CDAFormatErr:
                                SDK_IccEasyFlow
                                SDK_NeedMagCardErr
                                SDK_ECNotSupportErr
                                SDK_EcLackBalanceErr
** Created By:		sujianzhong2012.08.09
** Remarks:               //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/
s32 sdkTestDealEmvFlow(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                       const SDK_ICC_PARAM *pstIccParam)      //sjz 20120726
{
    u8 onlineRandNum;
    //   u8 asPAN[128] = {0};
    s32 ret;
    SDK_ICC_CARDDATA *carddata;


    TestLog("emv", "\r\n Is  EMV or PBOC  trade\r\n");
    //==============����ΪEMV/PBOC����=========================
    ret = sdkIccReadAppData(pstIccTradeParam,
                            pstIccParam);                                           //��Ӧ��
    TestLog("emv", "sdkIccReadAppData return value = %d\r\n", ret);

    if (SDK_ReadECBalance ==
        ret)                                           //  �������ֽ�û�з�����Ȩ��,����,��Ҫȥ�����
    {
        ret = sdkIccReadEcData(pstIccTradeParam,
                               pstIccParam);                                           //�������ֽ����
        TestLog("emv", "sdkIccReadEcData return value = %d\r\n", ret);

        if (SDK_OfflineDecline == ret) {
            return sdkIccTransComplete();                                           //�������
        }
        else if (SDK_OK == ret) //sujianzhong 2013.02.23 16:5
        {
            return SDK_ECNotSupportErr;
        }
        else {
            return ret;
        }
    }
    else if (SDK_OK != ret) {
        return ret;
    }

    if (NULL != pstIccTradeParam->VerifyCardNo)     //sjz �ȽϺ�����
    {
        carddata = (SDK_ICC_CARDDATA *) emvb_malloc(sizeof(SDK_ICC_CARDDATA));
        memset(carddata, 0, sizeof(SDK_ICC_CARDDATA));

        if (SDK_OK != sdkIccGetCardData(carddata)) {
            memset(carddata, 0, sizeof(SDK_ICC_CARDDATA));
        }
        TestLogHex("emv", "����", carddata->asCardNO, atoi(&carddata->ucCardLen));
        ret = (*pstIccTradeParam->VerifyCardNo)(
                carddata->asCardNO);                                       // ȷ�Ͽ���
        TestLog("emv", "pstIccTradeParam->VerifyCardNo return value = %d\r\n", ret);
        emvb_free(carddata);


        if (ret != SDK_OK) {
            return ret;
        }
    }

    if (pstIccTradeParam->eFlowMode ==
        SDK_ICC_EASY_FLOW)                                           //������
    {
        return SDK_IccEasyFlow;
    }
//    if(pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW &&
//       memcmp(pstIccTradeParam->bcTradeAmount, "\x00\x00\x00\x00\x00\x00", 6) == 0)
//    { //�ж��Ƿ�������
//        sdkIccDispNoAmount();
//        return SDK_ERR;
//    }
    ret = sdkIccDataAuth(pstIccTradeParam,
                         pstIccParam);                                            //��̬������֤

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccProcessRestrict(pstIccTradeParam,
                                pstIccParam);                                            //��������

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccCardHolderVerf(pstIccTradeParam,
                               pstIccParam);                                            //�ֿ�����֤

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccTermRiskManage(pstIccTradeParam,
                               pstIccParam);                                            //�ն˷��չ���

    if (SDK_ShowRandNum == ret) {
        sdkIccGetOnlineRandNum(&onlineRandNum);

        if (NULL != pstIccTradeParam->DispOnlineRandNum) {
            ret = (*pstIccTradeParam->DispOnlineRandNum)(onlineRandNum);             // ��ʾ�����

            if (ret != SDK_OK) {
                return ret;
            }
        }
    }
    else if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccTermiAnalys(pstIccTradeParam,
                            pstIccParam);                                           //�ն���Ϊ����

    if (SDK_NeedMagCardErr == ret) {
        if (pstIccTradeParam->bIsFallback) {
            return SDK_NeedMagCardErr;
        }
        else {
            return SDK_ERR;
        }
    }
    else if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccTransAnalys(pstIccTradeParam, pstIccParam); //���׷���

    if (ret != SDK_OK) //��������SDK_OnlineWait
    {
        return ret;
    }
    return sdkIccTransComplete();                                               //�������
}

/*****************************************************************************
** Descriptions:sdkTestTransFlow1��ͬ��sdkIccTransFlow1
** Parameters:          SDK_ICC_TRADE_PARAM * pstIccTradeParam
                                const SDK_ICC_PARAM * pstIccParam
** Returned value:	SDK_NeedMagCardErr ʹ��fallback����
                                SDK_ERR ����ʧ��
                                SDK_OnlineWait:�����ȴ�(�ȴ�����)
                                SDK_TransOfflineApprove �ѻ���׼(�ѻ��ɹ�)
                                SDK_TransOfflineDeclineErr �ѻ��ܾ�(��������)
                                SDK_TransTerminateErr ��ֹ����
                                SDK_IccCommandErr ����ʧ��
                                SDK_CardBlockErr ��������
                                SDK_IccDataFormatErr ���ݸ�ʽ��
                                SDK_CancelTransErr ȡ������
                                SDK_NoAppSelErr û��Ӧ��
                                SDK_AppTimeOutErr:��ʱ
                                SDK_IccDataRedundErr:�������ݴ�
                                SDK_CDAFormatErr :CDA��ʽ��
                                SDK_IccReturnErr ������ʧ��
                                SDK_EcLackBalanceErr:�����ֽ�����
                                SDK_IccEasyFlow:ֻ��ȡ���ʺţ��߼�����
                                SDK_DispLog:��ʾ������ϸ
                                SDK_OK:��������
                                SDK_UsePbocErr:��ʹ��PBOC
                                SDK_PARA_ERR:��������
                                SDK_ICC_RESET_ERR:��λʧ��
                                SDK_ECNotSupportErr:  �������ֽ𿨲�֧�ָù���


** Created By:		sujianzhong2012.07.30
** Remarks:             //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/
s32 sdkTestTransFlow1(SDK_ICC_TRADE_PARAM *pstIccTradeParam, const SDK_ICC_PARAM *pstIccParam) {
    u8 *OutBuf = NULL;
    s32 OutBuf_size = 300;
    u32 OutLen = 0;
    s32 ret;
    SDK_ICC_FIXED_AID FixedAid;

    TestLog("emv", "goto->sdkTestTransFlow1 \r\n");

    if ((pstIccTradeParam == NULL) || (pstIccParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if ((SDK_ICC_RF != pstIccParam->ucCardMode) && (SDK_ICC_ICC !=
                                                    pstIccParam->ucCardMode))                                           //����IC��
    {
        return SDK_ERR;
    }

    if (pstIccParam->ucCLType == 0x02) //�Ѿ�ȷ������qpboc����ʱ��ض�����stm32�����
    {
        return sdkIccDealQReaderTrade(pstIccTradeParam); //���������״���
    }

    if (SDK_OK != sdkIccTransInit(
            pstIccTradeParam))                 //��ʼ������                          //jiguang 20120519
    {
        return SDK_PARA_ERR;
    }

    if (SDK_ICC_ICC == pstIccParam->ucCardMode) {
        //sdkmSleep(800);     //�Ӵ�ʽIC����λҪͣ��800ms,��Ȼ�п��ܻḴλ���ɹ�
    }
    OutBuf = (u8 *) sdkGetMem(OutBuf_size);

    if (NULL == OutBuf) {
        Assert(0);
        return SDK_ERR;
    }
    memset(OutBuf, 0, OutBuf_size);

    ret = sdkIccResetIcc(pstIccParam, OutBuf, &OutLen);       //IC����λ
    TestLog("emv", "sdkIccResetIcc ret = %d\r\n", ret);

    if (SDK_OK != ret) {
        if (pstIccTradeParam->bIsFallback) {
            sdkFreeMem(OutBuf);
            OutBuf = NULL;
            return SDK_NeedMagCardErr;
        }
        else {
            sdkFreeMem(OutBuf);
            OutBuf = NULL;
            return SDK_ICC_RESET_ERR;
        }
    }
    ret = sdkIccGetAppCandidate(pstIccTradeParam,
                                pstIccParam);                                           // ��ȡӦ�ú�ѡ�б�

    if (ret != SDK_OK) {
        sdkFreeMem(OutBuf);
        OutBuf = NULL;
        return ret;
    }
    sdkFreeMem(OutBuf);
    OutBuf = NULL;

    memcpy(&FixedAid, &pstIccTradeParam->stFixedAid, sizeof(SDK_ICC_FIXED_AID));

    while (1) {
        ret = sdkTestGoToSelectApp(FixedAid, pstIccTradeParam, pstIccParam);      //ȥѡ��Ӧ��

        if (ret == SDK_FixedAidAbsentErr)       //ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
        {
            FixedAid.bIsFixed = false;           //ѡ��һ��֮�������־ sjz 20120508
            continue;
        }
        else if (SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
        {
            if (pstIccTradeParam->bIsFallback) {
                return SDK_NeedMagCardErr;
            }
        }
        else if (ret == SDK_OK) {
            TestLog("emv", "pstIccTradeParam->eFlowMode = %d\r\n", pstIccTradeParam->eFlowMode);

//��������ϸ
            if (SDK_ICC_READLOG == pstIccTradeParam->eFlowMode) {
                return SDK_DispLog;                                           //��ʾ������ϸ
            }

//��ѯ���
            else if (SDK_ICC_EC_QUERY == pstIccTradeParam->eFlowMode) {
                return sdkIccReadECBalance(pstIccTradeParam, pstIccParam);     //jiguang  20120602
            }
            ret = sdkTestGoToInitialApp(pstIccTradeParam, pstIccParam);      //ȥ��ʼ��Ӧ��
        }

        if (ret == SDK_AppSelectTryAgainErr) {
            if (NULL != pstIccTradeParam->DispReselectApp) {
                ret = (*pstIccTradeParam->DispReselectApp)();                                              // ��ʾ"����ѡ��"

                if (ret != SDK_OK) {
                    return ret;
                }
            }
            ret = sdkIccReSelectApp();                                           //����ѡ��Ӧ��

            if (SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
            {
                if (pstIccTradeParam->bIsFallback) {
                    return SDK_NeedMagCardErr;
                }
            }

            if (ret != SDK_OK) {
                return ret;
            }
            continue;
        }
            //qPboc����
        else if (SDK_OnlineWait ==
                 ret)                                               // qpboc����(�Ǵ������ֽ��˻�)
        {
            return sdkTestDealQpbocOnline(pstIccTradeParam, pstIccParam);
        }
            //qPboc�ѻ���׼
        else if (SDK_OfflineApprove ==
                 ret)                                               //qPboc�ѻ���׼
        {
            return sdkTestDealQpbocOffline(pstIccTradeParam, pstIccParam);        //jiguang 20120602
        }
        else if (ret != SDK_OK) {
            return ret;
        }
        break;
    }

    return sdkTestDealEmvFlow(pstIccTradeParam, pstIccParam);      //����emv����
}

/*****************************************************************************
** Descriptions:sdkTestTransFlow2��ͬ��sdkIccTransFlow2
** Parameters:          SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_PARA_ERR:��������
                                SDK_ERR:����ʧ��
                                SDK_TransOnlineApprove:�����ɹ�
                                SDK_TransOnlineDeclineErr:�����ܾ�
                                SDK_TransOfflineApprove:�ѻ���׼
                                SDK_TransOfflineDeclineErr:�ѻ��ܾ�
                                SDK_TransNotAcceptErr:��Ƭ������������ķ���
                                SDK_TransTerminateErr:����ʧ��
                                SDK_IccCommandErr :��������
                                SDK_CDAFormatErr:CDA���ݸ�ʽ��
                                SDK_AppTimeOutErr: �������ʱ

** Created By:		sujianzhong2012.07.30
** Remarks:  //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/
s32 sdkTestTransFlow2(SDK_ICC_TRADE_PARAM *pstIccTradeParam, const SDK_ICC_PARAM *pstIccParam) {
    s32 ret;

    if ((pstIccTradeParam == NULL) || (pstIccParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (SDK_ICC_RF == pstIccParam->ucCardMode) //���ٲ� 2013.12.30 15:24
    {
        if (pstIccParam->ucCLType != 0x01) //�ǽ�PBOC�����������,qpboc�����������
        {
            return SDK_TransOnlineApprove; //���ٲ� 2013.12.30 15:24
        }
    }

    if (SDK_ICC_ICC != pstIccParam->ucCardMode &&
        SDK_ICC_RF != pstIccParam->ucCardMode)     //���ǽӴ�ʽIC��,qpboc����Ҫ��������
    {
        return SDK_ERR;
    }

    while (1) {
        ret = sdkIccTransProcess(pstIccTradeParam,
                                 pstIccParam);                                            //���״���

        if (SDK_NeedRefer == ret) {
            sdkIccProcessRefer(1); //����ο�������ĿǰĬ��Ϊ�ο�ʧ�ܣ������׾ܾ�
            continue;
        }
        else if ((ret == SDK_NeedReversal) || (SDK_OK == ret) || (SDK_ECLoadErr == ret)) {
            break;
        }
        else {
            return ret;
        }
    }

    return sdkIccTransComplete();                                           //�������
}

////20140414  �Ȳ��ͷŶ�̬�������Դ

s32 sdkTestIccDispLog(SDK_ICC_APDU_RESP *apdu_r, u8 *aucDOL, u8 ucRecordNum, bool partno) {
    unsigned char *pDOL, *pDataOut;
    unsigned char aucTransDate[3], aucTransTime[3], aucAuthAmount[6], aucOtherAmount[6],
            aucCountryCode[2], aucTransCurcyCode[2], aucMerchantName[20], aucATC[4], ucTransType;
    unsigned char ucTagLen;
    unsigned short i;
    unsigned short recorddatalen = 0;
    unsigned char aucBuff[30], temp[13];

    pDOL = aucDOL;
    pDataOut = (unsigned char *) apdu_r->DataOut;
    memset(aucTransDate, 0x00, sizeof(aucTransDate));
    memset(aucTransTime, 0x00, sizeof(aucTransTime));
    memset(aucAuthAmount, 0x01, sizeof(aucAuthAmount));
    memset(aucOtherAmount, 0x01, sizeof(aucOtherAmount));
    memset(aucCountryCode, 0x00, sizeof(aucCountryCode));
    memset(aucTransCurcyCode, 0x00, sizeof(aucTransCurcyCode));
    memset(aucMerchantName, 0x00, sizeof(aucMerchantName));
    memset(aucATC, 0x00, sizeof(aucATC));
    ucTransType = SDK_ICC_TRANS_TYPE_GOODS;

    while (recorddatalen < apdu_r->LenOut) //���������
    {
        if (((*pDOL) & 0x1F) == 0x1F) { ucTagLen = 2; }
        else { ucTagLen = 1; }

        if (!memcmp(pDOL, "\x9A", ucTagLen)) {
            memcpy(aucTransDate, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x21", ucTagLen)) {
            memcpy(aucTransTime, pDataOut, *(pDOL + ucTagLen));
        }

        else if (!memcmp(pDOL, "\x9F\x02", ucTagLen)) {
            memcpy(aucAuthAmount, pDataOut, *(pDOL + ucTagLen));
        }

        else if (!memcmp(pDOL, "\x9F\x03", ucTagLen)) {
            memcpy(aucOtherAmount, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x1A", ucTagLen)) {
            memcpy(aucCountryCode, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x5F\x2A", ucTagLen)) {
            memcpy(aucTransCurcyCode, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x4E", ucTagLen)) {
            memcpy(aucMerchantName, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9C", ucTagLen)) {
            memcpy(&ucTransType, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x36", ucTagLen)) {
            memcpy(aucATC, pDataOut, *(pDOL + ucTagLen));
        }
        pDataOut += (*(pDOL + ucTagLen));
        recorddatalen += (*(pDOL + ucTagLen));
        pDOL += ucTagLen + 1;
    }

    for (i = 0; i < 6; i++) {
        if (aucAuthAmount[i] != 0XFF) { break; }

        if (aucOtherAmount[i] != 0XFF) { break; }
    }

    if (i == 6) {
        return SDK_ERR;
    }
    else     //sxl ��ʾ������־
    {
        if (partno == 0) {
            sdkDispClearScreen();
            memset(aucBuff, 0x00, sizeof(aucBuff));

            if (ucRecordNum == 0x0a) { ucRecordNum = 0x10; }
            sdkBcdToAsc(aucBuff, &ucRecordNum, 1);
            strcpy((char *) (&aucBuff[2]), ":��������:"); //sxl arm_linux
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucTransDate, 3);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "����ʱ��:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucTransTime, 3);
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            memset(temp, 0x00, sizeof(temp));
            strcpy((char *) aucBuff, "���:");  //sxl arm_linux
            sdkRegulateAmount(temp, aucAuthAmount);
            strcat((char *) aucBuff, (char *) temp); //sxl arm_linux
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);  //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            memset(temp, 0x00, sizeof(temp));
            strcpy((char *) aucBuff, "�����:"); //sxl arm_linux
            sdkRegulateAmount(temp, aucOtherAmount);
            strcat((char *) aucBuff, (char *) temp);       //sxl arm_linux
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);  //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "�ն˹��Ҵ���:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucCountryCode, 2);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            sdkDispBrushScreen();
        }
        else {
            sdkDispClearScreen();
            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "���׻��Ҵ���:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucTransCurcyCode, 2);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "�̻�����:");
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            memcpy(aucBuff + strlen((char *) aucBuff), aucMerchantName, sizeof(aucMerchantName));
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);  //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "��������:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), &ucTransType, 1);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "ATC:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucATC, 2);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            sdkDispBrushScreen();
        }
        return SDK_OK;
    }
}

////20140414  �Ȳ��ͷŶ�̬�������Դ

s32 sdkTestIccDealLog(SDK_ICC_APDU_RESP *pLogData, u8 *pLogFormat, u8 ucLogNumOut) {
    u8 i = 0;
    bool bSecHalf = false;    //��ʾ�ڶ�����
    s32 key;

    TestLog("goto->sdkTestIccDealLog", "\r\n");
    sdkTestIccDispLog(&pLogData[i], pLogFormat, i, bSecHalf);

    while (1) {
        key = sdkKbGetKey();

        if (key == SDK_KEY_ENTER) {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            bSecHalf = false;
            i++;

            if (i < ucLogNumOut) {
                sdkTestIccDispLog(&pLogData[i], pLogFormat, i, bSecHalf);
            }
            else {
                break;
            }
        }
        else if (key == SDK_KEY_DOWN) {
            sdkSysBeep(SDK_SYS_BEEP_OK);

            if (bSecHalf == false) //��ǰ��ʾ���ǵ�һ����
            {
                bSecHalf = true;
                sdkTestIccDispLog(&pLogData[i], pLogFormat, i, bSecHalf); //��ʾ�ڶ�����
            }
        }
        else if (key == SDK_KEY_UP) {
            sdkSysBeep(SDK_SYS_BEEP_OK);

            if (bSecHalf == true) //��ǰ��ʾ���ǵڶ�����
            {
                bSecHalf = false;
                sdkTestIccDispLog(&pLogData[i], pLogFormat, i, bSecHalf); //��ʾ��һ����
            }
        }
        else if (key == SDK_KEY_ESC) {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            return SDK_OK;
        }
    }

    return SDK_OK;
}

void sdkTestIccSetPed()  //20140414  �Ȳ��ͷŶ�̬�������Դ
{
    s32 key;
    SDK_SYS_INITIAL_INFO initial;
    SDK_PED_WK_CFG stPedWkCfg[2];
    u8 temp[30] = {0};
    s32 ret;

    memset(temp, 0x31, sizeof(temp));

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "�����豸",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1.���ü���,���÷ǽ�",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "2.���ü���,���÷ǽ�",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "3.���ü���,���÷ǽ�",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "4.���ü���,���÷ǽ�",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    key = sdkKbWaitKey(
            SDK_KEY_MASK_0 | SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 |
            SDK_KEY_MASK_ESC, 300 * 100);
    switch (key) {
        case '1':
            initial.bIsHavePinPad = false;
            initial.bIsINRf = true;
            break;

        case '2':
            initial.bIsHavePinPad = true;
            initial.bIsINRf = true;
            break;

        case '3':
            initial.bIsHavePinPad = false;
            initial.bIsINRf = false;
            break;

        case '4':
            initial.bIsHavePinPad = true;
            initial.bIsINRf = false;
            break;

        default:
            return;
    }

    sdkSysReInitParam(&initial);


    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������Կ��...", SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();
    //������Կ
    ret = sdkPEDUpdateTmk(0, SDK_PED_DES_SINGLE, temp, SDK_PED_TIMEOUT * 2);
    sdkmSleep(200);

    if (ret == SDK_OK) {
        memset(&stPedWkCfg[0], 0, sizeof(SDK_PED_WK_CFG) * 2);
        stPedWkCfg[0].bOnlyCalcCheckValue = false;

        stPedWkCfg[0].eTmkType = SDK_PED_DES_SINGLE;
        stPedWkCfg[1].eTmkType = SDK_PED_DES_SINGLE;

        stPedWkCfg[0].eWkType = SDK_PED_DES_SINGLE;
        stPedWkCfg[1].eWkType = SDK_PED_DES_SINGLE;

        stPedWkCfg[0].ucTmkIndex = 0;
        stPedWkCfg[1].ucTmkIndex = 0;

        stPedWkCfg[0].ucWkIndex = 0;
        stPedWkCfg[1].ucWkIndex = 1;

        stPedWkCfg[0].ucEnWkLen = 8;
        stPedWkCfg[1].ucEnWkLen = 8;

        memset(stPedWkCfg[0].heCheckData, 0, 24);
        memset(stPedWkCfg[1].heCheckData, 0, 24);

        //3131313131313131//����Կ
        //F92714E80E832EDC//ADC67D84//4BF6E91B1E3A9D81//ADC67D84//���������Լ�checkvalue
        //3232323232323232//3333333333333333//���ܺ�����
        memcpy(stPedWkCfg[0].heEnWk, "\xF9\x27\x14\xE8\x0E\x83\x2E\xDC", 8);
        memcpy(stPedWkCfg[1].heEnWk, "\x4B\xF6\xE9\x1B\x1E\x3A\x9D\x81", 8);

        stPedWkCfg[0].ucCheckDataLen = 8;
        stPedWkCfg[1].ucCheckDataLen = 8;


        stPedWkCfg[0].ucCheckValueLen = 4;
        stPedWkCfg[1].ucCheckValueLen = 4;

        memcpy(stPedWkCfg[0].heCheckValue, "\xAD\xC6\x7D\x84", 4);
        memcpy(stPedWkCfg[1].heCheckValue, "\xAD\xC6\x7D\x84", 4);

        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�๤����Կ��...", SDK_DISP_LEFT_DEFAULT);
        sdkDispBrushScreen();
        //�๤����Կ
        ret = sdkPEDUpdateWk(&stPedWkCfg[0], 2, SDK_PED_TIMEOUT * 5);
    }

    if (ret != SDK_OK) {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "����Կʧ��!!!", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��������˳�!", SDK_DISP_LEFT_DEFAULT);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, SDK_ICC_TIMER_AUTO);
        return;
    }
    else {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "����Կ�ɹ�!!!", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "�����������!", SDK_DISP_LEFT_DEFAULT);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, SDK_ICC_TIMER_AUTO);
    }
    return;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.25
** Remarks:  //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/

void TestInitEmvData(SDK_ICC_TRADE_PARAM *IccTradeParam, SDK_ICC_PARAM *IccParam,
                     SDK_ICC_CARDDATA *CardData) {
    memset(IccParam, 0, sizeof(SDK_ICC_PARAM));
    memset(IccTradeParam, 0, sizeof(SDK_ICC_TRADE_PARAM));
    memset(CardData, 0, sizeof(SDK_ICC_CARDDATA));
    memset(gasTradeAmount, 0, sizeof(gasTradeAmount));

    IccParam->eCardType = SDK_ICC_CPU;
    IccTradeParam->bIsForceOnline = false;
    IccTradeParam->bIsFallback = true;
    IccTradeParam->pasTradeAmount = gasTradeAmount; //sujianzhong 2013.01.10 14:27
    IccTradeParam->bIsSupportEc = true;
    IccTradeParam->bIsPromptChooseEc = true;
    IccTradeParam->bIsDefaultEc = true;

    IccTradeParam->VerifyIDCard = NULL;

    IccTradeParam->UserIccTransInit = sdkTestIccUserTransInitCb;
    IccTradeParam->InputAmount = sdkTestIccInputAmountCb;
    IccTradeParam->DispReselectApp = sdkTestIccDispSelAgainCb;
    IccTradeParam->DispCardOut = sdkTestIccDispCardOutCb;
    IccTradeParam->VerifyCardNo = sdkTestIccDispCardNoCb;
    IccTradeParam->InputPWD = sdkTestInputCreditPwd;
    IccTradeParam->DispOnlineRandNum = sdkTestIccDispOnlineRandNumCb;
    IccTradeParam->DispChooseEC = sdkTestIccChooseEcCb;
    IccTradeParam->DispCandAppList = sdkTestIccDispCandAppListCb;
    IccTradeParam->DispPromptData = sdkTestIccDispPromptDataCb;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.25
** Remarks:  //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/

s32 TestJugeEmvProsses(SDK_ICC_TRADE_PARAM *IccTradeParam, SDK_ICC_PARAM *IccParam,
                       SDK_ICC_CARDDATA *CardData, u8 cardModeEn) {
    s32 ret;


    if (IccTradeParam->eFlowMode == SDK_ICC_EASY_FLOW) {
        IccTradeParam->stFixedAid.bIsFixed = true;
        IccTradeParam->stFixedAid.uclen = 8;
        memcpy(IccTradeParam->stFixedAid.heAid, "\xA0\x00\x00\x03\x33\x01\x01\x01", 8);
    }
    else {
        memset(&IccTradeParam->stFixedAid, 0, sizeof(SDK_ICC_FIXED_AID));
    }
    sdkTestReadCardDisp(cardModeEn);
    //Trace("emv","sdkTestIccReadCard(cardModeEn,  IccTradeParam, IccParam, CardData, SDK_DISP_LINE4);\r\n");
    ret = sdkTestIccReadCard(cardModeEn, IccTradeParam, IccParam, CardData, SDK_DISP_LINE4);
    TestLog("emv", "sdkIccReadCard return value=%d\r\n", ret);

    if (ret != SDK_OK) {
        sdkIccCloseMag();
        sdkIccPowerDown();
        return 0;
    }

    if (IccParam->ucCardMode != SDK_ICC_RF) {
        sdkIccPowerDown();
    }

    if ((SDK_ICC_RF != IccParam->ucCardMode) && (SDK_ICC_ICC != IccParam->ucCardMode)) {
        sdkTestIccDispText("��ʹ��IC��");
        sdkIccCloseMag();
        sdkIccPowerDown();
        return 0;
    }
    return 1;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Re
** Created By:		yinpeng  2013.01.25
** Remarks:  //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/

void TestEmvTradeProsses(SDK_ICC_TRADE_PARAM *IccTradeParam, SDK_ICC_PARAM *IccParam) {
    s32 ret;

    ret = sdkTestTransFlow1(IccTradeParam, IccParam);
    TestLog("emv", "sdkIccDealIccTrade return value:%d\r\n", ret);
    {
        u8 TVR[5] = {0}, TSI[2] = {0}, TermCapab[3] = {0};
        u32 len = 0;
        sdkIccReadTLV("\x95", TVR, &len);
        sdkIccReadTLV("\x9B", TSI, &len);
        sdkIccReadTLV("\x9F\x33", TermCapab, &len);
        TestLogHex("emv", "-----TVR---------", TVR, 5);
        TestLogHex("emv", "-----TSI---------", TSI, 2);
        TestLogHex("emv", "--TermCapab--", TermCapab, 3);
    }

    switch (ret) {
        // ��������
        case SDK_ICC_RESET_ERR:
            sdkTestIccDispText("��λʧ��!");
            break;

        case SDK_TransOnlineApprove:     //����1�޴˷���ֵ
        {
            u32 ScriptResultLen = 0;
            u8 *ScriptResult = NULL;
            ScriptResult = (u8 *) sdkGetMem(256);

            if (NULL == ScriptResult) {
                Assert(0);
                return;
            }
            memset(ScriptResult, 0, 256);
            TestLog("emv", "sdkIccGetScriptResult = %d\r\n",
                    sdkIccGetScriptResult(&ScriptResultLen, ScriptResult));
            sdkFreeMem(ScriptResult);
            ScriptResult = NULL;
        }
            sdkTestIccDispText("�������׳ɹ�");
            break;

        case SDK_OnlineWait:
            sdkTestIccDispText("�ȴ�����!");
            ret = sdkTesticcSendBagCb();                                // �����������ݰ�
            TestLog("emv", "sdkTesticcSendBagCb return value = %d\r\n", ret);

            if (ret == SDK_OK) {
#if 0 /*Modify by ���ٲ� at 2013.12.30  15:24 */

                if(IccParam->ucCardMode == SDK_ICC_RF)      //�ǽӲ��ô�������2
                {
                    sdkTestIccDispText("�������׳ɹ�");
                    break;
                }
#endif /* if 0 */
                ret = sdkTestTransFlow2(IccTradeParam, IccParam);

                if (ret == SDK_TransOnlineApprove) {
                    sdkTestIccDispText("�������׳ɹ�");
                    break;
                }
            }
            else {
                u8 temp[24] = {0};
                sprintf(temp, "����ʧ��,����ֵΪ%d", ret);
                sdkTestIccDispText(temp);
            }
            break;

        case SDK_TransOfflineApprove:
            sdkTestIccDispText("�ѻ����׳ɹ�");
            break;

        case SDK_TransOfflineDeclineErr:
            sdkTestIccDispText("�ѻ��ܾ�");
            break;

        case SDK_ECNotSupportErr:
            sdkTestIccDispText("��֧�ִ������ֽ�");
            break;

        case SDK_EcLackBalanceErr:
            sdkTestIccDispText("����,�������ֽ�");
            break;

        case SDK_NeedReversal:
            sdkTestIccDispText("��Ҫ����");
            break;

        case SDK_NeedMagCardErr:
            sdkTestIccDispText("��ʹ�ô�����");
            break;

        case SDK_IccEasyFlow:
            sdkTestIccDispText("���˼�����");
            break;

        case SDK_DispLog: {
            u8 pLogFormat[100] = {0}, pLogNumOut;
            SDK_ICC_APDU_RESP pLogData[10];
            u8 rslt;

            memset(pLogData, 0, sizeof(pLogData));
            TestLog("emv", "rslt = %d\r\n",
                    sdkIccReadLogDetail(IccParam, 0, pLogFormat, pLogData, &pLogNumOut));
            TestLog("emv", "rslt = %d\r\n",
                    sdkIccReadLogDetail(IccParam, 2, pLogFormat, pLogData, &pLogNumOut));
            TestLog("emv", "rslt = %d\r\n",
                    rslt = sdkIccReadLogDetail(IccParam, 10, pLogFormat, pLogData, &pLogNumOut));

            if (rslt == SDK_ReadLogEnd) {
                if (pLogNumOut > 0) {
                    sdkTestIccDealLog(pLogData, pLogFormat, pLogNumOut);
                }
            }
            else {
                sdkTestIccDispText("����ϸʧ��");
            }
        }

            break;

        case SDK_OK:

            if (IccTradeParam->eFlowMode == SDK_ICC_EC_QUERY) {
                u8 pFirstCurrencyCode_Bcd[2], pFirstBalance_Bcd[6], pSecondCurrencyCode_Bcd[2], pSecondBalance_Bcd[6];
                u8 amt[64] = {0};
                TestLog("emv", "sdkIccGetECQueryMsg rslt=%d\r\n",
                        sdkIccGetECQueryMsg(pFirstCurrencyCode_Bcd, pFirstBalance_Bcd,
                                            pSecondCurrencyCode_Bcd, pSecondBalance_Bcd));

                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ѯ�ɹ�",
                                  SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�������:",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkRegulateAmount(amt, pFirstBalance_Bcd);
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, amt,
                                  SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();

                sdkSysBeep(SDK_SYS_BEEP_OK);
                sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
            }
            break;

        case SDK_UsePbocErr:
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ʹ��PBOC����",
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
            sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
            break;


        case SDK_QpbocNoCard:
            sdkTestIccDispText("δ��ʾIC��");
            break;

        case SDK_QpbocMoreCards:
            sdkTestIccDispText("���ſ�");
            break;

        case SDK_QpbocAuthNoPerformed:
            sdkTestIccDispText("δ����˫����֤");
            break;

        case SDK_ICC_NOCARD:
            sdkTestIccDispText("���Ѱγ�������ʧ��!");
            break;

        default: {
            u8 temp[24] = {0};
            sprintf(temp, "����ʧ��,����ֵΪ%d", ret);
            sdkTestIccDispText(temp);
        }
            break;
    }


    TestLog("sdkiccexit", "sdkIccCloseMag");
    sdkIccCloseMag();
    TestLog("sdkiccexit", "sdkIccPowerDown");
    sdkIccPowerDown();
    {
        u8 *temp;
        s32 temp_len;
        temp = (u8 *) sdkGetMem(1024);

        if (temp == NULL) {
            TestLog("sdkiccreadtlverr", "ret error");
            return;
        }
        memset(temp, 0, 1024);
        TestLog("sdkiccreadtlv", "ret=%d\r\n", sdkIccReadTLV("\x9F\x21", temp, &temp_len));
        sdkFreeMem(temp);
    }

    TestLog("sdkiccexit", "sdkIccreturn");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.25
** Remarks:   //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/
s32 TestEmvCaseOption(char testkey, SDK_ICC_TRADE_PARAM *IccTradeParam, u8 *cardModeEn) {
    s32 ret;

    switch (testkey) {
        case '0':         //qpboc

            if (sdkTestIccInputAmountCb(IccTradeParam->pasTradeAmount) != SDK_OK) {
                return 0;
            }
            IccTradeParam->bIsSupportQPBOC = true;
            IccTradeParam->bIsSupportCLPBOC = false;
            IccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            *cardModeEn = SDK_ICC_MAG | SDK_ICC_MANUAL | SDK_ICC_RF;
            IccTradeParam->VerifyCardNo = NULL; //sujianzhong 2013.08.05 10:36
            break;

        case '1':         //�ǽ�ΪPBOC
            IccTradeParam->bIsSupportQPBOC = false;
            IccTradeParam->bIsSupportCLPBOC = true;
            IccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            *cardModeEn = SDK_ICC_MAG | SDK_ICC_MANUAL | SDK_ICC_RF;
            break;

        case '2':         //�Ӵ�ʽΪEMV
            IccTradeParam->bIsSupportQPBOC = false;
            IccTradeParam->bIsSupportCLPBOC = false;
            IccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            *cardModeEn = SDK_ICC_MAG | SDK_ICC_MANUAL | SDK_ICC_ICC;
            break;

        case '3':         //qpboc��pboc
            IccTradeParam->bIsSupportQPBOC = true;
            IccTradeParam->bIsSupportCLPBOC = true;
            IccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            *cardModeEn = SDK_ICC_MAG | SDK_ICC_MANUAL | SDK_ICC_RF;
            break;

        case '4':         //������
            IccTradeParam->bIsSupportQPBOC = false;
            IccTradeParam->bIsSupportCLPBOC = true;
            IccTradeParam->eFlowMode = SDK_ICC_EASY_FLOW;
            IccTradeParam->bIsSupportEc = false;
            IccTradeParam->bIsPromptChooseEc = true;
            IccTradeParam->bIsDefaultEc = true;
            *cardModeEn = SDK_ICC_MAG | SDK_ICC_MANUAL | SDK_ICC_ICC | SDK_ICC_RF;
            break;

        case '5':         //����ϸ��¼
            IccTradeParam->bIsSupportQPBOC = false;
            IccTradeParam->bIsSupportCLPBOC = true;
            IccTradeParam->eFlowMode = SDK_ICC_READLOG;
            *cardModeEn = SDK_ICC_MAG | SDK_ICC_MANUAL | SDK_ICC_ICC | SDK_ICC_RF;
            break;

        case '6':         //�����ֽ��ѯ���
            IccTradeParam->bIsSupportQPBOC = false;
            IccTradeParam->bIsSupportCLPBOC = true;
            IccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;
            IccTradeParam->bIsSupportEc = true;
            IccTradeParam->bIsPromptChooseEc = false;
            IccTradeParam->bIsDefaultEc = true;
            *cardModeEn = SDK_ICC_MAG | SDK_ICC_MANUAL | SDK_ICC_ICC | SDK_ICC_RF;
            break;

        case '7':    //�����������
            sdkTestIccSetPed();
            return 0;

        case '8':
            ret = sdkIccAddAnyAIDList(sdkTestAID, sizeof(sdkTestAID) / sizeof(sdkTestAID[0]));

            if (SDK_OK == ret) {
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����AID �ɹ�",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
            }
            else {
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����AID  ʧ��",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
            }
            return 0;

        case '9':

            ret = sdkIccAddAnyCAPKList(sdkTestCAPK, sizeof(sdkTestCAPK) / sizeof(sdkTestCAPK[0]));

            if (SDK_OK == ret) {
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����CAPK  �ɹ�",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
            }
            else {
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����CAPK  ʧ��",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
            }
            return 0;

        default:
            return 0;
    }

    return 1;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.25
** Remarks:             //20140414  �Ȳ��ͷŶ�̬�������Դ
*****************************************************************************/
void TestSdkIccEmv(char testkey) {
    s32 ret;

    SDK_ICC_TRADE_PARAM IccTradeParam;
    SDK_ICC_PARAM IccParam;
    SDK_ICC_CARDDATA *CardData;
    u8 cardModeEn = 0;

    CardData = (SDK_ICC_CARDDATA *) emvb_malloc(sizeof(SDK_ICC_CARDDATA));

    if (CardData == NULL) {
        Assert(0);
        return;
    }
    TestInitEmvData(&IccTradeParam, &IccParam, CardData); //yinpeng 2013.01.25 15:50��ʼ������
    ret = TestEmvCaseOption(testkey, &IccTradeParam,
                            &cardModeEn); //yinpeng 2013.01.29 15:19ѡ����԰���

    if (0 == ret) {
        emvb_free(CardData);
        return;
    }
    ret = TestJugeEmvProsses(&IccTradeParam, &IccParam, CardData,
                             cardModeEn); //yinpeng 2013.01.25 15:55 �ж�����
    emvb_free(CardData);

    if (0 == ret) {
        return;
    }
    TestEmvTradeProsses(&IccTradeParam, &IccParam); //yinpeng 2013.01.25 16:3���״���
    Trace("emv", "CardData deallocated.\r\n");
}

#if 0
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	void TestSdkIccEmv(void)
   ��������:	emv����
   �������:
   �������:
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��:2012.07.02 15:43:43
*******************************************************************/
void TestSdkIccEmv(char testkey)
{
    u8 cardModeEn;
    s32 ret;
    SDK_ICC_TRADE_PARAM IccTradeParam;
    SDK_ICC_PARAM IccParam;
    SDK_ICC_CARDDATA CardData;
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    memset(&IccParam, 0, sizeof(SDK_ICC_PARAM));
    memset(&IccTradeParam, 0, sizeof(SDK_ICC_TRADE_PARAM));
    memset(&CardData, 0, sizeof(SDK_ICC_CARDDATA));
    {
        u8 data[1024] = {0};
        u8 cardModeEn;
        u32 len = 0;
        s32 ret;
        SDK_ICC_PARAM IccParam;
        SDK_ICC_CARDDATA CardData;

        while(1)
        {
            cardModeEn = SDK_ICC_RF;
            IccParam.eCardType = SDK_ICC_MOBILE;
            memset(&CardData, 0, sizeof(SDK_ICC_CARDDATA));
            ret = sdkTestIccReadCard(cardModeEn, NULL, &IccParam, &CardData, SDK_DISP_LINE1);
            TestLog("emv", "sdkTestIccReadCard return value=%d\r\n", ret);

            if(ret != SDK_OK)
            {
                break;
            }

            if((SDK_ICC_RF != IccParam.ucCardMode))
            {
                break;
            }
            ret = sdkUpcardTransFlow(&IccParam);

            if(ret != SDK_OK)
            {
                break;
            }
            ret = sdkUpcardReadTLV("\xDF\x32", data, &len);

            if(ret != SDK_OK)
            {
                break;
            }
            ret = sdkUpcardReadTLV("\xDF\x33", data, &len);

            if(ret != SDK_OK)
            {
                break;
            }
            ret = sdkUpcardReadTLV("\xDF\x34", data, &len);

            if(ret != SDK_OK)
            {
                break;
            }
            ret = sdkUpcardGetMagData(&CardData);

            if(ret != SDK_OK)
            {
                break;
            }
        }

        sdkIccCloseMag();
        sdkIccPowerDown();
    }

    IccParam.eCardType = SDK_ICC_CPU;
    IccTradeParam.bIsForceOnline = false;
    IccTradeParam.bIsFallback = true;
    IccTradeParam.bIsSupportEc = true;
    IccTradeParam.bIsPromptChooseEc = true;
    IccTradeParam.bIsDefaultEc = true;
    {
#if 0   //yinpeng 2013.01.25 14:29
        s32 key = '3';
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "0.QPBOC", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE1, (pixel.siX / 2), "1.PBOC", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "2.EMV�Ӵ�", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE2, (pixel.siX / 2), "3.QPBOC|PBOC", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "4.������", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, (pixel.siX / 2), "5.��ϸ��ѯ", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "6.�����ֽ�����ѯ", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
//        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "7.���п�����ѯ", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "8.����AID", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE5, (pixel.siX / 2), "9.����CAPK", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispBrushScreen();

        key = sdkKbWaitKey(SDK_KEY_MASK_0 | SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 | SDK_KEY_MASK_5 | SDK_KEY_MASK_6 | SDK_KEY_MASK_7 | SDK_KEY_MASK_8 | SDK_KEY_MASK_9 | SDK_KEY_MASK_ESC, 300 * 100);
#endif

        switch (testkey)
        {
             case '0':     //qpboc

               if(sdkTestIccInputAmountCb(IccTradeParam.pasTradeAmount) != SDK_OK)
               {
                   return;
               }
               IccTradeParam.bIsSupportQPBOC = true;
               IccTradeParam.bIsSupportCLPBOC = false;
               IccTradeParam.eFlowMode = SDK_ICC_ALL_FLOW;
               break;

             case '1':     //�ǽ�ΪPBOC
               IccTradeParam.bIsSupportQPBOC = false;
               IccTradeParam.bIsSupportCLPBOC = true;
               IccTradeParam.eFlowMode = SDK_ICC_ALL_FLOW;
               break;

             case '2':     //�Ӵ�ʽΪEMV
               IccTradeParam.bIsSupportQPBOC = false;
               IccTradeParam.bIsSupportCLPBOC = false;
               IccTradeParam.eFlowMode = SDK_ICC_ALL_FLOW;
               break;

             case '3':     //qpboc��pboc
               IccTradeParam.bIsSupportQPBOC = true;
               IccTradeParam.bIsSupportCLPBOC = true;
               IccTradeParam.eFlowMode = SDK_ICC_ALL_FLOW;
               break;

             case '4':     //������
               IccTradeParam.bIsSupportQPBOC = false;
               IccTradeParam.bIsSupportCLPBOC = true;
               IccTradeParam.eFlowMode = SDK_ICC_EASY_FLOW;
               IccTradeParam.bIsSupportEc = false;
               IccTradeParam.bIsPromptChooseEc = true;
               IccTradeParam.bIsDefaultEc = true;
               break;

             case '5':     //����ϸ��¼
               IccTradeParam.bIsSupportQPBOC = false;
               IccTradeParam.bIsSupportCLPBOC = true;
               IccTradeParam.eFlowMode = SDK_ICC_READLOG;
               break;

             case '6':     //�����ֽ��ѯ���
               IccTradeParam.bIsSupportQPBOC = false;
               IccTradeParam.bIsSupportCLPBOC = true;
               IccTradeParam.eFlowMode = SDK_ICC_EC_QUERY;
               IccTradeParam.bIsSupportEc = true;
               IccTradeParam.bIsPromptChooseEc = false;
               IccTradeParam.bIsDefaultEc = true;
               break;

             case '7':
               sdkTestIccSetPed(); //yinpeng 2013.01.25 16:5����PED
               return;

             //             case '7': //���˻���ѯ���
//               IccTradeParam.bIsSupportQPBOC = true;
//               IccTradeParam.bIsSupportCLPBOC = true;
//               IccTradeParam.eFlowMode = SDK_ICC_ALL_FLOW;
//               IccTradeParam.bIsSupportEc = false;
//               IccTradeParam.bIsPromptChooseEc = true;
//               IccTradeParam.bIsDefaultEc = true;
//               break;

             case '8':
               sdkIccAddAnyAIDList(sdkTestAID, sizeof(sdkTestAID) / sizeof(sdkTestAID[0]));
               return;

             case '9':
               sdkIccAddAnyCAPKList(sdkTestCAPK, sizeof(sdkTestCAPK) / sizeof(sdkTestCAPK[0]));
               return;

             default:
               return;
        }
    }

    if(IccTradeParam.eFlowMode == SDK_ICC_EASY_FLOW)
    {
        IccTradeParam.stFixedAid.bIsFixed = true;
        IccTradeParam.stFixedAid.uclen = 8;
        memcpy(IccTradeParam.stFixedAid.heAid, "\xA0\x00\x00\x03\x33\x01\x01\x01", 8);
    }
    else
    {
        memset(&IccTradeParam.stFixedAid, 0, sizeof(SDK_ICC_FIXED_AID));
    }
//    IccTradeParam.UserIccTransInit = NULL;
//    IccTradeParam.InputAmount = NULL;
//    IccTradeParam.DispReselectApp = NULL;
//    IccTradeParam.DispCardOut = NULL;
//    IccTradeParam.VerifyCardNo = NULL;
//    IccTradeParam.InputPWD = NULL;
    IccTradeParam.VerifyIDCard = NULL;  //ʹ��Ĭ�ϵ�
//    IccTradeParam.DispOnlineRandNum = NULL;
//    IccTradeParam.DispChooseEC = NULL;

    IccTradeParam.UserIccTransInit = sdkTestIccUserTransInitCb;
    IccTradeParam.InputAmount = sdkTestIccInputAmountCb;
    IccTradeParam.DispReselectApp = sdkTestIccDispSelAgainCb;
    IccTradeParam.DispCardOut = sdkTestIccDispCardOutCb;
    IccTradeParam.VerifyCardNo = sdkTestIccDispCardNoCb;
    IccTradeParam.InputPWD = sdkTestInputCreditPwd;
    IccTradeParam.DispOnlineRandNum = sdkTestIccDispOnlineRandNumCb;
    IccTradeParam.DispChooseEC = sdkTestIccChooseEcCb;

    ret = sdkTestIccGetCardModeEn(&cardModeEn);

    if(ret != SDK_OK)
    {
        return;
    }
    sdkTestReadCardDisp(cardModeEn);
    ret = sdkTestIccReadCard(cardModeEn,  &IccTradeParam, &IccParam, &CardData, SDK_DISP_LINE4);
    TestLog("emv", "sdkIccReadCard return value=%d\r\n", ret);

    if(ret != SDK_OK)
    {
        sdkIccCloseMag();
        sdkIccPowerDown();
        return;
    }

    if(IccParam.ucCardMode != SDK_ICC_RF)
    {
        sdkIccPowerDown();
    }

    if((SDK_ICC_RF != IccParam.ucCardMode) && (SDK_ICC_ICC != IccParam.ucCardMode))
    {
        sdkTestIccDispText("��ʹ��IC��");
        sdkIccCloseMag();
        sdkIccPowerDown();
        return;
    }
    /*���׿�ʼ*/
    ret = sdkTestTransFlow1(&IccTradeParam, &IccParam);

    TestLog("emv", "sdkIccDealIccTrade return value:%d\r\n", ret);
    {
        u8 TVR[5] = {0}, TSI[2] = {0}, TermCapab[3] = {0};
        u32 len = 0;
        sdkIccReadTLV("\x95", TVR, &len);
        sdkIccReadTLV("\x9B", TSI, &len);
        sdkIccReadTLV("\x9F\x33", TermCapab, &len);
        TestLogHex("emv", "-----TVR---------", TVR, 5);
        TestLogHex("emv", "-----TSI---------", TSI, 2);
        TestLogHex("emv", "--TermCapab--", TermCapab, 3);
    }

    switch (ret)
    {
         // ��������
         case SDK_ICC_RESET_ERR:
           sdkTestIccDispText("��λʧ��!");
           break;

         case SDK_TransOnlineApprove:     //����1�޴˷���ֵ
            {
                u32 ScriptResultLen = 0;
                u8 ScriptResult[256] = {0};
                TestLog("emv", "sdkIccGetScriptResult = %d\r\n", sdkIccGetScriptResult(&ScriptResultLen, ScriptResult));
            }
           sdkTestIccDispText("�������׳ɹ�");
           break;

         case SDK_OnlineWait:
           sdkTestIccDispText("�ȴ�����!");
           ret =  sdkTesticcSendBagCb();                                // �����������ݰ�
           TestLog("emv", "sdkTesticcSendBagCb return value = %d\r\n", ret);

           if(ret == SDK_OK)
           {
               if(IccParam.ucCardMode == SDK_ICC_RF)      //�ǽӲ��ô�������2
               {
                   sdkTestIccDispText("�������׳ɹ�");
                   break;
               }
               ret = sdkTestTransFlow2(&IccTradeParam, &IccParam);

               if(ret == SDK_TransOnlineApprove)
               {
                   sdkTestIccDispText("�������׳ɹ�");
                   break;
               }
           }
           {
               u8 temp[24] = {0};
               sprintf(temp, "����ʧ��,����ֵΪ%d", ret);
               sdkTestIccDispText(temp);
           }
           break;

         case SDK_TransOfflineApprove:
           sdkTestIccDispText("�ѻ����׳ɹ�");
           break;

         case SDK_TransOfflineDeclineErr:
           sdkTestIccDispText("�ѻ��ܾ�");
           break;

         case SDK_ECNotSupportErr:
           sdkTestIccDispText("��֧�ִ������ֽ�");
           break;

         case SDK_EcLackBalanceErr:
           sdkTestIccDispText("����,�������ֽ�");
           break;

         case SDK_NeedReversal:
           sdkTestIccDispText("��Ҫ����");
           break;

         case SDK_NeedMagCardErr:
           sdkTestIccDispText("��ʹ�ô�����");
           break;

         case SDK_IccEasyFlow:
           sdkTestIccDispText("���˼�����");
           break;

         case SDK_DispLog:
            {
                u8 pLogFormat[100] = {0}, pLogNumOut;
                APDU_RESP pLogData[10];
                u8 rslt;

                memset(pLogData, 0, sizeof(pLogData));
                TestLog("emv", "rslt = %d\r\n", sdkIccReadLogDetail(&IccParam, 0, pLogFormat, pLogData, &pLogNumOut));
                TestLog("emv", "rslt = %d\r\n", sdkIccReadLogDetail(&IccParam, 2, pLogFormat, pLogData, &pLogNumOut));
                TestLog("emv", "rslt = %d\r\n", rslt = sdkIccReadLogDetail(&IccParam, 10, pLogFormat, pLogData, &pLogNumOut));

                if(rslt == SDK_ReadLogEnd)
                {
                    if(pLogNumOut > 0)
                    {
                        sdkTestIccDealLog(pLogData, pLogFormat, pLogNumOut);
                    }
                }
                else
                {
                    sdkTestIccDispText("����ϸʧ��");
                }
            }

            break;

         case SDK_OK:

           if(IccTradeParam.eFlowMode == SDK_ICC_EC_QUERY)
           {
               u8 pFirstCurrencyCode_Bcd[2], pFirstBalance_Bcd[6], pSecondCurrencyCode_Bcd[2], pSecondBalance_Bcd[6];
               u8 amt[64] = {0};
               TestLog("emv", "sdkIccGetECQueryMsg rslt=%d\r\n", sdkIccGetECQueryMsg(pFirstCurrencyCode_Bcd, pFirstBalance_Bcd, pSecondCurrencyCode_Bcd, pSecondBalance_Bcd));

               sdkDispClearScreen();
               sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ѯ�ɹ�", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
               sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�������:", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
               sdkRegulateAmount(amt, pFirstBalance_Bcd);
               sdkDispFillRowRam(SDK_DISP_LINE3, 0, amt, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
               sdkDispBrushScreen();

               sdkSysBeep(SDK_SYS_BEEP_OK);
               sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
           }
           break;

         case SDK_UsePbocErr:
           sdkDispClearScreen();
           sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ʹ��PBOC����", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
           sdkDispBrushScreen();
           sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
           break;


         case SDK_QpbocNoCard:
           sdkTestIccDispText("δ��ʾIC��");
           break;

         case SDK_QpbocMoreCards:
           sdkTestIccDispText("���ſ�");
           break;

         case SDK_QpbocAuthNoPerformed:
           sdkTestIccDispText("δ����˫����֤");
           break;

         case SDK_ICC_NOCARD:
           sdkTestIccDispText("���Ѱγ�������ʧ��!");
           break;

         default:
            {
                u8 temp[24] = {0};
                sprintf(temp, "����ʧ��,����ֵΪ%d", ret);
                sdkTestIccDispText(temp);
            }
            break;
    }

    sdkIccCloseMag();
    sdkIccPowerDown();
    {
        u8 temp[1024] = {0};
        s32 temp_len;
        TestLog("sdkiccreadtlv", "ret=%d\r\n", sdkIccReadTLV("\x9F\x21", temp, &temp_len));
    }
}

#endif

void TestSdkUpCard(void)  //20140107
{
    u8 *data;
    u8 cardModeEn;
    u32 len = 0;
    s32 ret;
    SDK_ICC_PARAM IccParam;
    SDK_ICC_CARDDATA *CardData;

    data = (u8 *) sdkGetMem(1024);

    if (data == NULL) {
        sdkTestIccDispText("���ֻ���ʧ��!");
        return;
    }
    memset(data, 0, 1024);
    CardData = (SDK_ICC_CARDDATA *) emvb_malloc(sizeof(SDK_ICC_CARDDATA));

    while (1) {
        cardModeEn = SDK_ICC_RF;
        IccParam.eCardType = SDK_ICC_MOBILE;
        memset(CardData, 0, sizeof(SDK_ICC_CARDDATA));
        sdkTestReadCardDisp(cardModeEn); //yinpeng 2013.01.31 16:12
        ret = sdkTestIccReadCard(cardModeEn, NULL, &IccParam, CardData, SDK_DISP_LINE1);
        TestLog("emv", "sdkTestIccReadCard return value=%d\r\n", ret);

        if (ret != SDK_OK) {
            break;
        }

        if ((SDK_ICC_RF != IccParam.ucCardMode)) {
            break;
        }
        ret = sdkUpcardTransFlow(&IccParam);

        if (ret != SDK_OK) {
            break;
        }
        ret = sdkUpcardReadTLV("\xDF\x32", data, &len);

        if (ret != SDK_OK) {
            break;
        }
        ret = sdkUpcardReadTLV("\xDF\x33", data, &len);

        if (ret != SDK_OK) {
            break;
        }
        ret = sdkUpcardReadTLV("\xDF\x34", data, &len);

        if (ret != SDK_OK) {
            break;
        }
        ret = sdkUpcardGetMagData(CardData);

        TestLogHex("upcard", "�ֻ�������", CardData, sizeof(SDK_ICC_CARDDATA));
        break;
    }

    sdkIccCloseMag();
    sdkIccPowerDown();

    if (ret == SDK_OK) {
        sdkTestIccDispText("���ֻ����ɹ�!");
    }
    else {
        sdkTestIccDispText("���ֻ���ʧ��!");
    }
    emvb_free(CardData);
    sdkFreeMem(data);
}

#if 0 //yinpeng 2013.01.25 14:11
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:
   ��������:
   �������:
   �������:
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��:2012.05.14 18:54:50
*******************************************************************/
void TestSdkIcc(void)
{
    while(1)
    {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "IC������", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1.CPU��", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "2.�ֻ���", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
//        sdkDispFillRowRam(SDK_DISP_LINE2 , (MAXCOL >> 1) , "2.4428��" , SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
//        sdkDispFillRowRam(SDK_DISP_LINE3 , 0 , "3.4442��" , SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
//        sdkDispFillRowRam(SDK_DISP_LINE3 , (MAXCOL >> 1) , "4. Mifare��" , SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "[ȡ��]->�˳�", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispBrushScreen();
        {
            s32 key = SDK_KEY_ENTER;
            key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 300 * 100);

            if(key == SDK_KEY_1)
            {
                TestSdkIccEmv(); //yinpeng 2013.01.24 16:59
            }
            else if(key == SDK_KEY_2)
            {
                TestSdkUpCard();
            }
//            else if(key == SDK_KEY_3)
//            {
//                TestSdkIcc4442();
//            }
//            else if(key == SDK_KEY_4)
//            {
//                TestSdkIcc4428();
//            }
            else if(key == SDK_KEY_ESC)
            {
                break;
            }
        }
    }

#ifdef SDK_ICC_TEST_DEBUG
    TestKillAllProcess();
#endif
}

void TestSdkIccMain(void)
{
    TestLog("Test", " \r\n **************************");
    TestLog("Test", "welcome start testing sdkIcc.c \r\n");
//���ͳ������
    sdkTestClrStatisticsData();
    TestSdkIcc();
//��ӡ�����Խ��
    sdkTestPrintStatisticsData();
    TestLog("Test", " \r\n \r\n**************************");
    TestLog("TestIcc", "\r\n end  testing sdkIcc.c \r\n");
    TestLog("Test", " *****************************\r\n");
}

#endif
#if 1 //del by sujianzhong 20120926

void TestIllPar_sdkIccInit(void) {
//    TestLog("emv" , "\r\nStart Test illegal param\r\n");
//    memset(&IccParam , 0 , sizeof(SDK_ICC_PARAM));
//    memset(&MagCardData , 0 , sizeof(struct _MagCardData));
//    memset(&qReaderDataRec , 0 , sizeof(struct _qReaderData));
//
//    TestLog("sdkIccReadMag Test" , "\r\n���Կ�ʼ\r\n");
//    TEST_IS_TRUE_WITH_INFO("�����������:SDK_ICC_ICC" , (sdkIccReadMag(SDK_ICC_ICC , SDK_ICC_CARDNO_MIN_NUM,&IccParam , &MagCardData) == SDK_PARA_ERR) );
//    TestLog("sdkIccReadMag Test" , "\r\n���Խ���\r\n");
//
//    TestLog("sdkIccReadIcc Test" , "\r\n���Կ�ʼ\r\n");
//    TEST_IS_TRUE_WITH_INFO("�����������:SDK_ICC_RF" , (sdkIccReadIcc(SDK_ICC_RF , &IccParam) == SDK_PARA_ERR) );
//    TestLog("sdkIccReadIcc Test" , "\r\n���Խ���\r\n");
//
//    TestLog("sdkIccDetect Test" , "\r\n���Կ�ʼ\r\n");
//    TEST_IS_TRUE_WITH_INFO("�����������:SDK_ICC_MANUAL" , (sdkIccDetect(SDK_ICC_MANUAL , 0 , 0 , &IccParam , &qReaderDataRec) == SDK_PARA_ERR) );
//    TestLog("sdkIccDetect Test" , "\r\n���Խ���\r\n");
}

void TestNormal_sdkIccInit(void) {
//    TestLog("emv" , "\r\nStart Test normal param\r\n");
//    memset(&IccParam , 0 , sizeof(SDK_ICC_PARAM));
//    memset(&MagCardData , 0 , sizeof(struct _MagCardData));
//    memset(&qReaderDataRec , 0 , sizeof(struct _qReaderData));
//
//    TestLog("sdkIccReadMag Test" , "\r\n���Կ�ʼ\r\n");
//    TEST_IS_TRUE_WITH_INFO("�����������:SDK_ICC_MAG" , (sdkIccReadMag(SDK_ICC_MAG , &IccParam , &MagCardData) == SDK_PARA_ERR) );
//    TestLog("sdkIccReadMag Test" , "\r\n���Խ���\r\n");
//
//    TestLog("sdkIccReadIcc Test" , "\r\n���Կ�ʼ\r\n");
//    TEST_IS_TRUE_WITH_INFO("�����������:SDK_ICC_ICC" , (sdkIccReadIcc(SDK_ICC_ICC , &IccParam) == SDK_PARA_ERR) );
//    TestLog("sdkIccReadIcc Test" , "\r\n���Խ���\r\n");
//
//    TestLog("sdkIccDetect Test" , "\r\n���Կ�ʼ\r\n");
//    TEST_IS_TRUE_WITH_INFO("�����������:SDK_ICC_RF" , (sdkIccDetect(SDK_ICC_RF , 0 , 0 , &IccParam , &qReaderDataRec) == SDK_PARA_ERR) );
//    TestLog("sdkIccDetect Test" , "\r\n���Խ���\r\n");
}

#if 0 /*Modify by yinpeng at 2013.01.18  17:32 */


static void TestIllPar_sdkIccCfgTerminalMsg()
{
    TestLog("emv", " \r\n \r\n��ʼsdkIccCfgTerminalMsg�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccCfgTerminalMsg(NULL, "123456789012345", "�¹���") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccCfgTerminalMsg("12345678", NULL, "�¹���") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccCfgTerminalMsg("12345678", "123456789012345", NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("pasTerminalNo's len less than 8", sdkIccCfgTerminalMsg("123456", "123456789012345", "�¹���") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("pasTerminalNo's len more than 8", sdkIccCfgTerminalMsg("123456789", "123456789012345", "�¹���") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("pasMerchantNo's len less than 15", sdkIccCfgTerminalMsg("12345678", "1234567890", "�¹���") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("pasMerchantNo's len more than 15 ", sdkIccCfgTerminalMsg("12345678", "123456789012345678", "�¹���") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("pasMerchantName's len more than 20", sdkIccCfgTerminalMsg("12345678", "123456789012345", "0�¹����¹����¹����¹����¹����¹����¹���") == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkIccCfgTerminalMsg()
{
    u8 tem[30] = {"0�¹����¹����¹����¹���"};

//    s32 i, len;

    tem[20] = 0;
    TestLog("emv", "\r\n normal test start \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccCfgTerminalMsg("12345678", "123456789012345", tem) == SDK_OK);
    TestLog("emv", "\r\n normal test end \r\n ");
}

static void Test_sdkIccCfgTerminalMsg()
{
    TestLog("emv", "\r\n test start : s32 sdkIccCfgTerminalMsg(const u8 *pasTerminalNo, const u8 *pasMerchantNo, const u8 *pasMerchantName)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccCfgTerminalMsg();
    //�����������
    TestNormal_sdkIccCfgTerminalMsg();
    TestLog("emv", "\r\ns32 sdkIccCfgTerminalMsg(const u8 *pasTerminalNo, const u8 *pasMerchantNo, const u8 *pasMerchantName) ���Խ���\r\n ");
}

static void TestIllPar_sdkIccCfgTermCapab()
{
    TestLog("emv", " \r\n \r\n��ʼsdkIccCfgTermCapab�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccCfgTermCapab(NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error", sdkIccCfgTermCapab("\x01\xFF") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error", sdkIccCfgTermCapab("\xFF\xFF\xFF\xFF") == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkIccCfgTermCapab()
{
    TestLog("emv", "\r\n normal test start \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccCfgTermCapab("\x03\xFF") == SDK_OK);
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccCfgTermCapab("\x03\xFF\xFF\xFF") == SDK_OK);
    TestLog("emv", "\r\n normal test end \r\n ");
}

static void Test_sdkIccCfgTermCapab()
{
    TestLog("emv", "\r\n test start : s32 sdkIccCfgTermCapab(u8 *pheLpIn)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccCfgTermCapab();
    //�����������
    TestNormal_sdkIccCfgTermCapab();
    TestLog("emv", "\r\n s32 sdkIccCfgTermCapab(u8 *pheLpIn) ���Խ���\r\n ");
}

static void TestIllPar_sdkIccCfgTermAddCapab()
{
    TestLog("emv", " \r\n \r\n��ʼsdkIccCfgTermAddCapab�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccCfgTermAddCapab(NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error", sdkIccCfgTermAddCapab("\x01\xFF") == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error", sdkIccCfgTermAddCapab("\xFF\xFF\xFF\xFF") == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkIccCfgTermAddCapab()
{
    TestLog("emv", "\r\n normal test start \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccCfgTermAddCapab("\x05\xFF") == SDK_OK);
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccCfgTermAddCapab("\x05\xFF\xFF\xFF\xFF\xFF") == SDK_OK);
    TestLog("emv", "\r\n normal test end \r\n ");
}

static void Test_sdkIccCfgTermAddCapab()
{
    TestLog("emv", "\r\n test start : s32 sdkIccCfgTermAddCapab(u8 *pheLpIn)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccCfgTermAddCapab();
    //�����������
    TestNormal_sdkIccCfgTermAddCapab();
    TestLog("emv", "\r\n s32 sdkIccCfgTermAddCapab(u8 *pheLpIn) ���Խ���\r\n ");
}

#endif

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.31
** Remarks:  //20140414
*****************************************************************************/

void TestSdkPSAMCArd(char testkey) {
    u8 *data;   //sxl?
    u8 cardModeEn, key;
    u32 len = 0;
    SDK_ICC_PARAM IccParam = {0};
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;

    cardModeEn = SDK_ICC_ICC;
    IccParam.ucCardMode = SDK_ICC_ICC;

    data = (u8 *) sdkGetMem(1024);

    if (data == NULL) {
        TestLog("", "Allocate for data[] Error!\r\n");
        return;
    }
    memset(data, 0, 1024);

//    sdkDispClearScreen();
//    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "pls select Card Type:", SDK_DISP_MDEFAULT);
//    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1->PSAM  2->SAM", SDK_DISP_MDEFAULT);
//    sdkDispBrushScreen();
//   key = sdkKbWaitKey(SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_ESC, SDK_ICC_TIMER_AUTO);
    if (testkey == '0') {
        IccParam.eCardType = SDK_ICC_PSAM;
        TestLog("PSAM", "select PSAM CARD!\r\n");
    }
    else if (testkey == '1') {
        IccParam.eCardType = SDK_ICC_SAM;
        TestLog("SAM", "select SAM CARD!\r\n");
    }
    else {
        TestLog("", "111ESC return!\r\n");
        return;
    }
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "pls input slot NO:", SDK_DISP_MDEFAULT);
    sdkDispBrushScreen();
    key = sdkKbGetScanf(SDK_ICC_TIMER_AUTO, data, 1, 1, SDK_MMI_NUMBER, SDK_DISP_LINE2);

    if (key == SDK_KEY_ENTER) {
        IccParam.ucSlotNo = atoi(data + 1);
        TestLog("emv", "Slot NO:%d\r\n", IccParam.ucSlotNo);
    }
    else {
        TestLog("emv", "222ESC return!\r\n");
        return;
    }

    if (sdkIccResetIcc(&IccParam, data, &len) != SDK_OK) {
        sdkTestIccDispText("reset err!");
        return;
    }
    else {
        u8 *buf;
        buf = (u8 *) sdkGetMem(1024);

        if (buf == NULL) {
            TestLog("", "Allocate for buf[] Error!\r\n");
            sdkFreeMem(data);
            return;
        }
        memset(buf, 0, sizeof(buf));
        strcpy(buf, "data:");
        sdkBcdToAsc(buf + strlen(buf), data + 1, len - 1); //��һ���ֽ��ǿ�����
        sdkTestIccDispText(buf);
        sdkTestIccDispText("reset suc!");
        sdkFreeMem(buf);
    }
    sdkFreeMem(data);
//    if(IccParam.eCardType == SDK_ICC_PSAM)
//    {
//        memset(&apdu_s, 0, sizeof(apdu_s));
//        memcpy(apdu_s.Command, "\x00\xB0\x95\x00", 4); //��PSAM�Ĺ�����Ϣ�ļ�
//        apdu_s.Lc = 0;
//        apdu_s.Le = 256;
//        apdu_s.EnableCancel = 1;       //ʹ�ܰ�ȡ����
//        memset(&apdu_r, 0, sizeof(apdu_r));
//        sdkIccDealCardData(&IccParam, &apdu_s, &apdu_r);
//
//        if(apdu_r.ReadCardDataOk == 0xAA || !(apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00))
//        {
//            // ������ʧ��
//            sdkTestIccDispText("PSAM read card err!");
//            return;
//        }
//        TestLog("PSAM", "SWA=%x, SWB=%x\r\n", apdu_r.SWA, apdu_r.SWB);
//        TestLogHex("PSAM", "Data", apdu_r.DataOut, apdu_r.LenOut);
//        memset(data, 0, sizeof(data));
//        strcpy(data, "CardNo:");
//        sdkBcdToAsc(data + strlen(data), apdu_r.DataOut, apdu_r.LenOut);
//        sdkTestIccDispText(data);
//        sdkTestIccDispText("PSAM read card suc!");
//    }
//    else if(IccParam.eCardType == SDK_ICC_SAM)
//    {
//        memset(&apdu_s, 0, sizeof(apdu_s));
//        memcpy(apdu_s.Command, "\x00\xA4\x04\x00", 4); //��PSAM�Ĺ�����Ϣ�ļ�
//        memcpy(apdu_s.DataIn, "\xA0\x00\x00\x00\x18\x50\x00\x00\x00\x00\x00\x00\x52\x41", 0x0E);
//        apdu_s.Lc = 0x0E;
//        apdu_s.Le = 256;
//        apdu_s.EnableCancel = 1;       //ʹ�ܰ�ȡ����
//        memset(&apdu_r, 0, sizeof(apdu_r));
//        sdkIccDealCardData(&IccParam, &apdu_s, &apdu_r);
//
//        if(apdu_r.ReadCardDataOk == 0xAA || !(apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00))
//        {
//            // ������ʧ��
//            sdkTestIccDispText("SAM read card err!");
//            return;
//        }
//        TestLog("SAM", "SWA=%x, SWB=%x\r\n", apdu_r.SWA, apdu_r.SWB);
//        TestLogHex("SAM", "Data", apdu_r.DataOut, apdu_r.LenOut);
//        memset(data, 0, sizeof(data));
//        strcpy(data, "DATA:");
//        sdkBcdToAsc(data + strlen(data), apdu_r.DataOut, apdu_r.LenOut);
//        sdkBcdToAsc(data + strlen(data), &apdu_r.SWA, 1);
//        sdkBcdToAsc(data + strlen(data), &apdu_r.SWB, 1);
//        sdkTestIccDispText(data);
//        sdkTestIccDispText("SAM read card suc!");
//    }
}

/*=======BEGIN: sujianzhong 2013.08.05  10:39 modify===========*/
bool TestSdkInputSysTime(void)  //20140414
{
    u8 key, error = 0, bcdtime[7] = {0}, ret, asctemp[5], ScanfTemp[128] = {0};
    u16 temp;

    while (1) {
        if (!error) {
            error = 1;
            memset(ScanfTemp, 0, sizeof(ScanfTemp));
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE4);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, "Time Format Err!", SDK_DISP_DEFAULT);
            sdkDispBrushScreen();

            if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ALL, 30000)) {
                return false;
            }
            error = 0;
            continue;
        }
        key = sdkKbGetScanf(30000, ScanfTemp, 14, 14, SDK_MMI_NUMBER | SDK_MMI_TIME,
                            SDK_DISP_LINE4);

        if (key == SDK_KEY_ESC) {
            return false;
        }
        else {
            if (ScanfTemp[0] == 14) {
                memset(asctemp, 0, sizeof(asctemp));
                memcpy(asctemp, &ScanfTemp[1], 4);                //ȡ���

                if (memcmp(asctemp, "19", 2) && memcmp(asctemp, "20", 2))                //���ʽ��
                {
                    continue;
                }
                temp = atoi(asctemp);

                if (temp < 1950 || temp > 2049) { continue; }
                sdkAscToBcd(bcdtime, &ScanfTemp[3], 12);
                ret = sdkJudgeTime(bcdtime, 6);

                if (ret) {
                    sdkSetRtc(bcdtime);
                    return true;
                }
            }
        }
    }
}

void TestSdkIccSetTime(void)  //20140414
{
    u8 key, tmp[20], lcdtemp[20], lcdtempOld[20];

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Set Time", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "[ESC] quite!", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    while (1) {
        memset(tmp, 0, sizeof(tmp));
        sdkGetRtc(tmp);
        memset(lcdtemp, 0, sizeof(lcdtemp));

        if (tmp[0] < 0x50) {
            sprintf(lcdtemp, "20%02x-%02x-%02x %02x:%02x:%02x", tmp[0],        //��ʾ����ϵͳʱ��
                    tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
        }
        else //2000��֮ǰ
        {
            sprintf(lcdtemp, "19%02x-%02x-%02x %02x:%02x:%02x", tmp[0],        //��ʾ����ϵͳʱ��
                    tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
        }

        if (strcmp(lcdtempOld, lcdtemp) != 0) {
            sdkDispClearRowRam(SDK_DISP_LINE4);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, lcdtemp, SDK_DISP_DEFAULT);
            sdkDispBrushScreen();
            strcpy(lcdtempOld, lcdtemp);
        }
        key = sdkKbGetKey();

        if (key == SDK_KEY_ESC) {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            return;
        }
        else if (key == SDK_KEY_BACKSPACE) {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            TestSdkInputSysTime();
            return;
        }
        else if (key == SDK_KEY_ENTER) {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            return;
        }
        else if (key != 0) {
            sdkSysBeep(SDK_SYS_BEEP_ERR);
        }
    }
}

/*====================== END======================== */
/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.07
** Remarks: //20140414
*****************************************************************************/
static void TestIllPar_sdkIccGetVersion() {
    // u8 temp[3];
    // u32 u;
    TestLog("emv", " \r\n \r\n��ʼsdkIccGetVersion�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", (sdkIccGetVersion(NULL) ==
                                                 SDK_PARA_ERR)); //shijianglong 2013.05.17 10:59
    // TEST_IS_TRUE_WITH_INFO("param's data is error", sdkIccGetVersion(temp)== 10 );
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.07
** Remarks:  //20140414
*****************************************************************************/

static void TestNormal_sdkIccGetVersion() {
    u8 temp[128] = {0};

    int len = sdkIccGetVersion(temp);

    if (len > sizeof(temp)) {
        FinalErr("���Դ������");
    }
    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccGetVersion(temp) > 0);
    //    TEST_IS_TRUE_WITH_INFO("normal test ", memcmp(temp,"123")==0);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.07
** Remarks:
*****************************************************************************/

static void Test_sdkIccGetVersion() {
    TestLog("emv", "\r\n test start : s32 sdkIccGetVersion(u8 *pasVersion)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetVersion();
    //�����������
    TestNormal_sdkIccGetVersion();
    TestLog("emv", "\r\n s32 sdkIccGetVersion(u8 *pasVersion)\r\n ");
}

#if 0
/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:
*****************************************************************************/

static void TestIllPar_sdkIccGetPAN()
{
    TestLog("emv", " \r\n \r\n��ʼsdkIccGetPAN�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccGetPAN(NULL) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:
*****************************************************************************/
static void TestNormal_sdkIccGetPAN()
{
    u8 temp[19] = {0};

    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ",  sdkIccGetPAN(temp) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:
*****************************************************************************/
static void Test_sdkIccGetPAN()
{
    TestLog("emv", "\r\n test start : s32 sdkIccGetPAN(u8 *pasOutPAN)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetPAN();
    //�����������
    TestNormal_sdkIccGetPAN();
    TestLog("emv", "\r\n s32 sdkIccGetPAN(u8 *pasOutPAN)\r\n ");
}

#endif

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/

static void TestIllPar_sdkIccDelOneAID() {
    TestLog("emv", " \r\n \r\n��ʼsdkIccDelOneAID�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccDelOneAID(NULL, 10) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error", sdkIccDelOneAID("1234567", -1) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:
*****************************************************************************/
static void TestNormal_sdkIccDelOneAID() {
    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccDelOneAID("1234567", 7) == SDK_ICC_NODATA);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/
static void Test_sdkIccDelOneAID() {
    TestLog("emv", "\r\n test start : s32 sdkIccDelOneAID(const u8 *pheInAID, s32 siAIDLen)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccDelOneAID();
    //�����������
    TestNormal_sdkIccDelOneAID();
    TestLog("emv", "\r\n s32 sdkIccDelOneAID(const u8 *pheInAID, s32 siAIDLen)\r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/

static void TestIllPar_sdkIccGetAIDListNum() {
    TestLog("emv", " \r\n \r\n��ʼsdkIccGetAIDListNum�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccGetAIDListNum(NULL) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/
static void TestNormal_sdkIccGetAIDListNum() {
    s32 temp = 0;

    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccGetAIDListNum(&temp) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/
static void Test_sdkIccGetAIDListNum() {
    TestLog("emv", "\r\n test start : s32 sdkIccGetAIDListNum(s32 *psiInAIDListNum)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetAIDListNum();
    //�����������
    TestNormal_sdkIccGetAIDListNum();
    TestLog("emv", "\r\n s32 sdkIccGetAIDListNum(s32 *psiInAIDListNum)\r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/

static void TestIllPar_sdkIccDelOneCAPK() {
    TestLog("emv", " \r\n \r\n��ʼsdkIccDelOneCAPK�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccDelOneCAPK(NULL, 3) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccDelOneCAPK("1234567", -1) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/
static void TestNormal_sdkIccDelOneCAPK() {
    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccDelOneCAPK("1234567", 3) == SDK_ICC_NODATA);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/
static void Test_sdkIccDelOneCAPK() {
    TestLog("emv",
            "\r\n test start : s32 sdkIccDelOneCAPK(const u8 *pheInRID, s32 siCAPKIndex)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccDelOneCAPK();
    //�����������
    TestNormal_sdkIccDelOneCAPK();
    TestLog("emv", "\r\ns32 sdkIccDelOneCAPK(const u8 *pheInRID, s32 siCAPKIndex)\r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/

static void TestIllPar_sdkIccGetCAPKListNum() {
    TestLog("emv", " \r\n \r\n��ʼsdkIccGetCAPKListNum�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccGetCAPKListNum(NULL) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/
static void TestNormal_sdkIccGetCAPKListNum() {
    s32 temp = 0;

    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccGetCAPKListNum(&temp) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.08
** Remarks:  //20140414
*****************************************************************************/
static void Test_sdkIccGetCAPKListNum() {
    TestLog("emv", "\r\n test start : s32 sdkIccGetCAPKListNum(s32 *psiInCAPKListNum)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetCAPKListNum();
    //�����������
    TestNormal_sdkIccGetCAPKListNum();
    TestLog("emv", "\r\nss32 sdkIccGetCAPKListNum(s32 *psiInCAPKListNum)\r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:  //20140414
*****************************************************************************/
static void TestIllPar_sdkIccConfigTLV() {
    TestLog("emv", " \r\n \r\n��ʼsdkIccGetCAPKListNum�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccConfigTLV(NULL, "12345", 5) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccConfigTLV("\x1F\x23", NULL, 5) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error",
                           sdkIccConfigTLV("\x00\x0F", "12345", 5) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error",
                           sdkIccConfigTLV("\xFF\x0F", "12345", 5) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error",
                           sdkIccConfigTLV("\x1F\x0F", "12345", -1) == SDK_PARA_ERR);

    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:  //20140414
*****************************************************************************/
static void TestNormal_sdkIccConfigTLV() {
    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccConfigTLV("\x1F\x12", "12345", 5) == SDK_ERR);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:  //20140414
*****************************************************************************/
static void Test_sdkIccConfigTLV() {
    TestLog("emv",
            "\r\n test start :s32 sdkIccConfigTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen) \r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccConfigTLV();
    //�����������
    TestNormal_sdkIccConfigTLV();
    TestLog("emv",
            "\r\nsss32 sdkIccConfigTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen) \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:  //20140414
*****************************************************************************/
static void TestIllPar_sdkIccReadTLV() {
    s32 len = 0;
    u8 *temp;
    s32 temp_size = 512;                        //liujingxuan

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return;
    }
    memset(temp, 0, temp_size);

    TestLog("emv", " \r\n \r\n��ʼsdkIccGetCAPKListNum�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccReadTLV(NULL, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccReadTLV("\x1F\x23", NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccReadTLV("\x1F\x23", temp, NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error",
                           sdkIccReadTLV("\x00\x0F", temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("param's data is error",
                           sdkIccReadTLV("\xFF\x0F", temp, &len) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");

    sdkFreeMem(temp);
    temp = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestNormal_sdkIccReadTLV() {
    s32 len = 0;
    u8 *temp;
    s32 temp_size = 512;                        //liujingxuan

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return;
    }
    memset(temp, 0, temp_size);
    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    //TEST_IS_TRUE_WITH_INFO("normal test", sdkIccReadTLV("\x9F\x02", temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("normal test", sdkIccReadTLV("\x9F\x02", temp, &len) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");

    sdkFreeMem(temp);
    temp = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void Test_sdkIccReadTLV() {
    TestLog("emv",
            "\r\n test start :s32 sdkIccReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen) \r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccReadTLV();
    //�����������
    TestNormal_sdkIccReadTLV();
    TestLog("emv",
            "\r\nssss32 sdkIccReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen) \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestIllPar_sdkIccGetOnlineRandNum() {
    TestLog("emv", " \r\n \r\n��ʼsdkIccGetCAPKListNum�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccGetOnlineRandNum(NULL) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestNormal_sdkIccGetOnlineRandNum() {
    u8 *temp;
    s32 temp_size = 100;                        //liujingxuan

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return;
    }
    memset(temp, 0, temp_size);

    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccGetOnlineRandNum(temp) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");

    sdkFreeMem(temp);
    temp = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void Test_sdkIccGetOnlineRandNum() {
    TestLog("emv", "\r\n test start :s32 sdkIccGetOnlineRandNum(u8 *pheOnlineRandNum)  \r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetOnlineRandNum();
    //�����������
    TestNormal_sdkIccGetOnlineRandNum();
    TestLog("emv", "\r\nssss32 sdkIccGetOnlineRandNum(u8 *pheOnlineRandNum)  \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestIllPar_sdkIccGetOnlineData() {
    TestLog("emv", " \r\n \r\n��ʼsdkIccGetCAPKListNum�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccGetOnlineData(SDK_OK, NULL, "12345", 5, "123456", 6) ==
                           SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccGetOnlineData(SDK_OK, "00", NULL, 5, "123456", 6) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccGetOnlineData(SDK_OK, "00", "12345", 5, NULL, 6) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestNormal_sdkIccGetOnlineData() {
    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ",
                           sdkIccGetOnlineData(SDK_OK, "00", "12345", 5, "123456", 6) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void Test_sdkIccGetOnlineData() {
    TestLog("emv",
            "\r\n test start :s32 sdkIccGetOnlineData(u8 ucOnlineResult, const u8 *pheRspCode, const u8 *pheIssuerAuthData, s32 siIssuerAuthDataLen, const u8 *pheScript, s32 siScriptLen) \r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetOnlineData();
    //�����������
    TestNormal_sdkIccGetOnlineData();
    TestLog("emv",
            "\r\nsssss32 sdkIccGetOnlineData(u8 ucOnlineResult, const u8 *pheRspCode, const u8 *pheIssuerAuthData, s32 siIssuerAuthDataLen, const u8 *pheScript, s32 siScriptLen) \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestIllPar_sdkIccGetScriptResult() {
    s32 len = 0;
    u8 *temp;
    s32 temp_size = 512;                        //liujingxuan

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return;
    }
    memset(temp, 0, temp_size);

    TestLog("emv", " \r\n \r\n��ʼsdkIccGetScriptResult�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccGetScriptResult(NULL, temp) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccGetScriptResult(&len, NULL) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");

    sdkFreeMem(temp);
    temp = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestNormal_sdkIccGetScriptResult() {
    s32 len = 0;
    u8 *temp;
    s32 temp_size = 512;                        //liujingxuan

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return;
    }
    memset(temp, 0, temp_size);

    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccGetScriptResult(&len, temp) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");

    sdkFreeMem(temp);
    temp = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void Test_sdkIccGetScriptResult() {
    TestLog("emv",
            "\r\n test start :s32 sdkIccGetScriptResult(s32 *psiScriptResultLen, u8 *pheScriptResult)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetScriptResult();
    //�����������
    TestNormal_sdkIccGetScriptResult();
    TestLog("emv",
            "\r\nsssss32 sdkIccGetScriptResult(s32 *psiScriptResultLen, u8 *pheScriptResult)\r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestIllPar_sdkIccGetECQueryMsg() {
    u8 temp[3] = {0};
    u8 temp1[6] = {0};
    u8 buf[3] = {0};
    u8 buf1[6] = {0};

    TestLog("emv", " \r\n \r\n��ʼsdkIccGetECQueryMsg�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccGetECQueryMsg(NULL, temp1, buf, buf1) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccGetECQueryMsg(temp, NULL, buf, buf1) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccGetECQueryMsg(temp, temp1, NULL, buf1) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccGetECQueryMsg(temp, temp1, buf, NULL) == SDK_PARA_ERR);
    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestNormal_sdkIccGetECQueryMsg() {
    u8 temp[3] = {0};
    u8 temp1[6] = {0};
    u8 buf[3] = {0};
    u8 buf1[6] = {0};

    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkIccGetECQueryMsg(temp, temp1, buf, buf1) == SDK_OK);
    TestLog("emv", "\r\n �����������Խ��� \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void Test_sdkIccGetECQueryMsg() {
    TestLog("emv",
            "\r\n test start :s32 sdkIccGetECQueryMsg(u8 *pbcFirstCurrencyCode, u8 *pbcFirstBalance, u8 *pbcSecondCurrencyCode, u8 *pbcSecondBalance) \r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccGetECQueryMsg();
    //�����������
    TestNormal_sdkIccGetECQueryMsg();
    TestLog("emv",
            "\r\nsssss32 sdkIccGetECQueryMsg(u8 *pbcFirstCurrencyCode, u8 *pbcFirstBalance, u8 *pbcSecondCurrencyCode, u8 *pbcSecondBalance) \r\n ");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestIllPar_sdkUpcardReadTLV() {
    s32 len = 0;
    u8 *temp;
    s32 temp_size = 512;                        //liujingxuan

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return;
    }
    memset(temp, 0, temp_size);

    TestLog("emv", " \r\n \r\n��ʼsdkUpcardReadTLV�Ƿ���������  \r\n \r\n");

#if 0           //sxl? ��Ҫ��дд
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkUpcardReadTLV(NULL, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkUpcardReadTLV("\xDF\x32", NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkUpcardReadTLV("\xDF\x32", temp, NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("error parem", sdkUpcardReadTLV("\x0F\x32", temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("error parem", sdkUpcardReadTLV("\xDF\x35", temp, &len) == SDK_PARA_ERR);
#endif

    TestLog("emv", " \r\n \r\n�Ƿ������������ \r\n \r\n");

    sdkFreeMem(temp);
    temp = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void TestNormal_sdkUpcardReadTLV() {
    s32 len = 0;
    u8 *temp;
    s32 temp_size = 512;                        //liujingxuan

    temp = (u8 *) sdkGetMem(temp_size);

    if (NULL == temp) {
        Assert(0);
        return;
    }
    memset(temp, 0, temp_size);

    TestLog("emv", "\r\n �����������Կ�ʼ \r\n");
#if 0           //sxl? ��Ҫ��дд
    TEST_IS_TRUE_WITH_INFO("normal test ", sdkUpcardReadTLV("\xDF\x32", temp, &len) == SDK_OK);
#endif
    TestLog("emv", "\r\n �����������Խ��� \r\n ");

    sdkFreeMem(temp);
    temp = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.18
** Remarks:
*****************************************************************************/
static void Test_sdkUpcardReadTLV() {
    TestLog("emv",
            "\r\n test start :s32 sdkUpcardReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen) \r\n ");

    //�Ƿ���������
    TestIllPar_sdkUpcardReadTLV();
    //�����������
    TestNormal_sdkUpcardReadTLV();
    TestLog("emv",
            "\r\n32 sdkUpcardReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen)\r\n ");
}

/*************************�Զ�����************************************************/

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.17
** Remarks:
******************************************************************************************/

void Test_sdkIccAuto(void) {
    if (0) //(sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM)//huacong 2013.03.28 11:48
    {
        sdkTestMsgBox("�û����ݲ�֧��IC��ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return;         //501��ʱ��֧��IC��ģ��
    }
    else {
        TestLog("TestEmv", " \r\n \r\n��ʼEMV�������� \r\n \r\n");
        Test_sdkIccGetVersion();
        Test_sdkIccDelOneAID();
        Test_sdkIccGetAIDListNum();
        Test_sdkIccDelOneCAPK();
        Test_sdkIccGetCAPKListNum();
        Test_sdkIccConfigTLV();      //sxl?
        Test_sdkIccReadTLV();
        Test_sdkIccGetOnlineRandNum();
        Test_sdkIccGetOnlineData();
        Test_sdkIccGetScriptResult();
        Test_sdkIccGetECQueryMsg();
        Test_sdkUpcardReadTLV();
        TestLog("TestEmv", " \r\n \r\nEMV  �������Խ���\r\n \rs\n");
        return;
    }
}

/**********�ֶ����Ժ���***********************************************************/

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.24
** Remarks:
*****************************************************************************/

static s32 Test_SdkIccEmvQpboc(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('0'); //QPBOC����
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvPboc(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('1'); //PBOC����
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvContact(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('2'); //EMV�Ӵ�����
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmv(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('3'); //QPBOC|PBOC����
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvSimp(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('4'); //�����̲���
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvDetail(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('5'); //��ϸ��ѯ����
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvQpbocRest(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('6'); //�����ֽ�����ѯ����
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvCardRest(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('7'); //����������ò���
    sdkIccTransFreeAllMem();
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvAid(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('8'); //����AID ����
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkIccEmvCapk(char *pasOutInfo, char const *pTitle) {
    TestSdkIccEmv('9'); //����CAPK����
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkUpCard(char *pasOutInfo, char const *pTitle) {
    TestSdkUpCard(); //�ֻ���
    sprintf(pasOutInfo, "  ");
    return 0;
}

static s32 Test_SdkPsamCard(char *pasOutInfo, char const *pTitle) {
    if (sdkTestIsUcos())    //��ʱ����Ҫ֧�֣�
    {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return -1;
    }
    TestSdkPSAMCArd('0'); // psam��
    sprintf(pasOutInfo, " ");
    return 0;
}

static s32 Test_SdkSamCard(char *pasOutInfo, char const *pTitle) {
    TestSdkPSAMCArd('1'); //sam��
    sprintf(pasOutInfo, " ");
    return 0;
}

static s32 Test_SdkIccSetTime(char *pasOutInfo, char const *pTitle) {
    TestSdkIccSetTime(); //sam��
    sprintf(pasOutInfo, " ");
    return 0;
}

static const TEST_MANUAL_MAP IccManaulMap[] =
        {
                {"QPBOC",         Test_SdkIccEmvQpboc},
                {"�ǽ�PBOC",       Test_SdkIccEmvPboc},
                {"�Ӵ�ʽPBOC/EMV",  Test_SdkIccEmvContact},
                {"QPBOC|�ǽ�PBOC", Test_SdkIccEmv},
                {"������",        Test_SdkIccEmvSimp},
                {"��ϸ��ѯ",        Test_SdkIccEmvDetail},
                {"�����ֽ�����ѯ",  Test_SdkIccEmvQpbocRest},
                {"�����������",   Test_SdkIccEmvCardRest},
                {"����AID",       Test_SdkIccEmvAid},
                {"����CAPK",      Test_SdkIccEmvCapk},
                {" �ֻ���",        Test_SdkUpCard},
                {" PSAM  ��",     Test_SdkPsamCard},
                {" SAM  ��",      Test_SdkSamCard},
                {"����ʱ��",       Test_SdkIccSetTime}
        }; //yinpeng 2013.01.24 15:53

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng  2013.01.25
** Remarks:   //20140414
*****************************************************************************/
TEST_MAP_INFO GetIccManualTestInfo() //yinpeng 2013.01.24 15:52
{
    TEST_MAP_INFO ret;

    ret.size = sizeof(IccManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = IccManaulMap;
    return ret;
}

#endif


//   ��          Ȩ: �¹���

