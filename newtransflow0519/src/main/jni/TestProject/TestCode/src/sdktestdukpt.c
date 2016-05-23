#include "sdkGlobal.h"
#include "SdkTest.h"

typedef struct TEST_DUKPT_DATA_ST {
    u8 ucKsn[10];
    u8 ucPinBlock[8];
    u8 ucMac[8];
} TESTDUKPTDATAST;

void Test_sdkDukptAuto(void) {
    u8 ksnLenght;
    u8 temp[30], times = 0;
    u8 key[16] =
            {
                    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
            },
            ksn[10] =
            {
                    0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0xE0,
                    0x00, 0x00
            }, ksn1[10] = {0}, ksn2[10] = {0},
            pan[6] = {0x40, 0x12, 0x34, 0x56, 0x78, 0x90}, mac[8], mode = 0;
    s32 i = 0, j = 0;
    bool flag = false, bakstatu = 0, rfbak = 0;
    SDK_PED_PIN_CFG pedpincfg;

    SDK_SYS_INITIAL_INFO sjkao;


    TESTDUKPTDATAST testdukptdatast[6] =
            {
                    {
                            {0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0xE0, 0x00, 0x01},
                            {0x1B, 0x9C, 0x18, 0x45, 0xEB, 0x99, 0x3A, 0x7A},
                            {0x9C, 0xCC, 0x78, 0x17, 0x3F, 0xC4, 0xFB, 0x64},
                    },
                    {
                            {0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0xE0, 0x00, 0x02},
                            {0x10, 0xA0, 0x1C, 0x8D, 0x02, 0xC6, 0x91, 0x07},
                            {0xF6, 0x08, 0xA9, 0xBC, 0xA6, 0xFF, 0xC3, 0x11},
                    },
                    {
                            {0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0xE0, 0x00, 0x03},
                            {0x18, 0xDC, 0x07, 0xB9, 0x47, 0x97, 0xB4, 0x66},
                            {0x20, 0xB5, 0x9A, 0x4F, 0xEA, 0xC9, 0x37, 0xE3},
                    },
                    {
                            {0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0xE0, 0x00, 0x04},
                            {0x0B, 0xC7, 0x95, 0x09, 0xD5, 0x64, 0x5D, 0xF7},
                            {0xC7, 0xBF, 0xA6, 0xCC, 0x44, 0x16, 0x18, 0x28},
                    },
                    {
                            {0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0xE0, 0x00, 0x05},
                            {0x5B, 0xC0, 0xAF, 0x22, 0xAD, 0x87, 0xB3, 0x27},
                            {0x02, 0x02, 0xB9, 0x63, 0x39, 0x02, 0x20, 0x58},
                    },
                    {
                            {0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0xE0, 0x00, 0x06},
                            {0xA1, 0x6D, 0xF7, 0x0A, 0xE3, 0x61, 0x58, 0xD8},
                            {0xCF, 0x6C, 0x72, 0xE6, 0xA4, 0x98, 0x92, 0xD5},
                    },

            };
    //sdkDispMsgBox("DUKPT�Զ�����ģ��", "������չ���в���\r\n������˳�", 0, SDK_KEY_MASK_ALL);
    //return ;
    bakstatu = sdkPEDIsWithPinpad();
    rfbak = sdkSysIsRfIn();
    //sdkSysInitParam(&sjkao);

    pedpincfg.eKeyType = SDK_PED_DES_DUKPT;
    pedpincfg.ePinMode = SDK_PED_MAG_PIN;
    pedpincfg.iTimeOut = 30000;
    pedpincfg.ucTpkIndex = 0;
    pedpincfg.ucRow = 2;
    memcpy(&pedpincfg.hePan[2], pan, 6);


#if((MACHINEMODEL == SDK_SYS_MACHINE_K501AM) || (MACHINEMODEL == SDK_SYS_MACHINE_K508A))

    sdkTestMsgBox("�û����ݲ�֧��DUKPTģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
    return;
#else
    {
        TestLog("zhouzhihua","dukpt init start\r\n");
        TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDDukptLoadAndInit(SDK_PED_DUKPT_UPDATE_ALL, NULL, 16, temp,10,500)== SDK_PARA_ERR));
        TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDDukptLoadAndInit(SDK_PED_DUKPT_UPDATE_ALL, temp, 16, NULL ,10,500)== SDK_PARA_ERR));

        TEST_IS_TRUE_WITH_INFO("��������", (sdkPEDDukptLoadAndInit(SDK_PED_DUKPT_UPDATE_ALL, key, 16, ksn ,10,500)== SDK_OK));                                                                                              
        TestLog("zhouzhihua","dukpt init end\r\n");

        TestLog("zhouzhihua","dukpt pin&mac start\r\n");
                                              
        TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDDukptGetPin(SDK_PED_DUKPT_GET_PIN_RELEASE, 0, &pedpincfg, 4, 12, NULL, ksn, &ksnLenght, 3000)== SDK_PARA_ERR));
        TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDDukptGetPin(SDK_PED_DUKPT_GET_PIN_RELEASE, 0, &pedpincfg, 4, 12,temp , NULL, &ksnLenght,3000)== SDK_PARA_ERR));
        j = 0;
        while(1) //���Ե�Ĭ������1234
        {
            memset(ksn1 , 0, sizeof(ksn1));
            memset(temp , 0, sizeof(temp));
            memset(ksn2 , 0, sizeof(ksn2));
            memset(mac , 0, sizeof(mac));
            if(times == 0)
            {
                sjkao.bIsHavePinPad = 1;
                sdkSysInitParam(&sjkao);
            }
            else if(times >= 6)
            {
                sjkao.bIsHavePinPad = 0;
                sdkSysInitParam(&sjkao);
            }
            mode = sdkPEDIsWithPinpad() ? (SDK_PED_DUKPT_GET_PIN_DEBUG):(SDK_PED_DUKPT_GET_PIN_RELEASE);

            TEST_IS_TRUE_WITH_INFO("��������",(sdkPEDDukptGetPin(mode, 0, &pedpincfg, 4, 12,temp, ksn1, &ksnLenght, 30000) > 0));
            TEST_IS_TRUE_WITH_INFO("��������",(sdkPEDDukptGetMac(SDK_PED_DUKPT_GET_MAC_OLD, 0, "4012345678909D987", 17,mac, ksn2, &ksnLenght, 500)> 0));
            TestLog("zhouzhihua" ,"------------------------------\r\n");
            TestLogHex("zhouzhihua" ,"mac",mac,8);
            TestLogHex("zhouzhihua" ,"ksn2",ksn2,10);
            TestLog("zhouzhihua" ,"------------------------------\r\n");

            flag = false;
            for(i = 0 ; i < 6 ; i++)
            {
                if( !memcmp(testdukptdatast[i].ucKsn , ksn1 , 10) )
                {
                    flag = true;
                    TEST_IS_TRUE_WITH_INFO("��������PIN",   memcmp(temp , testdukptdatast[i].ucPinBlock, 8) == 0);
                }
               
            }
            TEST_IS_TRUE_WITH_INFO("dukpt pin",  flag);
            flag = false;
            for(i = 0 ; i < 6 ; i++)
            {
                if( !memcmp(testdukptdatast[i].ucKsn , ksn2 , 10) )
                {
                    flag = true;
                    TEST_IS_TRUE_WITH_INFO("��������MAC",   memcmp(mac , testdukptdatast[i].ucMac, 8) == 0);
                }
            }
            times++;

          
            TEST_IS_TRUE_WITH_INFO("dukpt mac",  flag);
            if(times >= 12)
            {
                sjkao.bIsHavePinPad = bakstatu;
                sjkao.bIsINRf = rfbak;
                return ;
            }
        }
        TestLog("zhouzhihua","dukpt pin&mac end\r\n");
    }
#endif
}

























