#include "sdkGlobal.h"
#include "SdkTest.h"


#define  QUEUESIZE     50    //���ٲ� 2013.04.12 17:11 ���д�С


/*****************************************************************************
** Descriptions:	        ������Զ���
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.04.12
** Remarks:
*****************************************************************************/
void Test_sdkQueueNormal(void) {
    SDK_QUEUE_HAND pHand = NULL;
    u32 i = 0, j = 0, k = 0, m = 0;//, n = 0//shijianglong 2013.05.06 15:6

    u8 temp[QUEUESIZE] = {0};
    u8 tmp[QUEUESIZE] = {0};

    /*��ֵ��������*/
    for (i = 0; i < QUEUESIZE; i++) {
        temp[i] = (u8) i;
    }

    pHand = sdkQueueCreate(QUEUESIZE);


    /*1.�������в����Զ���*/
    TEST_IS_TRUE_WITH_INFO("�������ռ����", sdkQueueGetMaxSize(pHand) == QUEUESIZE);
    TEST_IS_TRUE_WITH_INFO("�������ݳ��Ȳ���", sdkQueueGetDataLen(pHand) == 0);
    TEST_IS_TRUE_WITH_INFO("�����Ƿ�Ϊ�ղ���", sdkQueueIsEmpey(pHand) == SDK_OK);
    TEST_IS_TRUE_WITH_INFO("�����Ƿ�������", sdkQueueIsFull(pHand) == SDK_ERR);
    TEST_IS_TRUE_WITH_INFO("����ʣ��ռ����", sdkQueueGetFreeSize(pHand) == QUEUESIZE);

    sdkTestShowProcessing("�����0%...");

    /*2.�������ݲ����Զ���*/
    for (i = 1; i <= QUEUESIZE; i++) //ÿ�β���i���ȵ�����
    {
        TEST_IS_TRUE_WITH_INFO("�������ݲ���", sdkQueueInsertData(pHand, temp, i) == i);

        TEST_IS_TRUE_WITH_INFO("�������ռ����", sdkQueueGetMaxSize(pHand) == QUEUESIZE);
        TEST_IS_TRUE_WITH_INFO("�������ݳ��Ȳ���", sdkQueueGetDataLen(pHand) == i);
        TEST_IS_TRUE_WITH_INFO("�����Ƿ�Ϊ�ղ���", sdkQueueIsEmpey(pHand) == SDK_ERR);

        if (i == QUEUESIZE) {
            TEST_IS_TRUE_WITH_INFO("�����Ƿ�������", sdkQueueIsFull(pHand) == SDK_OK);
        }
        else {
            TEST_IS_TRUE_WITH_INFO("�����Ƿ�������", sdkQueueIsFull(pHand) == SDK_ERR);
        }
        TEST_IS_TRUE_WITH_INFO("����ʣ��ռ����", sdkQueueGetFreeSize(pHand) == QUEUESIZE - i);

//ȡ��������ֵѭ������
        for (j = 0; j < i; j++) {
            TEST_IS_TRUE_WITH_INFO("����ȡֵ����", sdkQueueGetPosValue(pHand, j) == temp[j]);
        }

//ȡ���ݿ�Ѱ������
        for (j = 1; j < i; j++) {
//            printf("\r\nsdkQueueGetData = %d j = %d\r\n", sdkQueueGetData(pHand, tmp, j), j);
            TEST_IS_TRUE_WITH_INFO("�������ݿ��ȡ����", sdkQueueGetData(pHand, tmp, j) == j);

            for (k = 0; k < j; k++) //�Ƚ�����
            {
                TEST_IS_TRUE_WITH_INFO("�������ݿ����ݱȽϲ���", tmp[k] == temp[k]);
            }
        }

//�Ƴ����ݿ�ѭ������
        for (j = 0; j < i; j++) {
            for (j = i; j < k; k--) {
                TEST_IS_TRUE_WITH_INFO("�������ݿ��Ƴ�����",
                                       sdkQueueRemoveRange(pHand, j, k) == i - (k - j));

                //�����Ƴ��������������
                TEST_IS_TRUE_WITH_INFO("�������ݿ��ȡ����", sdkQueueGetData(pHand, tmp, QUEUESIZE) ==
                                                          i - (k - j)); //ȡ��ȫ������

                for (m = 0; m < i - (k - j); m++) {
                    if (m < j) {
                        TEST_IS_TRUE_WITH_INFO("�������ݿ����ݱȽϲ���", tmp[m] == temp[m]);
                    }
                    else {
                        TEST_IS_TRUE_WITH_INFO("�������ݿ����ݱȽϲ���", tmp[m] == temp[m + (k - j)]);
                    }
                }

                TEST_IS_TRUE_WITH_INFO("�ָ��Ƴ����ݲ���", sdkQueueInsertData(pHand, temp, i) == i);
            }
        }

//�������ѭ������
        TEST_IS_TRUE_WITH_INFO("����������ݲ���", sdkQueueEmpey(pHand) == SDK_OK);
        {
            u8 process[128] = {0};

            sprintf(process, "�����%d%...", i);
            sdkTestShowProcessing(process);
        }
    }

    /*5.�ݻٶ��У����Խ���*/
    TEST_IS_TRUE_WITH_INFO("�ݻٶ��в���", sdkQueueRelease(pHand) == SDK_OK);
    sdkTestShowProcessing("�����50%...");
}

/*****************************************************************************
** Descriptions:	        �����쳣����
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.04.12
** Remarks:
*****************************************************************************/
void Test_sdkQueueUnusual(void) {
    SDK_QUEUE_HAND pHand = NULL;
    u8 temp[QUEUESIZE] = {0};
    s32 i = 0;

    /*��ֵ��������*/
    for (i = 0; i < QUEUESIZE; i++) {
        temp[i] = (u8) i;
    }

    pHand = sdkQueueCreate(QUEUESIZE);


    TEST_IS_TRUE_WITH_INFO("���������쳣����", sdkQueueCreate(10) == NULL);
    TEST_IS_TRUE_WITH_INFO("��ն����쳣����", sdkQueueEmpey(NULL) == SDK_PARA_ERR);


    TEST_IS_TRUE_WITH_INFO("����ȡ�����쳣����1",
                           sdkQueueGetData(NULL, temp, QUEUESIZE) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("����ȡ�����쳣����2", sdkQueueGetData(pHand, temp, QUEUESIZE) == 0);
    sdkTestShowProcessing("�����70%...");


    TEST_IS_TRUE_WITH_INFO("ȡ���г����쳣����", sdkQueueGetDataLen(NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("ȡ����ʣ�೤���쳣����", sdkQueueGetFreeSize(NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("ȡ������󳤶��쳣����", sdkQueueGetMaxSize(NULL) == SDK_PARA_ERR);

    TEST_IS_TRUE_WITH_INFO("ȡ����ֵ�쳣����", sdkQueueGetPosValue(NULL, 0) == 0);

    TEST_IS_TRUE_WITH_INFO("��ն����쳣����1", sdkQueueInsertData(NULL, temp, 0) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("��ն����쳣����2", sdkQueueInsertData(pHand, temp, -1) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("��ն����쳣����3",
                           sdkQueueInsertData(pHand, temp, QUEUESIZE + 1) == SDK_PARA_ERR);

    TEST_IS_TRUE_WITH_INFO("�ж϶���Ϊ���쳣����", sdkQueueIsEmpey(NULL) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("�ж϶���Ϊ���쳣����", sdkQueueIsFull(NULL) == SDK_PARA_ERR);
    sdkTestShowProcessing("�����80%...");

    TEST_IS_TRUE_WITH_INFO("�������ݿ��Ƴ��쳣����1", sdkQueueRemoveRange(NULL, 1, 3) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("�������ݿ��Ƴ��쳣����2", sdkQueueRemoveRange(pHand, 1, 3) == SDK_PARA_ERR);
    sdkQueueInsertData(pHand, temp, 100);
    TEST_IS_TRUE_WITH_INFO("�������ݿ��Ƴ��쳣����3", sdkQueueRemoveRange(pHand, 3, 1) == SDK_PARA_ERR);

    TEST_IS_TRUE_WITH_INFO("�ݻٶ����쳣����", sdkQueueRelease(NULL) == SDK_PARA_ERR);

    sdkTestShowProcessing("�����100%...");
    sdkQueueRelease(pHand); //���ǵôݻٶ���
}

/*****************************************************************************
** Descriptions:	        �Զ�����
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.04.14
** Remarks:
*****************************************************************************/
void Test_sdkQueueAuto(void) {
    Test_sdkQueueNormal();
    Test_sdkQueueUnusual();
}

