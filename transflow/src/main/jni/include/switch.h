#ifndef AFX_SWITCH_H
#define AFX_SWITCH_H

#define CUP_PROGRAM     0    			    //��׼��������
#define UMS_PROGRAM     1                   //��׼���̳���

#define hx_debug                            //debug���ط�����ر�
#undef hx_debug

#define ELECSIGN						    //����ǩ���ܿ���

#define ES_SIGNINPOS					    //��ʹ�����õ���ǩ����
#undef ES_SIGNINPOS						    //�����õ���ǩ�������ģʽ


#define PROGRAM_TYPE        CUP_PROGRAM

#if(PROGRAM_TYPE == CUP_PROGRAM)
//#define APPID               "YLCUP"
//#define APPNAME             "�й�����"
//#define APPDIR              "/mtd0/dll/"
#define APPID               "HXYBZ"         //huangxin 2015.01.12 17:55
#define APPNAME             "��������"          
#define APPDIR              "/mtd0/dll/"
//#define UNIONPAYVER  		"310028"
#define UNIONPAYVER  		"310030"
#elif(PROGRAM_TYPE == UMS_PROGRAM)
#define APPID               "CNCUP"
#define APPNAME             "��������"
#define APPDIR              "/mtd0/dll/"
//#define UNIONPAYVER       "311003"
#define UNIONPAYVER         "311004"        //lilin 20140616

//#define UMSVER            "31100301"
#define UMSVER  		    GetUmsVer()
#endif

//#define APPVERSION          ""APPID"140610703"
//#define APPVERSION          ""APPID"140617703"
//#define APPVERSION          ""APPID"140717706"
//#define APPVERSION          ""APPID"140812707"
//#define APPVERSION          ""APPID"140822708"
//#define APPVERSION          ""APPID"150406701"
//#define APPVERSION          ""APPID"150702702"
//#define APPVERSION              ""APPID"150707703"
//#define APPVERSION              ""APPID"150713704"
//#define APPVERSION              ""APPID"151208705"
//#define APPVERSION              ""APPID"151211706"
//#define APPVERSION              ""APPID"151230707"
//#define APPVERSION              ""APPID"160303708"
#define APPVERSION              ""APPID"160317709"

#endif

