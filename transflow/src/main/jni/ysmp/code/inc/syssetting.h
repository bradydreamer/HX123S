#ifndef _SYS_SETTING_H_
#define _SYS_SETTING_H_


#define  CommuSettingFile        "/mtd0/mpos/commuSetting.dt"

typedef struct CommSettingStr {
    u8 commuType[2];
} COMM_SETTING_STR;

extern COMM_SETTING_STR gCommSetting;


extern SDK_UI_EXIT_VALUE OnSysSetting(s32 ix);

extern s32 CommuSettingMenu();

extern s32 BtSettingMenu();

extern SDK_UI_EXIT_VALUE OnDisconnectBt(s32 siIndex);

extern SDK_UI_EXIT_VALUE OnOpenBt(s32 siIndex);

extern SDK_UI_EXIT_VALUE OnCloseBt(s32 siIndex);

extern SDK_UI_EXIT_VALUE OnSetBtDiscoverable(s32 siIndex);

extern SDK_UI_EXIT_VALUE OnSetBtNameAndPwd(s32 siIndex);

extern SDK_UI_EXIT_VALUE VersionMenu(s32 siIndex);                //20150317 ��Ӱ汾��ѯ

#endif
