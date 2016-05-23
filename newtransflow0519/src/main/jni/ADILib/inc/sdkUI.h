#ifndef __SDK_UI_H__
#define __SDK_UI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum {
    SDK_UI_SHOW_MODE_1_ROW = 0,            //1.XX
    //2.XX
    //3.XX
    //4.XX

    //5.XX
    //6.XX
    //7.XX
    //8.XX
/*���ٵ���֧�֣��Զ�תΪSDK_UI_SHOW_MODE_1_ROW�����һ����Ҫ֧�֣���������	
    sdkUISetModeitemNum(4)��ʵ��
 */
            SDK_UI_SHOW_MODE_1_ROW1,            //1.XX
    //2.XX
    //3.XX
    //4.XX

    //1.XX
    //2.XX
    //3.XX
    //4.XX

    SDK_UI_SHOW_MODE_2_ROW1,            //1.XX   2.XX
    //3.XX   4.XX

    SDK_UI_SHOW_MODE_2_ROW2,            //1.XX   3.XX
    //2.XX   4.XX

    SDK_UI_SHOW_MODE_NOFDISP_1_ROW,        //1.XX
    //2.XX
    //3.XX
    //4.XX

    //5.XX
    //6.XX
    //7.XX
    //8.XX(������ѡ��˵�ģʽ)
/*���ٵ���֧�֣��Զ�תΪSDK_UI_SHOW_MODE_1_ROW�����һ����Ҫ֧�֣���������	
    sdkUISetModeitemNum(4)��ʵ��
 */
            SDK_UI_SHOW_MODE_NOFDISP_1_ROW1,    //1.XX
    //2.XX
    //3.XX
    //4.XX

    //1.XX
    //2.XX
    //3.XX
    //4.XX(������ѡ��˵�ģʽ)

    SDK_UI_SHOW_MODE_NOFDISP_2_ROW1,    //1.XX   2.XX
    //3.XX   4.XX (������ѡ��˵�ģʽ)

    SDK_UI_SHOW_MODE_NOFDISP_2_ROW2,    //1.XX   3.XX
    //2.XX   4.XX (������ѡ��˵�ģʽ)

    SDK_UI_SHOW_MODE_Err
} SDK_UI_SHOW_MODE;

typedef enum {
    SDK_UI_EXIT_INTERNAL_ERR = -3,                     // �ڲ�����//internal error
    SDK_UI_EXIT_TIME_OUT = -4,                      //��ʱ//timeout
    SDK_UI_EXIT_FORCE_OUT = -5,                     //ǿ���˳�//force out
    SDK_UI_EXIT_PARA_ERR = -1,                      //��������//parameters error
    SDK_UI_EXIT_ENTER = 1,                      //�û���ȷ���˳�//user press ENTER to quit
    SDK_UI_EXIT_CANCLE,                                                       //�û���ȡ��//user press ESC
    SDK_UI_EXIT_ERR_VALUE                                              //��ʼ��ֵ��ʾ����//initialize value error
} SDK_UI_EXIT_VALUE;

//�˵��ص�����ԭ�ͣ�ִ�в˵�ʱ����Ӳ˵�ʱ��˳����Զ���ֵ��siIndex����ص�����
//�Ա����ڶ���˵���Ӧͬһ���ص�����ʱ���ص������������ĸ��˵������˴˻ص�����
typedef SDK_UI_EXIT_VALUE (*MenuCallBackFun)(s32 siIndex);

extern s32 sdkUISetTile(const u8 *pasTile);

extern s32 sdkUISetShowMode(SDK_UI_SHOW_MODE mode);

extern s32 sdkUIAddItem(const u8 *pasTile, MenuCallBackFun pFun);

extern s32 sdkUIShowMenu(void);

extern s32 sdkUIRevKey(s32 key);

extern s32 sdkUIClearItems(void);

extern s32 sdkUIAddHideItem(s32 key, MenuCallBackFun pFun);

extern s32 sdkUISetModeitemNum(s32 siModeNum);

extern s32 sdkUISetSelectPosition(s32 siCurrentNoDisp, s32 siItemNum);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


