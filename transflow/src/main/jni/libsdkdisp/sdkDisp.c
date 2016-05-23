#include "sdkdevOption.h"
#include "sdkGlobal.h"
#include "sdkExGlobal.h"
#include <stdarg.h> //lint !e451
#include "iconBitMap.h"

#ifndef  DEBUG_LCD
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#define ALIGN_LEFT      0 //�����
#define ALIGN_CENTER    1 //����
#define ALIGN_RIGHT     2 //�Ҷ���

#define ORIGIN_IS_TOP_LEFT_CORNER   0 //ԭ����ʼλ��: falseΪ��Ļ���Ͻǣ� trueΪ ��Ļ���Ͻ�����ƫ��״̬����ռ�߶�

/*��Ļ������������*/
#if MACHINEMODEL == SDK_SYS_MACHINE_200P /*Modify by ���ƽ at 2014.08.12  14:54 */
#define LCD_STATUS_BAR_HEIGHT           12 //״̬���߶�
#define LCD_DEFAULT_ASCII_FONT          E_WORD_ASCII_6_12 //Ĭ��ascii����
#define LCD_DEFAULT_NATIVE_FONT         E_WORD_ASCII_6_12 //Ĭ�Ϲ�������
#define LCD_PIXEL_BITS               1 //�ڰ���
#else
#define LCD_STATUS_BAR_HEIGHT                20 //״̬���߶�
#define LCD_DEFAULT_ASCII_FONT          E_WORD_ASCII_12_24 //Ĭ��ascii����
#define LCD_DEFAULT_NATIVE_FONT         E_WORD_NATIVE_24_24 //Ĭ�Ϲ�������
#define LCD_PIXEL_BITS               32 //�����
#endif /* if 0 */

#if LCD_PIXEL_BITS == 1 //�ڰ���
#define LCD_PROG_BAR_COLOR              0 //������Ĭ����ɫ
#define LCD_BRUSH_COLOR                 1 //��ˢĬ����ɫ

#elif LCD_PIXEL_BITS == 32 //�����
#define LCD_PROG_BAR_COLOR              ((98 << 16) | (167 << 8) | (40))  //������Ĭ����ɫ :���ﲻ֪����ʲô��ɫ������98 167 40
#define LCD_BRUSH_COLOR                 ((255 << 16) | (255 << 8) | (255)) //��������ˢ��ɫ

#endif /* if 0 */


typedef struct SDK_LCD_ROW_INFO {
    strFont str_font; //��ǰϵͳ������ʽ
    u32 brush_color; //��ˢ��ɫ(Ĭ�ϰ�ɫ)

    u16 lcd_width; //lcd ��
    u16 lcd_height; //lcd ��

    SDK_SYS_LINE_MODE row_counts; //��ģʽ
    u16 row_height; //�и�
    u16 row_width; //�п�

    /*ʵ����ʾ����*/
    u16 show_lcd_left;
    u16 show_lcd_right;
    u16 show_lcd_top;
    u16 show_lcd_bottom;
    u16 show_lcd_width; //lcd ��  = show_lcd_right - show_lcd_left;
    u16 show_lcd_height; //lcd �� = show_lcd_bottom - show_lcd_top;

    u16 row_x_offset_temp; //ˮƽƫ�ƣ� �洢��ǰ��һ����ʱֵ

    u8 lcd_brightness; //�ڰ���Ϊ�Աȶȣ�����Ϊ�����̶�
    bool is_show_stat_bar; //�Ƿ���ʾ״̬����true:show, false:not show
} SDK_LCD_ROW_INFO;

static SDK_LCD_ROW_INFO g_row_info = {0};
static s32 g_power_state = DDI_POWER_ZERO; //��¼���״̬



void sdkDispGetSysFont(SDK_FONT *currfont, u8 flag);

static s32 sdkDispGetAsciiWH(u32 m_ascsize, u32 *asc_width, u32 *asc_height);

static s32 sdkDispGetNativeWH(u32 m_nativesize, u32 *word_width, u32 *word_height);


void sdk_warning_fun_not_work(u8 *pFunName) {
    sdkDispClearScreen();
    sdkDispRowAt(1, 0, pFunName, 0x07);
    sdkDispRowAt(2, 0, "not work", 0x07);
    sdkDispBrushScreen();
    Trace("devlcd", "fun not work :%s\r\n", pFunName);
    {
//        OS_ERR tem; OSTimeDly(1000, 0, &tem);//sujianzhong 2015.05.20 17:23
    }
}

/*****************************************************************************
** Descriptions:	��ʼ����ģʽ����
** Parameters:          s32 max_row
** Returned value:
** Created By:		���ƽ  2014.04.23
** Remarks:
*****************************************************************************/
s32 sdkDispSetMaxRow(SDK_SYS_LINE_MODE max_row)  //Ĭ��Ϊȫ��
{
    u16 row_height;
    u16 row_rev;
    u32 max_word_height = 0;
    u32 max_ascii_height = 0;

    if (g_row_info.show_lcd_height == 0) {
        Assert(0);
        return SDK_ERR;
    }
    sdkDispGetNativeWH(g_row_info.str_font.m_nativesize, NULL, &max_word_height);
    sdkDispGetAsciiWH(g_row_info.str_font.m_ascsize, NULL, &max_ascii_height);
    max_word_height = max_word_height > max_ascii_height ? max_word_height : max_ascii_height;

    if (max_row * max_word_height > g_row_info.show_lcd_height) //Ĭ������:G2Ϊ24������, 200PΪ12������
    {
        return SDK_PARA_ERR;
    }
    else {
        g_row_info.row_counts = max_row;
        sdkSysSetSreenMaxLine(max_row);
        row_height = g_row_info.show_lcd_height / g_row_info.row_counts;
        row_rev = g_row_info.show_lcd_height % g_row_info.row_counts;

        if (row_rev + 1 == g_row_info.row_counts) //ʣ��ռ�պ� = ���� - 1; ���Բ��뵽ÿһ���м�϶��
        {
            row_height++;
        }
        else //��ʣ��ռ仮�ֵ���Ļ����
        {
            g_row_info.show_lcd_top += row_rev >> 1;
        }
        g_row_info.row_height = row_height; //�и�
        g_row_info.row_width = g_row_info.show_lcd_width; //�п�
        return SDK_OK;
    }
}

/*****************************************************************************
** Descriptions:	��ȡascii�ַ��Ŀ��
** Parameters:          s32 *word_width
                    s32 *word_height
** Returned value:
** Created By:		���ƽ  2014.04.23
** Remarks:
*****************************************************************************/
static s32 sdkDispGetAsciiWH(u32 m_ascsize, u32 *asc_width, u32 *asc_height) {
    u32 u_width = 0;
    u32 u_height = 0;

    /*�õ���ǰ��������߶�*/
    switch (m_ascsize) {
        case E_WORD_ASCII_6_12: {
            u_height = 12;
            u_width = 6;
        }
            break;

        case E_WORD_ASCII_8_16: {
            u_height = 16;
            u_width = 8;
        }
            break;

        case E_WORD_ASCII_12_24: {
            u_height = 24;
            u_width = 12;
        }
            break;

        case E_WORD_ASCII_16_32: {
            u_height = 32;
            u_width = 16;
        }
            break;

        case E_WORD_ASCII_24_48: {
            u_height = 48;
            u_width = 24;
        }
            break;

        default: {
            return SDK_ERR;
        }
            //break;
    }

    if (NULL != asc_width) {
        *asc_width = u_width;
    }

    if (NULL != asc_height) {
        *asc_height = u_height;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    ��ȡ�����ַ��Ŀ��
** Parameters:          s32 *word_width
                    s32 *word_height
** Returned value:
** Created By:		���ƽ  2014.04.23
** Remarks:
*****************************************************************************/
static s32 sdkDispGetNativeWH(u32 m_nativesize, u32 *word_width, u32 *word_height) {
    u32 u_width = 0;
    u32 u_height = 0;

    /*�õ���ǰ��������߶�*/
    switch (m_nativesize) {
        case E_WORD_NATIVE_12_12: {
            u_width = 12;
            u_height = 12;
        }
            break;

        case E_WORD_NATIVE_16_16: {
            u_width = 16;
            u_height = 16;
        }
            break;

        case E_WORD_NATIVE_20_20: {
            u_width = 20;
            u_height = 20;
        }
            break;

        case E_WORD_NATIVE_24_24: {
            u_width = 24;
            u_height = 24;
        }
            break;

        case E_WORD_NATIVE_32_32: {
            u_width = 32;
            u_height = 32;
        }
            break;

        case E_WORD_NATIVE_48_48: {
            u_width = 48;
            u_height = 48;
        }
            break;

        default: {
            return SDK_ERR;
        }
            //break;
    }

    if (NULL != word_width) {
        *word_width = u_width;
    }

    if (NULL != word_height) {
        *word_height = u_height;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ�ַ�������ǰ������ʾ�Ŀ��
** Parameters:          u32 row_index
                    u8 *pstr
                    u32 *uiX
                    u32 *uiY
** Returned value:
** Created By:		���ƽ  2014.04.24
** Remarks:
*****************************************************************************/
s32 sdkDispGetStrWH(u8 *pstr, u32 *str_width, u32 *str_height) {
    u32 assii_max_width;
    u32 assii_max_height;
    u32 word_max_width;
    u32 word_max_height;
    u32 i = 0;
    u32 len = strlen((s8 *) pstr);


    /*�õ���ǰ����Ŀ��*/
    sdkDispGetAsciiWH(g_row_info.str_font.m_ascsize, &assii_max_width, &assii_max_height);
    sdkDispGetNativeWH(g_row_info.str_font.m_nativesize, &word_max_width, &word_max_height);

    if (str_height != NULL) {
        *str_height = assii_max_height > word_max_height ? assii_max_height : word_max_height;
    }

    /*�����ı����Ŀ��*/
    if (str_width != NULL) {
        *str_width = 0;

        for (i = 0; i < len; i++) {
            if ((u8) pstr[i] >= 0x80) //��ascii
            {
                i++;
                *str_width += word_max_width;
            }
            else {
                *str_width += assii_max_width;
            }
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ�е���ʼ����
** Parameters:          s32 row_index >= 0 < row_counts
                    s32 *siX
                    s32 *siY
** Returned value:
** Created By:		���ƽ  2014.04.23
** Remarks:             �п��Ǹ���������������Ļ�߶���ƽ��
*****************************************************************************/
static s32 sdkDispGetRowXY(u32 row_index, u32 *uiX, u32 *uiY) {
    if (NULL != uiX) {
        *uiX = g_row_info.show_lcd_left + g_row_info.row_x_offset_temp; //��X��ʼλ��
    }

    if (NULL != uiY) {
        *uiY = g_row_info.show_lcd_top + row_index * g_row_info.row_height;  //��Y��ʼλ��
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ�����ı���ʾ����ʼ����
** Parameters:          s32 row_index >= 0 < row_counts
                    u32 align ���뷽ʽ
                    u8 *pstr
                    s32 *siX
                    s32 *siY
** Returned value:	������ʾ��ʵ���ַ�����( ���ַ�̫��ʱ����ص������޷���ʾ�Ĳ��� )
** Created By:		���ƽ  2014.04.23
** Remarks:             ����������㵱ǰ��������ʾ����ʼ����
*****************************************************************************/
static s32 sdkDispGetRowStrXY(u32 row_index, u32 align, const u8 *pstr, u32 *uiX, u32 *uiY) {
    s32 row_height = 0;
    s32 word_max_height = 0;
    s32 assii_max_height = 0;
    s32 row_max_width = 0;
    s32 assii_max_width = 0;
    s32 word_max_width = 0;
    s32 str_width = 0;
    u32 i = 0;
    u32 len = strlen((s8 *) pstr);
    bool asciiFlag = false;
    bool nativeFlag = false;

    //Trace("devlcd", "len = %d",len );
    /*�õ���ǰ����������*/
    sdkDispGetAsciiWH(g_row_info.str_font.m_ascsize, (u32 * ) & assii_max_width,
                      (u32 * ) & assii_max_height);
    sdkDispGetNativeWH(g_row_info.str_font.m_nativesize, (u32 * ) & word_max_width,
                       (u32 * ) & word_max_height);

    row_max_width = g_row_info.show_lcd_width - g_row_info.row_x_offset_temp;

    /*�����ı����Ŀ��*/
    for (i = 0; i < len; i++) {
        if ((u8) pstr[i] >= 0x80) //��ascii
        {
            if (row_max_width >= str_width + word_max_width) {
                i++;
                str_width += word_max_width;
            }
            else {
                break;
            }
            nativeFlag = true;
        }
        else {
            if (row_max_width >= str_width + assii_max_width) {
                str_width += assii_max_width;
            }
            else {
                break;
            }
            asciiFlag = true;
        }
    }

    if (NULL != uiX) {
        if (ALIGN_CENTER == align) {
            *uiX = g_row_info.show_lcd_left + g_row_info.row_x_offset_temp +
                   ((row_max_width - str_width) >> 1); //��X��ʼλ��
        }
        else if (ALIGN_RIGHT == align) {
            *uiX = g_row_info.show_lcd_right - str_width;
        }
        else //ALIGH_LEFT
        {
            *uiX = g_row_info.show_lcd_left + g_row_info.row_x_offset_temp; //��X��ʼλ��
        }
    }

    //�����ı���ʼ������ = ��Ļ�������*2 + ����ʼλ�� - ����ƫ��
    if (NULL != uiY) {
        /*�õ��и�*/
        row_height = g_row_info.row_height;

        /*����ָ�*/
        if ((asciiFlag && nativeFlag) || (!asciiFlag && !nativeFlag)) // 11 00
        {
            word_max_height =
                    word_max_height > assii_max_height ? word_max_height : assii_max_height;
        }
        else if (!asciiFlag && nativeFlag) //01
        { ; //word_max_height = word_max_height;
        }
        else //10
        {
            word_max_height = assii_max_height;
        }
        *uiY = (row_height - word_max_height) >> 1;  //����ƫ��  (�и�-����ָ�)/2
        *uiY = g_row_info.show_lcd_top + (row_index * row_height) + *uiY; //��Y��ʼλ��
    }
    return i;
}

/*****************************************************************************
** Descriptions:	��ȡ����ָ�������ı���ʾ����ʼ����
** Parameters:          s32 row_index >= 0 < row_counts
                    u32 align ���뷽ʽ
                    u8 *pstr
                    s32 *siX
                    s32 *siY
** Returned value:	������ʾ��ʵ���ַ�����( ���ַ�̫��ʱ����ص������޷���ʾ�Ĳ��� )
** Created By:		���ƽ  2014.04.23
** Remarks:             ����������㵱ǰ��������ʾ����ʼ����
*****************************************************************************/
static s32 sdkDispGetRowStrXY_inArea(u32 row_index, u32 align, u32 startcol, u32 endcol, u8 *pstr,
                                     u32 *uiX, u32 *uiY) {
    s32 row_height;
    s32 word_max_height;
    s32 assii_max_height;
    s32 row_max_width;
    s32 assii_max_width = 0;
    s32 word_max_width = 0;
    s32 str_width = 0;
    u32 i = 0;
    u32 len = strlen((s8 *) pstr);
    bool asciiFlag = false;
    bool nativeFlag = false;


    /*�õ���ǰ����������*/
    sdkDispGetAsciiWH(g_row_info.str_font.m_ascsize, (u32 * ) & assii_max_width,
                      (u32 * ) & assii_max_height);
    sdkDispGetNativeWH(g_row_info.str_font.m_nativesize, (u32 * ) & word_max_width,
                       (u32 * ) & word_max_height);

    row_max_width = endcol - startcol;

    /*�����ı����Ŀ��*/
    for (i = 0; i < len; i++) {
        if ((u8) pstr[i] >= 0x80) //��ascii
        {
            if (row_max_width >= str_width + word_max_width) {
                i++;
                str_width += word_max_width;
            }
            else {
                break;
            }
            nativeFlag = true;
        }
        else {
            if (row_max_width >= str_width + assii_max_width) {
                str_width += assii_max_width;
            }
            else {
                break;
            }
            asciiFlag = true;
        }
    }

    if (NULL != uiX) {
        if (ALIGN_CENTER == align) {
            *uiX = g_row_info.show_lcd_left + startcol +
                   ((row_max_width - str_width) >> 1); //��X��ʼλ��
        }
        else if (ALIGN_RIGHT == align) {
            *uiX = g_row_info.show_lcd_left + endcol - str_width;
        }
        else {
            *uiX = g_row_info.show_lcd_left + startcol; //��X��ʼλ��
        }
    }

    //�����ı���ʼ������ = ��Ļ�������*2 + ����ʼλ�� - ����ƫ��
    if (NULL != uiY) {
        /*�õ��и�*/
        row_height = g_row_info.row_height;

        /*����ָ�*/
        if ((asciiFlag && nativeFlag) || (!asciiFlag && !nativeFlag)) // 11 00
        {
            word_max_height =
                    word_max_height > assii_max_height ? word_max_height : assii_max_height;
        }
        else if (!asciiFlag && nativeFlag) //01
        { ; //word_max_height = word_max_height;
        }
        else //10
        {
            word_max_height = assii_max_height;
        }
        *uiY = (row_height - word_max_height) >> 1;  //����ƫ��  (�и�-�ָ�)/2
        *uiY = g_row_info.show_lcd_top + (row_index * row_height) + *uiY; //��Y��ʼλ��
    }
    return i;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.08.10
** Remarks:
*****************************************************************************/
s32 sdkDispLcdOpen(void) {
    s32 rtn;
    u16 row_height;
    u16 row_rev;

    rtn = ddi_lcd_open();

    if (DDI_OK == rtn) {
        /*��ʼ������ϢĬ��ֵ*/
        g_row_info.str_font.m_font = E_PRN_FONT_SONGTI;
        g_row_info.str_font.m_ascsize = LCD_DEFAULT_ASCII_FONT;
        g_row_info.str_font.m_nativesize = LCD_DEFAULT_NATIVE_FONT;

        g_row_info.brush_color = LCD_BRUSH_COLOR;
        g_row_info.row_x_offset_temp = 0; //��ʱ��ƫ��

        /*��ȡLCD�ֱ���*/
        ddi_lcd_ioctl(DDI_LCD_CTL_RESOLUTION, (u32)(&g_row_info.lcd_width),
                      (u32)(&g_row_info.lcd_height));
        /*��������ֵ��������ȷ���ģ�������ʱ����*/
        g_row_info.show_lcd_left = 0;
        g_row_info.show_lcd_top = 0;
        g_row_info.show_lcd_right = g_row_info.lcd_width - 1;
        g_row_info.show_lcd_bottom = g_row_info.lcd_height - 1;
        g_row_info.show_lcd_width = g_row_info.show_lcd_right - g_row_info.show_lcd_left +
                                    1; //lcd ��  = show_lcd_right - show_lcd_left;
        g_row_info.show_lcd_height = g_row_info.show_lcd_bottom - g_row_info.show_lcd_top +
                                     1; //lcd �� = show_lcd_bottom - show_lcd_top;

        //��ʼ��Ĭ�ϵ���ģʽ
        g_row_info.row_counts = SDK_SYS_LINE5; //������
        sdkSysSetSreenMaxLine(SDK_SYS_LINE5);
        row_height = g_row_info.show_lcd_height / g_row_info.row_counts;
        row_rev = g_row_info.show_lcd_height % g_row_info.row_counts;

        if (row_rev + 1 == g_row_info.row_counts) //ʣ��ռ�պ� = ���� - 1; ���Բ��뵽ÿһ���м�϶��
        {
            row_height++;
        }
        else //��ʣ��ռ仮�ֵ���Ļ����
        {
            g_row_info.show_lcd_top += row_rev >> 1;
        }
        g_row_info.row_height = row_height; //�и�
        g_row_info.row_width = g_row_info.show_lcd_width; //�п�

        g_row_info.lcd_brightness = 7; //Ĭ��Ϊ7
        g_row_info.is_show_stat_bar = false;
    }
    return rtn;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.08.10
** Remarks:
*****************************************************************************/
void sdkDispLcdClose(void) {
    memset(&g_row_info, 0, sizeof(SDK_LCD_ROW_INFO));
    ddi_lcd_close();
}

/*****************************************************************************
** Descriptions:    ��/�ر� ����ƿ��ƹ���(ֻ�кڰ������б���ƴ�ر� ʹ��)
** Parameters:          bool IsEnableCtr
** Returned value:
** Created By:		fusuipu  2013.08.12
** Remarks:
*****************************************************************************/
s32 sdk_dev_misc_ex_enable_lcdlamp(bool IsEnableCtr) {
    if (DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_BKLIGHT, (u32)(IsEnableCtr), 0)) {
        return SDK_ERR;
    }
#if LCD_PIXEL_BITS == 1 //�ڰ���
#else

    if( IsEnableCtr == true )
    {
        sdk_dev_key_set_lcd_open_flag(IsEnableCtr); //���ð����Ƿ�ɻָ���ɫ��Ļ����
    }
#endif /* if 0 */
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    ��/�رձ����(ֻ�кڰ������б���ƴ�ر�)
** Parameters:          bool IsEnableCtr
** Returned value:
** Created By:		fusuipu  2013.08.12
** Remarks:
*****************************************************************************/
s32 sdk_dev_misc_ex_control_lcdlamp(bool IsOpen) {
#if LCD_PIXEL_BITS == 1 //�ڰ���

    if (DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_BKLIGHT_CTRL, (u32) IsOpen, 0)) {
        return SDK_ERR;
    }
#else //����
    Trace("devlcd", "IsOpen = %d\r\n", IsOpen);

    if( IsOpen )
    {
        //������Ļ���ָ�ԭ�е�����
        if( DDI_OK !=  ddi_lcd_ioctl(DDI_LCD_CTL_BRIGHT, (u32)g_row_info.lcd_brightness, 0)
            || DDI_OK !=  ddi_lcd_ioctl(DDI_LCD_CTL_BKLIGHT_CTRL, (u32)IsOpen, 0) )
        {
            return SDK_ERR;
        }
    }
    else
    {
        //��������Ϊ1
        if( DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_BRIGHT, (u32)1, 0) )
        {
            Trace("devlcd", "DDI_LCD_CTL_BRIGHT fail\r\n");
            return SDK_ERR;
        }
        Trace("devlcd", "DDI_LCD_CTL_BRIGHT success\r\n");
    }
#endif /* if 0 */
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	���ñ������ʱʱ��
** Parameters:          siDlyTime
** Returned value:
** Created By:		fusuipu  2013.08.12
** Remarks:
*****************************************************************************/
s32 sdkDispExSetLcdlampDelayTime(s32 siDlyTime) {
    u8 uiTime;
    s32 rtn;

    if (siDlyTime > 0 && siDlyTime < 500) {
        uiTime = 1;
    }
    else {
        uiTime = (siDlyTime + 500) / 1000;
    }
    rtn = ddi_lcd_ioctl(DDI_LCD_CTL_BKLIGHTTIME, (u32) uiTime, 0);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    return SDK_ERR;
}

/****************************************************************************
  **Description:     ��ָ��λ����ʾָ����С��ͼƬ
  **Input parameters:  dispRect,ͼƬҪ��ʾ������
                     bmpWidth,bmpheight,ͼƬ�����
                     pBmpDots ͼƬ����ָ�룬��ɫ����Һ����ģ��
                                ����ȡģ���ֽڵ���
  **Output parameters:
**
********************************Returned value: -1 ʧ��   0 �ɹ�

****************************************************************************/


/*****************************************************************************
** Descriptions:	�����β���ָ������ɫ�������ڲ�����
** Parameters:          s32 siX0
                    s32 siY0
                    s32 siX1
                    s32 siY1
                    s32 siIsShowBorder: �Ƿ���ʾ�߽�
                    s32 siIsFullColor : �Ƿ���siColor�����ɫ
                    s32 siColor :�����ɫֵ
** Returned value:
** Created By:		���ƽ  2014.06.30
** Remarks:
*****************************************************************************/
s32 sdkDispRect(s32 siX0, s32 siY0, s32 siX1, s32 siY1, s32 siIsShowBorder, s32 siIsFullColor,
                s32 siColor) {
    strRect sRect;

#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:2 */
    sRect.m_x0 = siX0 + g_row_info.show_lcd_left;

    if( sRect.m_x0 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    sRect.m_y0 = siY0 + g_row_info.show_lcd_top;

    if( sRect.m_y0 > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
    sRect.m_x1 = siX1 + g_row_info.show_lcd_left;

    if( sRect.m_x1 > g_row_info.show_lcd_right )
    {
        sRect.m_x1 = g_row_info.show_lcd_right;
    }
    sRect.m_y1 = siY1 + g_row_info.show_lcd_top;

    if( sRect.m_y1 > g_row_info.show_lcd_bottom )
    {
        sRect.m_y1 = g_row_info.show_lcd_bottom;
    }
#else
    sRect.m_x0 = siX0;

    if (sRect.m_x0 > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    sRect.m_y0 = siY0;

    if (sRect.m_y0 > g_row_info.show_lcd_bottom) {
        return SDK_PARA_ERR;
    }
    sRect.m_x1 = siX1;

    if (sRect.m_x1 > g_row_info.show_lcd_right) {
        sRect.m_x1 = g_row_info.show_lcd_right;
    }
    sRect.m_y1 = siY1;

    if (sRect.m_y1 > g_row_info.show_lcd_bottom) {
        sRect.m_y1 = g_row_info.show_lcd_bottom;
    }
#endif /* if 0 */

    if (siIsFullColor) {
        if (DDI_OK != ddi_lcd_fill_rect(&sRect, siColor)) {
            return SDK_ERR;
        }
    }

    if (siIsShowBorder) {
        if (DDI_OK != ddi_lcd_show_rect(&sRect)) {
            return SDK_ERR;
        }
    }
    return SDK_OK;
}

static s32 sdkDispDrawIcon(s32 siX, s32 siY, MC_BITMAP *dotBuf) {
    s32 v, h;

    //u32 brush_color;

    if (dotBuf == NULL || dotBuf->bmBits == NULL) {
        return SDK_PARA_ERR;
    }

    if (LCD_BRUSH_COLOR != g_row_info.brush_color) {
        if (DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_SET_COLOR, LCD_BRUSH_COLOR, 0)) {
            Assert(0);
            return SDK_ERR;
        }
    }

    if (dotBuf->bmScan) //�����������ɨ��( ͼƬ���½�Ϊ��ʼλ�� )
    {
        for (v = (s32) dotBuf->bmHeight - 1; v >= 0; v--) //��
        {
            for (h = 0; h < (s32) dotBuf->bmWidth; h++) //��
            {
                //Trace( "devlcd", "((%d)>>3) + %d*((dotBuf->bmWidth+7)>>3) = %d\r\n", h,v,((h)>>3) + v*((dotBuf->bmWidth+7)>>3) );
                if ((dotBuf->bmBits[((h) >> 3) + v * ((dotBuf->bmWidth + 7) >> 3)] &
                     (0x01 << (h % 8))) != 0) {
                    //printf("��");
                    //���
                    if ((siX + h) < g_row_info.lcd_width && (siY + v) < g_row_info.lcd_height) {
                        ddi_lcd_show_pixel(siX + h, siY + dotBuf->bmHeight - 1 - v);
                    }
                }
                else {
                    //printf("��");
                }
            }

            //printf("\r\n");
        }
    }
    else //�����������ɨ��( ͼƬ���Ͻ�Ϊ��ʼλ�� )
    {
        for (v = 0; v < (s32) dotBuf->bmHeight; v++) //��
        {
            for (h = 0; h < (s32) dotBuf->bmWidth; h++) //��
            {
                //Trace( "devlcd", "((%d)>>3) + %d*((dotBuf->bmWidth+7)>>3) = %d\r\n", h,v,((h)>>3) + v*((dotBuf->bmWidth+7)>>3) );
                if ((dotBuf->bmBits[((h) >> 3) + v * ((dotBuf->bmWidth + 7) >> 3)] &
                     (0x01 << (h % 8))) != 0) {
                    //printf("��");
                    //���
                    if ((siX + h) < g_row_info.lcd_width && (siY + v) < g_row_info.lcd_height) {
                        ddi_lcd_show_pixel(siX + h, siY + dotBuf->bmHeight - 1 - v);
                    }
                }
                else {
                    //printf("��");
                }
            }

            //printf("\r\n");
        }
    }

    if (LCD_BRUSH_COLOR != g_row_info.brush_color) {
        if (DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_SET_COLOR, g_row_info.brush_color, 0)) {
            Assert(0);
            return SDK_ERR;
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����Ļ��color--8����
** Parameters:          s32 siX:x����
                               s32 siY:y����
                               u8 ucColor:8���������
** Returned value:
** Created By:		fusuipu  2013.08.12
** Remarks:             �ڰ�����ͼ��ר�ã��Ƚ�ͼ��ת��λͼ���飬�ٵ��˺�����ʾ�������ڻ�һ����ĺ���sdkDispPutPixel��
*****************************************************************************/
s32 sdk_dev_lcd_draw_dot(s32 siX, s32 siY, u32 ucColor) //δ�����
{
#if 0 /*Modify by ���ƽ at 2014.05.08  11:33 */
    strRect strRect;

    strRect.m_x0 = siX;
    strRect.m_y0 = siY;
    strRect.m_x1 = siX;
    strRect.m_y1 = siY;
    ddi_lcd_fill_rect (&strRect, ucColor);
#endif /* if 0 */
#ifdef XGD_SDK_DEBUG
    sdk_warning_fun_not_work(__FUNCTION__);
#endif
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	ָ��������
** Parameters:          u32 x0
                    u32 y0
                    u32 x1
                    u32 y1
** Returned value:
** Created By:		shiweisong  2013.08.10
** Remarks:
*****************************************************************************/
static void sdkDispNoFiDspAt(u32 x0, u32 y0, u32 x1, u32 y1) //��֪���ú����ľ������ã���ʱ��ʵ��
{
    strRect temp_rect;

    if (x0 > x1 || y0 > y1) {
        return;
    }
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:19 */
    temp_rect.m_x0 = x0 + g_row_info.show_lcd_left;

    if( temp_rect.m_x0 > g_row_info.show_lcd_right )
    {
        return;
    }
    temp_rect.m_y0 = y0 + g_row_info.show_lcd_top;

    if( temp_rect.m_y0 > g_row_info.show_lcd_bottom)
    {
        return;
    }
    temp_rect.m_x1 = x1 + g_row_info.show_lcd_left;

    if( temp_rect.m_x1 > g_row_info.show_lcd_right )
    {
        temp_rect.m_x1 = g_row_info.show_lcd_right;
    }
    temp_rect.m_y1 = y1 + g_row_info.show_lcd_top;

    if( temp_rect.m_y1 > g_row_info.show_lcd_bottom )
    {
        temp_rect.m_y1 = g_row_info.show_lcd_bottom;
    }
#else
    temp_rect.m_x0 = x0;

    if (temp_rect.m_x0 > g_row_info.show_lcd_right) {
        return;
    }
    temp_rect.m_y0 = y0;

    if (temp_rect.m_y0 > g_row_info.show_lcd_bottom) {
        return;
    }
    temp_rect.m_x1 = x1;

    if (temp_rect.m_x1 > g_row_info.show_lcd_right) {
        temp_rect.m_x1 = g_row_info.show_lcd_right;
    }
    temp_rect.m_y1 = y1;

    if (temp_rect.m_y1 > g_row_info.show_lcd_bottom) {
        temp_rect.m_y1 = g_row_info.show_lcd_bottom;
    }
#endif /* if 0 */
    ddi_lcd_ioctl(DDI_LCD_CTL_INVERT_RECT, (u32)(&temp_rect), 0);
}

/*****************************************************************************
** Descriptions:	�����Աȶ�
** Parameters:          s32 siLightLev
** Returned value:
** Created By:		fusuipu  2013.05.10
** Remarks:         sdk�Ѿ��������޶���20��60֮���ˣ�����ת��
*****************************************************************************/
s32 sdkSysSetConTrast(s32 siLightLev) {
    s32 nlightLev = (siLightLev - 20) * 10; //�Ŵ�ʮ��
    const s32 nLevelLev = ((60 - 20) * 10) / 12; //�ܹ�16���ȼ�,ֻȥ����12���ȼ����ó�ÿ���ȼ��ķ�Χֵ
    u32 nLevel = (u32)(nlightLev / nLevelLev);
    s32 rtn;

    nLevel = (nLevel > 11 ? 11 : nLevel) + 4;

    //Trace( "zjp", "nLevel = %d\r\n", nLevel );
    if (g_row_info.lcd_brightness == nLevel) {
        return SDK_OK;
    }
    rtn = ddi_lcd_ioctl(DDI_LCD_CTL_BRIGHT, nLevel, 0);

    if (DDI_OK == rtn) {
        g_row_info.lcd_brightness = nLevel;
        return SDK_OK;
    }
    else if (DDI_EINVAL == rtn) {
        return SDK_PARA_ERR;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:    sdk����ת��Ϊ�ײ�����
** Parameters:          SDK_ASC_FONT eAscFont;//asc����ģʽ
                    u32 *m_font:���壬���� ASCII �ַ�
                    u32 *m_ascsize:ASCII �ַ��ֺ�
** Returned value:
** Created By:		���ƽ  2014.10.28
** Remarks:
*****************************************************************************/
static s32 sdkDispConvertAsciiFont(SDK_ASC_FONT eAscFont, u32 *m_font, u32 *m_ascsize) {
    s32 rtn = SDK_OK;

    //�ж�ASC����,������
    if (NULL != m_ascsize && NULL != m_font) {
        switch (eAscFont) {
            //����
#if MACHINEMODEL != SDK_SYS_MACHINE_200P
            case SDK_PRN_ASCII6X8:
              *m_ascsize = E_WORD_ASCII_ERR;
              *m_font = E_PRN_FONT_SONGTI;
              break;

#endif
            case SDK_PRN_ASCII6X12:
                *m_ascsize = E_WORD_ASCII_6_12;
                *m_font = E_PRN_FONT_SONGTI;
                break;

            case SDK_PRN_ASCII8X16:
                *m_ascsize = E_WORD_ASCII_8_16;
                *m_font = E_PRN_FONT_SONGTI;
                break;

            case SDK_PRN_ASCII12X24:
                *m_ascsize = E_WORD_ASCII_12_24;
                *m_font = E_PRN_FONT_SONGTI;
                break;

            case SDK_PRN_ASCII16X24:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_SONGTI;
                break;

            case SDK_PRN_ASCII16X32:
                *m_ascsize = E_WORD_ASCII_16_32;
                *m_font = E_PRN_FONT_SONGTI;
                break;

            case SDK_PRN_ASCII16X48:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_SONGTI;
                break;
//����Ӻ�

            case SDK_PRN_ASCII8X16B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

            case SDK_PRN_ASCII12X24B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

            case SDK_PRN_ASCII16X24B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

            case SDK_PRN_ASCII16X32B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

            case SDK_PRN_ASCII16X48B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

//��Բ
            case SDK_PRN_ASCII12X24Y:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUYUAN;
                break;

            case SDK_PRN_ASCII16X24Y:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUYUAN;
                break;

            case SDK_PRN_ASCII16X32Y:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUYUAN;
                break;

            case SDK_PRN_ASCII16X48Y:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUYUAN;
                break;

//��Բ�Ӻ�
            case SDK_PRN_ASCII12X24YB:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUHEI;
                break;

            case SDK_PRN_ASCII16X24YB:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUHEI;
                break;

            case SDK_PRN_ASCII16X32YB:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUHEI;
                break;

            case SDK_PRN_ASCII16X48YB:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_YOUHEI;
                break;

//����
            case SDK_PRN_ASCII20X32B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

            case SDK_PRN_ASCII28X48B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

            case SDK_PRN_ASCII24X48:
#if MACHINEMODEL != SDK_SYS_MACHINE_200P
                *m_ascsize = E_WORD_ASCII_24_48;
#else
                *m_ascsize = E_WORD_ASCII_ERR;
#endif
                *m_font = E_PRN_FONT_SONGTI;
                break;

            case SDK_PRN_ASCII16X16B:
                *m_ascsize = E_WORD_ASCII_ERR;
                *m_font = E_PRN_FONT_HEITI;
                break;

            default:
                Assert(0);
                rtn = SDK_ERR;
                break;
        }
        if (*m_ascsize == E_WORD_ASCII_ERR) {
            return SDK_ERR;
        }
    }

    return rtn;
}

/*****************************************************************************
** Descriptions:    sdk����ת��Ϊ�ײ�����
** Parameters:          SDK_CH_FONT eChFont:��������ģʽ
                    u32 m_nativesize:�����ֺ�
** Returned value:
** Created By:		���ƽ  2014.10.28
** Remarks:
*****************************************************************************/
static s32 sdkDispConvertNativeFont(SDK_CH_FONT eChFont, u32 *m_nativesize) {
    s32 rtn = SDK_OK;

    if (NULL != m_nativesize) {
        switch (eChFont) {
            case SDK_PRN_CH12X12:
                *m_nativesize = E_WORD_NATIVE_12_12;
                break;

            case SDK_PRN_CH16X16:
                *m_nativesize = E_WORD_NATIVE_16_16;
                break;

            case SDK_PRN_CH20X20:
                *m_nativesize = E_WORD_NATIVE_20_20;
                break;

            case SDK_PRN_CH24X24:
                *m_nativesize = E_WORD_NATIVE_24_24;
                break;

            case SDK_PRN_CH32X32:
                *m_nativesize = E_WORD_NATIVE_32_32;
                break;

#if MACHINEMODEL != SDK_SYS_MACHINE_200P
                case SDK_PRN_CH48X48:
                  *m_nativesize = E_WORD_NATIVE_48_48;
                  break;

#endif
            default:
                Assert(0);
                rtn = SDK_ERR;
                break;
        }
    }
    return rtn;
}

/*****************************************************************************
** Descriptions:	����ģʽ����ʾ����
** Parameters:          U32 rowid
                               U32 startcol
                               U32 endcol
                               U8 *str
                               U8 atr
                               U8 flag
** Returned value:
** Created By:		fusuipu  2014.01.09
** Remarks:
*****************************************************************************/
s32 sdk_dev_lcd_fill_row_ram_ext(u32 rowid, u32 startcol, u32 endcol, u8 *str, u8 atr, u8 flag) {
    u32 x;
    u32 y = 0;
    s32 len = 0;
    u8 tmpStr[128] = {0};

    if (startcol > (u32)(g_row_info.show_lcd_right - g_row_info.show_lcd_left)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (endcol > (u32)(g_row_info.show_lcd_right - g_row_info.show_lcd_left)) {
        endcol = g_row_info.show_lcd_right - g_row_info.show_lcd_left;
    }

    if (startcol > endcol) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by ���ƽ at 2014.06.03  18:54 */
    Trace("devlcd", "startcol = %d\r\n", startcol);
    Trace("devlcd", "endcol = %d\r\n", endcol);
    Trace("devlcd", "atr = %d\r\n", atr);
    Trace("devlcd", "flag = %d\r\n", flag);
    Trace("devlcd", "str = %s\r\n", str);
#endif /* if 0 */

    if (atr & SDK_DISP_CDISP) //����
    {
        len = sdkDispGetRowStrXY_inArea(rowid, ALIGN_CENTER,
                                        startcol, endcol, str, &x, &y);
    }
    else if (atr & SDK_DISP_RDISP) //�Ҷ���
    {
        len = sdkDispGetRowStrXY_inArea(rowid, ALIGN_RIGHT,
                                        startcol, endcol, str, &x, &y);
    }
    else { //Ĭ�������
        len = sdkDispGetRowStrXY_inArea(rowid, ALIGN_LEFT,
                                        startcol, endcol, str, &x, &y);
    }
#if 0 /*Modify by ���ƽ at 2014.04.25  16:25  ������ʱ��֪����δ���*/

    if( atr & SDK_DISP_CTRLA ) //����л���'\xa'��س�'\xd'�ַ�,CTRLAȡ����,����λΪ0ʱ��������
    {
    }

    if( atr & SDK_DISP_INCOL ) //����һ�У���֪����ô�õ�
    {
    }
#endif /* if 0 */
    memcpy(tmpStr, str, len);
    tmpStr[len] = 0;

    //Trace("devlcd", "str = %s, len = %d\r\n", str, len );
    if (SDK_DISP_NOFDISP == (atr & SDK_DISP_FDISP))  //����
    {
        u32 str_width;
        u32 bg_x;
        u32 bg_y;
        strRect m_rect;

        g_row_info.row_x_offset_temp = startcol;
        sdkDispGetRowXY(rowid, &bg_x, &bg_y);
        g_row_info.row_x_offset_temp = 0;

        if (atr & SDK_DISP_NOFDISPLINE) //����һ����
        {
            //Trace("devlcd", "����һ����\r\n" );
            m_rect.m_x0 = bg_x; //�������X����
            m_rect.m_y0 = bg_y; //�������Y����
            //m_rect.m_x1 = bg_x + (endcol - startcol);//�����յ�X����
            m_rect.m_x1 = g_row_info.show_lcd_right; //�����յ�X����
            m_rect.m_y1 = bg_y + g_row_info.row_height; //�����յ�Y����
        }
        else {
            sdkDispGetStrWH(tmpStr, &str_width, NULL);
            //Trace("devlcd", "x = %d\r\n", x );
            m_rect.m_x0 = x; //�������X����
            m_rect.m_y0 = bg_y; //�������Y����
            m_rect.m_x1 = x + str_width; //�����յ�X����
            m_rect.m_y1 = bg_y + g_row_info.row_height; //�����յ�Y����
        }
#if 0 /*Modify by ���ƽ at 2014.06.03  18:54 */
        Trace("devlcd", "m_rect.m_x0 = %d\r\n", m_rect.m_x0);
        Trace("devlcd", "m_rect.m_y0 = %d\r\n", m_rect.m_y0);
        Trace("devlcd", "m_rect.m_x1 = %d\r\n", m_rect.m_x1);
        Trace("devlcd", "m_rect.m_y1 = %d\r\n", m_rect.m_y1);
#endif /* if 0 */
        ddi_lcd_show_text(x, y, tmpStr);
        ddi_lcd_ioctl(DDI_LCD_CTL_INVERT_RECT, (u32) & m_rect, 0); //���÷���
    }
    else {
        ddi_lcd_show_text(x, y, tmpStr);
    }
    return SDK_OK;
}

//static s32 GetAscFontType(SDK_ASC_FONT eAscFont, u8 *pDevFont);
//static s32 GetChFontType(SDK_CH_FONT eChFont, u8 *pDevFont);
/****
*
*
   ��׼�溯��
*
*
****/
typedef struct Hand {
    bool siHandInputFlag;    //��д�����־
    s32 siStartX;           //���Ϻ�����
    s32 siStartY;           //����������
    s32 siEndX;             //���º�����
    s32 siEndY;             //����������
    s32 siByteLen;          //��д�����ַ����
    s32 siByteWith;         //��д�����ַ��߶�
} HandInput;
static HandInput HandInputInfo = {false, -1, -1, -1, -1, -1, -1};


/*****************************************************************************
** Descriptions:	״̬��������ʾ��ʹ�ܿ���(����Ļ���ط��������״̬���ռ�,ͼ
                    ����ʾ ������ģʽ�ص�����״̬�����¿�ʼ������ģʽ)
** Parameters:          bool statBarFlag = true;
                    _________________________             statBarFlag = false;
************|_ _ _ _ _״̬�� _ _ _ _ _|        _________________________
|       ��ģʽ��һ��      |       |  ״̬��/��ģʽ��һ���ص�|
|       ��ģʽ�ڶ���      |       |       ��ģʽ�ڶ���      |
|       ��ģʽ������      |       |       ��ģʽ������      |
|       ��ģʽ������      |       |       ��ģʽ������      |
|       ��ģʽ������      |       |       ��ģʽ������      |
                   ---------------------------       ---------------------------
                    u32 rowNmb:��ģʽ
** Returned value:
** Created By:		���ƽ  2014.06.23
** Remarks:             K200P����ʹ��(�ռ����ޣ��޷�����״̬��)
*****************************************************************************/
s32 sdkDispStateBar(bool statBarFlag) {
    if (g_row_info.is_show_stat_bar == statBarFlag) {
        return SDK_OK;
    }
    else {
        u16 row_height;
        u16 row_rev;
        u32 max_word_height = 0;
        u32 max_ascii_height = 0;

        if (g_row_info.show_lcd_height == 0) { //û�е��� openlcd ����
            Assert(0);
            return SDK_ERR;
        }
        sdkDispGetNativeWH(g_row_info.str_font.m_nativesize, NULL, &max_word_height);
        sdkDispGetAsciiWH(g_row_info.str_font.m_ascsize, NULL, &max_ascii_height);
        max_word_height = max_word_height > max_ascii_height ? max_word_height : max_ascii_height;


        if (statBarFlag) {
            if (((u32) g_row_info.row_counts) * max_word_height > (u32)(g_row_info.show_lcd_height -
                                                                        LCD_STATUS_BAR_HEIGHT)) { /*��ҪԤ��״̬���ռ�:LCD_STATUS_BAR_HEIGHT*/
                return SDK_PARA_ERR;
            }
            g_row_info.is_show_stat_bar = statBarFlag;
            g_row_info.show_lcd_left = 0;
            g_row_info.show_lcd_right = g_row_info.lcd_width - 1;
            //sdk_dev_lcd_Get_Bmp_Size( NULL, g_row_info.show_lcd_top, "mtd1/logo2.bmp" );
            g_row_info.show_lcd_top = LCD_STATUS_BAR_HEIGHT;
            g_row_info.show_lcd_bottom = g_row_info.lcd_height - 1;
            sdkDispClearStateBar();
        }
        else {
            g_row_info.is_show_stat_bar = statBarFlag;
            g_row_info.show_lcd_left = 0;
            g_row_info.show_lcd_right = g_row_info.lcd_width - 1;
            g_row_info.show_lcd_top = 0;
            g_row_info.show_lcd_bottom = g_row_info.lcd_height - 1;
        }
        g_row_info.show_lcd_width = g_row_info.show_lcd_right - g_row_info.show_lcd_left + 1;
        g_row_info.show_lcd_height = g_row_info.show_lcd_bottom - g_row_info.show_lcd_top + 1;
        //Trace("devlcd", "g_row_info.show_lcd_top :%d\r\n", g_row_info.show_lcd_top);

        row_height = g_row_info.show_lcd_height / g_row_info.row_counts;
        row_rev = g_row_info.show_lcd_height % g_row_info.row_counts;

        if (row_rev + 1 == g_row_info.row_counts) //ʣ��ռ�պ� = ���� - 1; ���Բ��뵽ÿһ���м�϶��
        {
            row_height++;
        }
        else //��ʣ��ռ仮�ֵ���Ļ����
        {
            g_row_info.show_lcd_top += row_rev >> 1;
        }
        g_row_info.row_height = row_height; //�и�
        g_row_info.row_width = g_row_info.show_lcd_width; //�п�


        return SDK_OK;
    }
}

/*****************************************************************************
** Descriptions:	�������״̬��������ʾ������
** Parameters:          void
** Returned value:
** Created By:		���ƽ  2014.06.24
** Remarks:             K200P����ʹ��(�ռ����ޣ��޷�����״̬��)
*****************************************************************************/
s32 sdkDispClearStateBar(void) {
    if (g_row_info.is_show_stat_bar) { //û�е��� openlcd ����
        strRect rect;

        rect.m_x0 = 0;
        rect.m_y0 = 0;
        rect.m_x1 = g_row_info.show_lcd_right;
        rect.m_y1 = g_row_info.show_lcd_top - 1;

        ddi_lcd_clear_rect(&rect);
        return SDK_OK;
    }
    return SDK_ERR;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:	sdkDispGetScreenPixel
   ��������:	�����Ļ���ش�С
   �������:��
   �������:��
   ��   ��  ֵ:���ش�С
   �޸ı�ע:
   ����ʱ��:2012.05.28 18:48:01
*******************************************************************/
SDK_DISP_PIXEL sdkDispGetScreenPixel(void) {
    SDK_DISP_PIXEL pixel;

    pixel.siX = g_row_info.lcd_width;
    pixel.siY = g_row_info.lcd_height;

    return pixel;
}

#if 0 /*Modify by ���ƽ at 2015.01.27  8:55 */

/*****************************************************************************
** Descriptions:	�����û��������趨ת����ʾ�ַ���
** Parameters:          u8 *lpOut
                               const u8 *lpIn
** Returned value:
** Created By:		fusuipu  2013.05.24
** Remarks:
*****************************************************************************/
static s32 sdkDispLanguChange(u8 *lpOut, const u8 *lpIn)
{
    u8 *buf;

    if(NULL == lpOut || NULL == lpIn)
    {
        Assert(0);
        return SDK_ERR;
    }
    buf = (u8 *)sdkGetMem(1024);

    if( NULL == buf )
    {
        Assert(0);
        return SDK_ERR;
    }
    memset(buf, 0, 1024);

    if(Private_sdkSysLangChange(buf, lpIn) > 0) //���ת����ĳ��ȴ���==0 ���Ƿ�����ȷ��
    {
        // Trace("dev","change OK\r\n");
        strcpy(lpOut, buf);
    }
    else
    {
        //  Trace("warming","change err\r\n");
        strcpy(lpOut, lpIn);
    }

    if( NULL != buf )
    {
        sdkFreeMem(buf);
    }
    return strlen(lpOut);
}

#endif /* if 0 */

s32 sdkSysGetColorValue(SDK_COLOR_ID eColorId);

/*******************************************************************
   ��    ��: ���ߵ�
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ��չ��ʾһ��
   ��ڲ���:
   �� �� ֵ:
   ��    ע:   ���ҿ���ʱ��������ҿ���Ϊ��Ӱ����ʾ��
 ********************************************************************/
static void ExtendLcdFillRowRam(u32 row, u32 col, u8 *str, u32 atr) {
    u8 dispatr = 0;
    u8 displen; //i, count, , Hzlen = 0//shijianglong 2013.04.17 14:39

    displen = (u8) strlen(str) * 6;

    if (atr & SDK_DISP_OVERLINE)       //�ϻ���
    {
        atr ^= SDK_DISP_OVERLINE;

        if (atr & SDK_DISP_LDISP) {
            Verify(sdkDispDrawLine(col > 2 ? (col - 2) : col, (row) * 13 - 1, displen + col,
                                   (row) * 13 - 1, SDK_DISP_BLACK_COLOR) >= 0);
        }
        else if (atr & SDK_DISP_RDISP) {
            Verify(sdkDispDrawLine((128 - displen) - 2, (row) * 13 - 1, 126, (row) * 13 - 1,
                                   SDK_DISP_BLACK_COLOR) >= 0);
        }
        else if (atr & SDK_DISP_CDISP) {
            Verify(sdkDispDrawLine((128 - (col + displen)) / 2 + col - 2, (row) * 13 - 1,
                                   (128 + col + displen) / 2, (row) * 13 - 1,
                                   SDK_DISP_BLACK_COLOR) >= 0);
        }
    }

    if (atr & SDK_DISP_DOWNLINE)   //�»���
    {
        atr ^= SDK_DISP_DOWNLINE;

        if (atr & SDK_DISP_LDISP) {
            Verify(sdkDispDrawLine(col > 2 ? (col - 2) : col, (row) * 13 + 12, displen + col,
                                   (row) * 13 + 12, SDK_DISP_BLACK_COLOR) >= 0);
        }
        else if (atr & SDK_DISP_RDISP) {
            Verify(sdkDispDrawLine((128 - displen) - 2, (row) * 13 + 12, 127, (row) * 13 + 12,
                                   SDK_DISP_BLACK_COLOR) >= 0);
        }
        else if (atr & SDK_DISP_CDISP) {
            Verify(sdkDispDrawLine((128 - (col + displen)) / 2 + col - 2, (row) * 13 + 12,
                                   (128 + col + displen) / 2, (row) * 13 + 12,
                                   SDK_DISP_BLACK_COLOR) >= 0);
        }
    }

    if (atr & SDK_DISP_SIDELINE)   //������������
    {
        atr ^= SDK_DISP_SIDELINE;

        if (atr & SDK_DISP_LDISP) {
            Verify(sdkDispDrawLine(col > 2 ? (col - 2) : col, (row) * 13, col > 2 ? (col - 2) : col,
                                   (row) * 13 + 12, SDK_DISP_BLACK_COLOR) >=
                   0);        //���ҿ���ʱ��������ҿ�
            Verify(sdkDispDrawLine(displen + col, (row) * 13, displen + col, (row) * 13 + 12,
                                   SDK_DISP_BLACK_COLOR) >= 0);
        }
        else if (atr & SDK_DISP_RDISP) {
            Verify(sdkDispDrawLine((128 - displen) - 2, (row) * 13 - 1, 128 - displen - 2,
                                   (row) * 13 + 12, SDK_DISP_BLACK_COLOR) >= 0);
            Verify(sdkDispDrawLine(126, (row) * 13 - 1, 126, (row) * 13 + 12,
                                   SDK_DISP_BLACK_COLOR) >= 0);
        }
        else if (atr & SDK_DISP_CDISP) {
            Verify(sdkDispDrawLine((128 + col + displen) / 2, (row) * 13 - 1,
                                   (128 + col + displen) / 2, (row) * 13 + 12,
                                   SDK_DISP_BLACK_COLOR) >= 0);
            Verify(sdkDispDrawLine((128 - (col + displen)) / 2 + col - 2, (row) * 13 - 1,
                                   (128 - (col + displen)) / 2 + col - 2, (row) * 13 + 12,
                                   SDK_DISP_BLACK_COLOR) >= 0);
        }
    }
    dispatr = atr & 0x000000FF;                           //�ײ�ֻ����U8
    sdkDispRowAt((u8) row, (u8) col, str, dispatr);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispFillRowRam
   ��������:��ʾ��ָ�����к��е��ַ���������ָ�������������ʱ���������
   �������:
        u32 siRow: �к�
        {
            SDK_DISP_LINE1               //��ʾ�к�1
            SDK_DISP_LINE2                       //��ʾ�к�2
            SDK_DISP_LINE3               //��ʾ�к�3
            SDK_DISP_LINE4                       //��ʾ�к�4
            SDK_DISP_LINE5               //��ʾ�к�5

        }
        u32 siColid : �к�
        const u8 *pasStr: ��ʾ����
        u8 ucAtr:��ʾ����
        atr ��ʾ����
        {
            SDK_DISP_FDISP/SDK_DISP_NOFDISP ����/����
            SDK_DISP_LDISP|SDK_DISP_CDISP|SDK_DISP_RDISP ��ʾ����ʼλ�� ��|��|��
            SDK_DISP_INCOL ������ʾ����һ��
        }
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.22 15:23:15
*******************************************************************/
s32 sdkDispFillRowRam(s32 siRow, s32 siColid, const u8 *pasStr, u32 ucAtr) {
    //u8 *str_buf;
    SDK_DISP_PIXEL pixel;

    memset(&pixel, 0, sizeof(SDK_DISP_PIXEL));


    if ((SDK_DISP_LINE5 < siRow) ||
        (0 > siRow))                                                 //���������or �к�С��0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    pixel = sdkDispGetScreenPixel();


    if (siColid >= pixel.siX)                                                 //������ʾ�������
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (NULL == pasStr)                                                     //��ָ��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ExtendLcdFillRowRam((u32) siRow, siColid, (u8 *) pasStr, ucAtr);

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDisplayAt
   ��������:��ָ��������ʾ��ʱ��������
   �������:
        u32 siX : X����
        u32 siY: Y����
        const u8 *pasStr:��ʾ����
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.22 15:54:55
*******************************************************************/
s32 sdkDispAt(s32 siX, s32 siY, const u8 *pasStr) {
    u32 len;
    s32 rslt;
    u8 tmpStr[128] = {0};

    if (NULL == pasStr)                                                       //��ָ��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = strlen(pasStr);

    if (sdkIsVisibleStr(pasStr, len) ==
        false)                                              //��ʾ���ݴ���
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:5 */

    if( siX + g_row_info.show_lcd_left > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }

    if( siY + g_row_info.show_lcd_top > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
    g_row_info.row_x_offset_temp = (u16)siX; //����������ƫ��
    len = sdkDispGetRowStrXY(0, ALIGN_LEFT, pasStr, NULL, NULL);  //�������ʾ���ַ�����


    siX += g_row_info.show_lcd_left;
    siY += g_row_info.show_lcd_top;
    memcpy(tmpStr, pasStr, len);  //���ﱣ��һ�����ݣ����õ������
    tmpStr[len] = 0;

    rslt = ddi_lcd_show_text(siX, siY, tmpStr);
    g_row_info.row_x_offset_temp = 0;
#else

    if (siX > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }

    if (siY > g_row_info.show_lcd_bottom) {
        return SDK_PARA_ERR;
    }
    g_row_info.row_x_offset_temp = (u16) siX; //����������ƫ��
    len = sdkDispGetRowStrXY(0, ALIGN_LEFT, pasStr, NULL, NULL);  //�������ʾ���ַ�����

    memcpy(tmpStr, pasStr, len);  //���ﱣ��һ�����ݣ����õ������
    tmpStr[len] = 0;

    rslt = ddi_lcd_show_text(siX, siY, tmpStr);
    g_row_info.row_x_offset_temp = 0;

#endif /* if 0 */

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������: sdkDispClearScreen
   ��������: ����ʱ���棬�ָ���ԭ���ı���ͼƬ����ɫ��
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.05.22 15:45:03
*******************************************************************/
void sdkDispClearScreen(void) {
    strRect rect = {0};

    rect.m_x0 = g_row_info.show_lcd_left;
    rect.m_y0 = g_row_info.show_lcd_top;
    rect.m_x1 = g_row_info.show_lcd_right;
    rect.m_y1 = g_row_info.show_lcd_bottom;

    ddi_lcd_clear_rect(&rect);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispClearRow
   ��������:���ָ������ʾ
   �������:
       u32 siRow:�к�
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.22 16:36:04
*******************************************************************/
s32 sdkDispClearRow(s32 siRow) {
    strRect rect;

    if ((SDK_DISP_LINE5 < siRow) ||
        (0 > siRow))                                                     //���������or �к�С��0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispGetRowXY(siRow, &rect.m_x0, &rect.m_y0);
    rect.m_x1 = g_row_info.show_lcd_width;
    rect.m_y1 = rect.m_y0 + g_row_info.row_height - 1;

    //Trace( "zjp", "rect.m_x0 = %d\r\n", rect.m_x0 );
    //Trace( "zjp", "rect.m_y0 = %d\r\n", rect.m_y0 );
    //Trace( "zjp", "rect.m_x1 = %d\r\n", rect.m_x1 );
    //Trace( "zjp", "rect.m_y1 = %d\r\n", rect.m_y1 );
    ddi_lcd_clear_rect(&rect);

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispClearAt
   ��������:ͨ��ָ�����������ָ���������ʾ��
   �������:
        u32 siX0:�������Ͻ�X����
        u32 siY0:�������Ͻ�Y����
        u32 siX1 :�������½�X����
        u32 siY1:�������½�Y����
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:��siX0=0��siY0=0��siX1=0��siY1=0ʱ�ǲ������ʾ�ġ�
                         ��Ҫȫ�������Ļ��ʾ������128x64��������Ҫ����siX0=0��siY0=0��siX1=128��siY1=64//shijianglong 2013.04.09 17:26
   ����ʱ��:2012.05.22 16:00:33
*******************************************************************/
s32 sdkDispClearAt(s32 siX0, s32 siY0, s32 siX1, s32 siY1) {
    strRect rect;

    if ((siX0 > siX1) || (siY0 >
                          siY1))                                                                                                    //����ֵ�߼�����
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.07  17:33 */
    rect.m_x0 = siX0 + g_row_info.show_lcd_left;

    if( rect.m_x0 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    rect.m_y0 = siY0 + g_row_info.show_lcd_top;

    if( rect.m_y0 > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
    rect.m_x1 = siX1 + g_row_info.show_lcd_left;

    if( rect.m_x1 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    rect.m_y1 =  siY1 + g_row_info.show_lcd_top;

    if( rect.m_y1 > g_row_info.show_lcd_bottom )
    {
        return SDK_PARA_ERR;
    }
#else
    rect.m_x0 = siX0;

    if (rect.m_x0 > g_row_info.show_lcd_width) {
        return SDK_PARA_ERR;
    }
    rect.m_y0 = siY0;

    if (rect.m_y0 > g_row_info.show_lcd_height) {
        return SDK_PARA_ERR;
    }
    rect.m_x1 = siX1;

    if (rect.m_x1 > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    rect.m_y1 = siY1;

    if (rect.m_y1 > g_row_info.show_lcd_bottom) {
        return SDK_PARA_ERR;
    }
#endif /* if 0 */

    ddi_lcd_clear_rect(&rect);

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�辢��
   ��������:sdkDispBrushScreen
   ��������:ˢ����Ļ����ʾ��ʱ�����������
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.05.22 15:33:32
*******************************************************************/
void sdkDispBrushScreen(void) {
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispSetBackground
   ��������:���ñ���ͼƬ
   �������:
        u32 siX0: ��ʼ��X����
        u32 siY0:��ʼ��Y����
        u32 siLen:��ʾ���
        u32 siYlen:��ʾ�߶�
        const u8 *pasBmpFileName:ͼƬ����
   �������:��
   ��   ��  ֵ: SDK_ERR:��������SDK_OK:�������óɹ���
   �޸ı�ע:
   ����ʱ��:2012.05.22 16:06:33
*******************************************************************/
s32 sdkDispSetBackground(s32 siX0, s32 siY0, s32 siLen, s32 siYlen, const u8 *pasBmpFileName) {
    s32 siX1, siY1;

    if (NULL == pasBmpFileName ||
        (sdkAccessFile(pasBmpFileName) == 0))             //�ļ�������//shijianglong 2013.01.23 17:59
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    siX1 = siX0 + siLen;
    siY1 = siY0 + siYlen;

    if (siX0 > siX1 || siY0 > siY1) {
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by ���ƽ at 2014.06.25  17:30 */
    strRect mRect;

#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:21 */
    mRect.m_x0 = siX0 + g_row_info.show_lcd_left;

    if( mRect.m_x0 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    mRect.m_y0 = siY0 + g_row_info.show_lcd_top;

    if( mRect.m_y0 > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
    mRect.m_x1 = siX1 + g_row_info.show_lcd_left;

    if( mRect.m_x1 > g_row_info.show_lcd_right )
    {
        mRect.m_x1 = g_row_info.show_lcd_right;
    }
    mRect.m_y1 = siY1 + g_row_info.show_lcd_top;

    if( mRect.m_y1 > g_row_info.show_lcd_bottom )
    {
        mRect.m_y1 = g_row_info.show_lcd_bottom;
    }
#else
    mRect.m_x0 = siX0;

    if( mRect.m_x0 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    mRect.m_y0 = siY0;

    if( mRect.m_y0 > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
    mRect.m_x1 = siX1;

    if( mRect.m_x1 > g_row_info.show_lcd_right )
    {
        mRect.m_x1 = g_row_info.show_lcd_right;
    }
    mRect.m_y1 = siY1;

    if( mRect.m_y1 > g_row_info.show_lcd_bottom )
    {
        mRect.m_y1 = g_row_info.show_lcd_bottom;
    }
#endif /* if 0 */

    ddi_lcd_show_picture_file(&mRect, pasBmpFileName);
#endif /* if 0 */
#ifdef XGD_SDK_DEBUG
    sdk_warning_fun_not_work(__FUNCTION__);
#endif
    return SDK_OK;
}

/*******************************************************************
   ��    Ȩ:    �¹���
   ��    ��:    ���ƽ
   ��������:    sdkDispShowBmpdots
   ��������:	��ʾͼƬ(��������)
   �������:
                dispRect,ͼƬҪ��ʾ������
                bmpWidth,bmpheight,ͼƬ�����
                pBmpDots ͼƬ����ָ�룬��ɫ����Һ����ģ��
                        ����ȡģ���ֽڵ���
   �������:    ��
   �� �� ֵ:    SDK_ERR:��������SDK_OK:�������óɹ���
   �޸ı�ע:
   ����ʱ��:    012.11.02 10:43:59
*******************************************************************/
s32 sdkDispShowBmpdots(const SDK_DISP_SRECT dispRect, u32 bmpWidth, u32 bmpheight,
                       const u8 *pBmpDots) {
    s32 rtn;
    strRect rect;
    strPicture strPic;

    if (pBmpDots == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  14:48 */

    if( dispRect.x0 + g_row_info.show_lcd_left > g_row_info.show_lcd_right ||
        dispRect.y0 + g_row_info.show_lcd_top > g_row_info.show_lcd_bottom ||
        dispRect.x1 + g_row_info.show_lcd_left > g_row_info.show_lcd_right ||
        dispRect.y1 + g_row_info.show_lcd_top > g_row_info.show_lcd_bottom ||
        dispRect.x0 > dispRect.x1 || dispRect.y0 > dispRect.y1 )
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rect.m_x0 = dispRect.x0 + g_row_info.show_lcd_left;
    rect.m_y0 = dispRect.y0 + g_row_info.show_lcd_top;
    rect.m_x1 = dispRect.x1 + g_row_info.show_lcd_left;
    rect.m_y1 = dispRect.y1 + g_row_info.show_lcd_top;
#else

    if (dispRect.x0 > g_row_info.show_lcd_right || dispRect.y0 > g_row_info.show_lcd_bottom ||
        dispRect.x1 > g_row_info.show_lcd_right || dispRect.y1 > g_row_info.show_lcd_bottom ||
        dispRect.x0 > dispRect.x1 || dispRect.y0 > dispRect.y1) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rect.m_x0 = dispRect.x0;
    rect.m_x1 = dispRect.x1;
    rect.m_y0 = dispRect.y0;
    rect.m_y1 = dispRect.y1;
#endif /* if 0 */


    strPic.m_height = bmpheight;
    strPic.m_width = bmpWidth;
    strPic.m_pic = (u32 *) pBmpDots;
    rtn = ddi_lcd_show_picture(&rect, &strPic);
    rtn = DDI_OK;

    if (rtn == DDI_EINVAL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    else if (rtn == DDI_EIO) {
        Assert(0);
        return SDK_ERR;
    }
    else {
        return SDK_OK;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispShowBmp
   ��������:	��ʾͼƬ
   �������:
        u32 siX0: ��ʼ��X����
        u32 siY0:��ʼ��Y����
        u32 siLen:��ʾ���
        u32 siYlen:��ʾ�߶�
        const u8 *pasBmpFileName:ͼƬ����
   �������:��
   ��   ��  ֵ:SDK_ERR:��������SDK_OK:�������óɹ���
   �޸ı�ע:
   ����ʱ��:2012.11.02 10:43:59
*******************************************************************/
s32 sdkDispShowBmp(s32 siX0, s32 siY0, s32 siXLen, s32 siYlen, const u8 *pasBmpFileName) {
    s32 rtn;
    strRect rect;
    s32 siX1, siY1;

    siX1 = siX0 + siXLen;
    siY1 = siY0 + siYlen;

    if (siX0 > siX1 || siY0 > siY1) {
        return SDK_PARA_ERR;
    }

    if (pasBmpFileName == NULL) {
        return SDK_PARA_ERR;
    }
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  14:47 */
    rect.m_x0 = siX0 + g_row_info.show_lcd_left;

    if( rect.m_x0 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    rect.m_y0 = siY0 + g_row_info.show_lcd_top;

    if( rect.m_y0 > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
    rect.m_x1 = rect.m_x0 + siXLen;

    if( rect.m_x1 > g_row_info.show_lcd_right )
    {
        rect.m_x1 = g_row_info.show_lcd_right;
    }
    rect.m_y1 = rect.m_y0 + siYlen;

    if( rect.m_y1 > g_row_info.show_lcd_bottom )
    {
        rect.m_y1 = g_row_info.show_lcd_bottom;
    }
#else
    rect.m_x0 = siX0;

    if (rect.m_x0 > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    rect.m_y0 = siY0;

    if (rect.m_y0 > g_row_info.show_lcd_bottom) {
        return SDK_PARA_ERR;
    }
    rect.m_x1 = rect.m_x0 + siXLen;

    if (rect.m_x1 > g_row_info.show_lcd_right) {
        rect.m_x1 = g_row_info.show_lcd_right;
    }
    rect.m_y1 = rect.m_y0 + siYlen;

    if (rect.m_y1 > g_row_info.show_lcd_bottom) {
        rect.m_y1 = g_row_info.show_lcd_bottom;
    }
#endif /* if 0 */

    rtn = ddi_lcd_show_picture_file(&rect, pasBmpFileName);

    if (rtn == DDI_EINVAL) {
        return SDK_PARA_ERR;
    }
    else if (rtn == DDI_EIO) {
        return SDK_ERR;
    }
    else {
        return SDK_OK;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispDrawProgbar
   ��������:����ʾ����?л�һ������������ˢ�?
   �������: u32 siRow:                   �к�
                        u32 siX1:                       ���������
                        u32 siX2:                       �������յ�
                        u8 ucValue:                    ������������ȵİٷ���(0~100)
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:����������յ���ͬ���ز�������������յ㲻�׼����̣�����0��1����������10���ϡ�
   ����ʱ��:2012.08.01 11:24:36
*******************************************************************/
s32 sdkDispDrawProgbar(s32 siRow, s32 siX1, s32 siX2, u8 ucValue) {
    u32 y = 0;
    u32 row_height = g_row_info.row_height;
    strRect str_rect;
    strRect str_rect2;
    u32 line_width = 2; //ʹ��2���߿�

    if ((SDK_DISP_LINE5 < siRow) ||
        (0 > siRow))                                                  //���������or �к�С��0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((100 < ucValue))                                 //���������ֵ�߼�����
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    siX1 += g_row_info.show_lcd_left;
    siX2 += g_row_info.show_lcd_left;

    if (siX1 > g_row_info.show_lcd_right || siX2 > g_row_info.show_lcd_right || siX1 >= siX2) {
        return SDK_PARA_ERR;
    }

    if ((s32)(siX2 - (line_width >> 1)) - (s32)(siX1 + (line_width >> 1)) <=
        0) //������뱣֤�о������������СΪ1������
    {
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by ���ƽ at 2014.06.03  15:54 */

    if( row_height < 2 * 2 + line_width * 2 + 10 ) //������Ĭ��Ϊ��������ƫ��2�����أ� �߿��������أ�������>=10������
    { //�и�����18������
        return;
    }
#endif /* if 0 */

    ddi_lcd_ioctl(DDI_LCD_CTL_SET_LINEWIDTH, line_width, 0); //���������߿�

    sdkDispGetRowXY(siRow, NULL, &y);

    line_width = (line_width >> 1);
    str_rect.m_x0 = siX1;
    str_rect.m_y0 = y + 2; //���� ����ƫ��2������:�������߶� = �и� - ����ƫ��2������ - ����ƫ��2������
    str_rect.m_x1 = siX2;
    str_rect.m_y1 = y + row_height -
                    2; //���� ����ƫ��2������:�������߶� = �и� - ����ƫ��2������ - ����ƫ��2������
    //Trace( "devlcd", "str_rect.m_x0 = %d\r\n", str_rect.m_x0 );
    //Trace( "devlcd", "str_rect.m_y0 = %d\r\n", str_rect.m_y0 );
    //Trace( "devlcd", "str_rect.m_x1 = %d\r\n", str_rect.m_x1 );
    //Trace( "devlcd", "str_rect.m_y1 = %d\r\n", str_rect.m_y1 );

    str_rect2.m_x0 = siX1;
    str_rect2.m_y0 = str_rect.m_y0 + line_width;
    str_rect2.m_x1 = siX1 +
                     ((s32)((s32)(siX2 - (line_width >> 1)) - (s32)(siX1 + (line_width >> 1)))) *
                     ucValue / 100;
    str_rect2.m_y1 = str_rect.m_y1 - line_width;
    //Trace( "devlcd", "str_rect2.m_x0 = %d\r\n", str_rect2.m_x0 );
    //Trace( "devlcd", "str_rect2.m_y0 = %d\r\n", str_rect2.m_y0 );
    //Trace( "devlcd", "str_rect2.m_x1 = %d\r\n", str_rect2.m_x1 );
    //Trace( "devlcd", "str_rect2.m_y1 = %d\r\n", str_rect2.m_y1 );
    ddi_lcd_fill_rect(&str_rect2, LCD_PROG_BAR_COLOR);
    ddi_lcd_show_rect(&str_rect);
    //Trace( "devlcd", "����1���߿� kaishi\r\n" );
    ddi_lcd_ioctl(DDI_LCD_CTL_SET_LINEWIDTH, 1, 0); //����1���߿�
    //Trace( "devlcd", "����1���߿�jiesu\r\n" );

    return SDK_OK;
}

/*****************************�����ӿں�������͸��****************************/


/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������: sdkDisplayRow
   ��������: �Ȳ�������ԭ�����ݣ���LCD��ĳһ��,ĳһ��(col����Ϊ��λ)��ʾ����
   �������:
       u32 siRow: �к�
        u32 eCol:  �к�
        const u8 * pasStr: ��ʾ�ַ���
        u8 ucAtr:  ��ʾ����
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.22 16:57:58
*******************************************************************/
s32 sdkDispRow(s32 siRow, s32 eCol, const u8 *pasStr, u32 ucAtr) {
    strRect rect;

    if ((SDK_DISP_LINE5 < siRow) || (0 > siRow))                         //���������or �к�С��0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (NULL == pasStr)                                                  //��ָ��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (eCol + g_row_info.show_lcd_left > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    /*�����о�����*/
    sdkDispGetRowXY(siRow, &rect.m_x0, &rect.m_y0);
    rect.m_x1 = g_row_info.show_lcd_right;
    rect.m_y1 = rect.m_y0 + g_row_info.row_height - 1;
    ddi_lcd_clear_rect(&rect);
    /*���������*/
    return sdkDispRowAt(siRow, eCol, pasStr, ucAtr);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispClearRow
   ��������:���ָ������ʾ
   �������:
       u32 siRow:�к�
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.22 16:36:04
*******************************************************************/
s32 sdkDispClearRowRam(s32 siRow) {
    strRect rect;

    if ((SDK_DISP_LINE5 < siRow) ||
        (0 > siRow))                                                     //���������or �к�С��0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispGetRowXY(siRow, &rect.m_x0, &rect.m_y0);
    rect.m_x1 = rect.m_x0 + g_row_info.show_lcd_width;
    rect.m_y1 = rect.m_y0 + g_row_info.row_height;

    //Trace( "zjp", "rect.m_x0 = %d\r\n", rect.m_x0 );
    //Trace( "zjp", "rect.m_y0 = %d\r\n", rect.m_y0 );
    //Trace( "zjp", "rect.m_x1 = %d\r\n", rect.m_x1 );
    //Trace( "zjp", "rect.m_y1 = %d\r\n", rect.m_y1 );
    ddi_lcd_clear_rect(&rect);


    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispClearRowRamAt
   ��������:���ָ���е���ʼ�е������еĻ�������
   �������:
       u32 siRow :�к�
        u32 siStartCol:��ʼ��
        u32 siEndCol:������
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.22 16:15:19
*******************************************************************/
s32 sdkDispClearRowRamAt(s32 siRow, s32 siStartCol, s32 siEndCol) {
    if ((SDK_DISP_LINE5 <
         siRow))                                                 //���������or �к�С��0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (siStartCol >
        siEndCol)                                                              //��ʼ�д��ڽ�����
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    {
        strRect rect;

        siStartCol += g_row_info.show_lcd_left;

        if (siStartCol > g_row_info.show_lcd_right) {
            Assert(0);
            return SDK_PARA_ERR;
        }
        siEndCol += g_row_info.show_lcd_left;

        if (siEndCol > g_row_info.show_lcd_right) {
            Assert(0);
            return SDK_PARA_ERR;
        }
        /*�����о�����*/
        rect.m_x0 = siStartCol;
        sdkDispGetRowXY(siRow, NULL, &rect.m_y0);
        rect.m_x1 = siEndCol;
        rect.m_y1 = rect.m_y0 + g_row_info.row_height - 1;

        if (DDI_OK == ddi_lcd_clear_rect(&rect)) {
            return SDK_OK;
        }
        else {
            return SDK_ERR;
        }
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:sdkDispGetScreenBufferSize
   ��������:�õ�������Ļ��Ҫ�Ļ���Ĵ�С
   �������:��
   �������:��
   ��   ��  ֵ:����Ĵ�С
   �޸ı�ע:
   ����ʱ��:2012.06.06 10:13:27
*******************************************************************/
u32 sdkDispGetScreenBufferSize(void) {
    return (g_row_info.lcd_width * g_row_info.lcd_height + 7) >> 3;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispRestoreScreen
   ��������:�ָ���Ļ
   �������:
           const u16 *pusLcdmem :����
            u32  len:�������ݳ���
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:                        ��������
                         SDK_OK:                                    �������óɹ�
                         SDK_DISP_SIZEERRO:              �����С����
   �޸ı�ע:
   ����ʱ��:2012.05.28 19:18:09
*******************************************************************/
s32 sdkDispRestoreScreen(const u16 *pusLcdmem, s32 siLen) {
    strRect temp_rect;
    strPicture strPic;

    //s32 rtn;

    if (siLen < (s32) sdkDispGetScreenBufferSize()) {
        Assert(0);
        return SDK_DISP_SIZEERRO;
    }

    if (NULL == pusLcdmem) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    temp_rect.m_x0 = 0;
    temp_rect.m_y0 = 0;
    temp_rect.m_x1 = g_row_info.lcd_width;
    temp_rect.m_y1 = g_row_info.lcd_height;
    strPic.m_pic = (u32 *) pusLcdmem;
    strPic.m_height = g_row_info.lcd_height;
    strPic.m_width = g_row_info.lcd_width;

    ddi_lcd_show_picture(&temp_rect, &strPic);    //zhouzhihua 2013.06.18 16:28

    return SDK_OK;
}

/*****************************�����ӿں�������͸��****************************/






/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������: sdkDisplayRowAt
   ��������:��LCD��ĳһ��,ĳһ��(col����Ϊ��λ)��ʾ����, ����������ԭ������
   �������:
        u32 siRow:�к�
       u32 siCol :  �к�
        const u8 *pAsStr: ��ʾ�ַ���
        u8 ucAtr:  ��ʾ����
   �������: ��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.22 16:54:08
*******************************************************************/
s32 sdkDispRowAt(s32 siRow, s32 siCol, const u8 *pAsStr, u32 ucAtr) {
    u8 rslt;
    u32 x;
    u32 y = 0;
    s32 len;
    u8 tmpStr[128] = {0};

    if ((SDK_DISP_LINE5 < siRow) || (0 > siRow))                         //���������or �к�С��0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (NULL == pAsStr)                                                  //��ָ��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = strlen(pAsStr);
    rslt = sdkIsVisibleStr(pAsStr, len);

    if (rslt == false)                                                  //��ʾ���ݴ���
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    //Trace("devlcd", "str = %s\r\n", str );
    //TraceHex("devlcd", "str Hex = ", str, 20 );
    if (siCol + g_row_info.show_lcd_left > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    g_row_info.row_x_offset_temp = (u16) siCol;

    if (ucAtr & SDK_DISP_CDISP) //����
    {
        len = sdkDispGetRowStrXY(siRow, ALIGN_CENTER, pAsStr, &x, &y);
    }
    else if (ucAtr & SDK_DISP_RDISP) //�Ҷ���
    {
        len = sdkDispGetRowStrXY(siRow, ALIGN_RIGHT, pAsStr, &x, &y);
    }
    else { //Ĭ�������
        len = sdkDispGetRowStrXY(siRow, ALIGN_LEFT, pAsStr, &x, &y);
    }
#if 0 /*Modify by ���ƽ at 2014.04.25  16:25  ������ʱ��֪����δ���*/

    if( atr & SDK_DISP_CTRLA ) //����л���'\xa'��س�'\xd'�ַ�,CTRLAȡ����,����λΪ0ʱ��������
    {
    }

    if( atr & SDK_DISP_INCOL ) //����һ�У���֪����ô�õ�
    {
    }
#endif /* if 0 */
    memcpy(tmpStr, pAsStr, len);
    tmpStr[len] = 0;

    if (SDK_DISP_NOFDISP == (ucAtr & SDK_DISP_FDISP))  //����
    {
        u32 str_width;
        u32 bg_x;
        u32 bg_y;
        strRect m_rect;

        sdkDispGetRowXY(siRow, &bg_x, &bg_y);

        if (ucAtr & SDK_DISP_NOFDISPLINE) //����һ����
        {
            //Trace("devlcd", "����һ����\r\n" );
            m_rect.m_x0 = bg_x; //�������X����
            m_rect.m_y0 = bg_y; //�������Y����
            m_rect.m_x1 = g_row_info.show_lcd_right; //�����յ�X����
            m_rect.m_y1 = bg_y + g_row_info.row_height - 1; //�����յ�Y����
        }
        else {
            sdkDispGetStrWH(tmpStr, &str_width, NULL);
            //Trace("devlcd", "atr = %d\r\n", atr );
            m_rect.m_x0 = x; //�������X����
            m_rect.m_y0 = bg_y; //�������Y����
            m_rect.m_x1 = x + str_width; //�����յ�X����
            m_rect.m_y1 = bg_y + g_row_info.row_height - 1; //�����յ�Y����
        }
        //Trace("devlcd", "m_rect.m_x0 = %d\r\n", m_rect.m_x0 );
        //Trace("devlcd", "m_rect.m_y0 = %d\r\n", m_rect.m_y0 );
        //Trace("devlcd", "m_rect.m_x1 = %d\r\n", m_rect.m_x1 );
        //Trace("devlcd", "m_rect.m_y1 = %d\r\n", m_rect.m_y1 );
        //Trace("devlcd", "tmpStr = %s\r\n", tmpStr );
        ddi_lcd_show_text(x, y, tmpStr);
        ddi_lcd_ioctl(DDI_LCD_CTL_INVERT_RECT, (u32) & m_rect, 0); //���÷���
    }
    else {
        ddi_lcd_show_text(x, y, tmpStr);
    }
    g_row_info.row_x_offset_temp = 0;

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:	sdkDispSaveScreen
   ��������:	������Ļ
   �������:
           const u16 *lcdmem:����
           u32 len:�������ݳ���
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.05.29 19:35:58
*******************************************************************/
s32 sdkDispSaveScreen(const u16 *lcdmem, s32 siLen) {
    if (siLen < (s32) sdkDispGetScreenBufferSize()) {
        Assert(0);
        return SDK_DISP_SIZEERRO;
    }

    if (NULL == lcdmem) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if 0
    {
        strRect temp_rect;
        strPicture strPic;
        s32 rtn;

        temp_rect.m_x0 = 0;
        temp_rect.m_y0 = 0;
        temp_rect.m_x1 = g_row_info.lcd_width;
        temp_rect.m_y1 = g_row_info.lcd_height;
        strPic.m_pic = lcdmem;
        strPic.m_height = g_row_info.lcd_height;
        strPic.m_width = g_row_info.lcd_width;
        rtn = ddi_lcd_extract_rect(&temp_rect, &strPic);

        Trace("devlcd", "rtn = %d\r\n", rtn);
    }
#endif

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:sdkDispDrawProgbarRam
   ��������:����ʾ�����л�һ������
   �������: u32 siRow:                    �к�
                         u32 siX1:                       ���������
                         u32 siX2:                       �������յ�
                         u8 ucValue:                    ������������ȵİٷ���(0~100)
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.08.01 11:08:08
*******************************************************************/
s32 sdkDispDrawProgbarRam(s32 siRow, s32 siX1, s32 siX2, u8 ucValue) {
    return sdkDispDrawProgbar(siRow, siX1, siX2, ucValue);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ����Ȫ
   ��������:sdkDispDrawLine
   ��������:����ʾ�����л�һֱ��
   �������: u32 siX0:                       ֱ�����x����
                          u32 siY0:                       ֱ�����y����
                          u32 siX1:                       ֱ���յ�x����
                          u32 siY1:                       ֱ���յ�y����
                          u32 uiColor:                    ��Ҫ���ߵ���ɫ
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:
   ����ʱ��:2012.11.1
*******************************************************************/
s32 sdkDispDrawLine(s32 siX0, s32 siY0, s32 siX1, s32 siY1, u32 uiColor) {
    strLine sLine;

#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  14:59 */
    sLine.m_x0 = siX0 + g_row_info.show_lcd_left;

    if( sLine.m_x0 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    sLine.m_y0 = siY0 + g_row_info.show_lcd_top;

    if( sLine.m_y0 > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
    sLine.m_x1 = siX1 + g_row_info.show_lcd_left;

    if( sLine.m_x1 > g_row_info.show_lcd_right )
    {
        sLine.m_x1 = g_row_info.show_lcd_right;
    }
    sLine.m_y1 = siY1 + g_row_info.show_lcd_top;

    if( sLine.m_y1 > g_row_info.show_lcd_bottom )
    {
        sLine.m_y1 = g_row_info.show_lcd_bottom;
    }
#else
    sLine.m_x0 = siX0;

    if (sLine.m_x0 > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    sLine.m_y0 = siY0;

    if (sLine.m_y0 > g_row_info.show_lcd_bottom) {
        return SDK_PARA_ERR;
    }
    sLine.m_x1 = siX1;

    if (sLine.m_x1 > g_row_info.show_lcd_right) {
        sLine.m_x1 = g_row_info.show_lcd_right;
    }
    sLine.m_y1 = siY1;

    if (sLine.m_y1 > g_row_info.show_lcd_bottom) {
        sLine.m_y1 = g_row_info.show_lcd_bottom;
    }
#endif /* if 0 */

    if (g_row_info.brush_color != uiColor) {
        if (DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_SET_COLOR, uiColor, 0)) //����ǰ��ɫ
        {
            return SDK_ERR;
        }

        if (DDI_OK != ddi_lcd_show_line(&sLine)) {
            return SDK_ERR;
        }

        if (DDI_OK !=
            ddi_lcd_ioctl(DDI_LCD_CTL_SET_COLOR, g_row_info.brush_color, 0)) //�ָ�ǰ��ɫ(Ĭ���ǰ�ɫ)
        {
            return SDK_ERR;
        }
    }
    else {
        if (DDI_OK != ddi_lcd_show_line(&sLine)) {
            return SDK_ERR;
        }
    }
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ����Ȫ
   ��������:sdkDispDrawPixel
   ��������:����ʾ�����л�һ���㣬���ڸı���Ļ��һ���������
   �������: s32 siX:                       ���x����
                          s32 siY:                       ���y����
                         u32 uiColor:                   ��ɫ
   �������:��
   ��   ��  ֵ:SDK_PARA_ERR:��������SDK_OK:�������óɹ�
   �޸ı�ע:shijianglong  2013.01.10
   ����ʱ��:2012.11.1
*******************************************************************/
s32 sdkDispPutPixel(s32 siX, s32 siY, u32 uiColor) {
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:13 */
    siX += g_row_info.show_lcd_left;

    if( siX > g_row_info.show_lcd_right )
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    siY += g_row_info.show_lcd_top;

    if( siY > g_row_info.show_lcd_bottom)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
#else

    if (siX > g_row_info.show_lcd_right) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (siY > g_row_info.show_lcd_bottom) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#endif /* if 0 */

    if (DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_SET_COLOR, uiColor, 0)) {
        Assert(0);
        return SDK_ERR;
    }

    if (DDI_OK != ddi_lcd_show_pixel(siX, siY)) {
        Assert(0);
        return SDK_ERR;
    }

    if (DDI_OK != ddi_lcd_ioctl(DDI_LCD_CTL_SET_COLOR, g_row_info.brush_color, 0)) {
        Assert(0);
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ���ص���ɫ����
** Parameters:          s32 siX :���x����
                               s32 siY: ���y����
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:
*****************************************************************************/
s32 sdkDispGetPixel(s32 siX, s32 siY) {
    point coord;
    s32 color;

#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  14:45 */
    coord.m_x = siX + g_row_info.show_lcd_left;

    if( coord.m_x > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    coord.m_y = siY + g_row_info.show_lcd_top;

    if( coord.m_y > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }
#else
    coord.m_x = siX;

    if (coord.m_x > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    coord.m_y = siY;

    if (coord.m_y > g_row_info.show_lcd_bottom) {
        return SDK_PARA_ERR;
    }
#endif /* if 0 */


    if (DDI_OK == ddi_lcd_ioctl(DDI_LCD_CTL_GET_DOTCOLOR, (u32)(&coord), (u32)(&color))) {
        return color;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:    ������д���뷨���������
** Parameters:          s32 siStartX
                               s32 siStartY
                               s32 siEndX
                               s32 siEndY
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:
*****************************************************************************/
s32 Private_sdkDispGetHandInputAreaPixel(s32 siStartX, s32 siStartY, s32 siEndX, s32 siEndY) {
    SDK_FONT font;
    u32 high = 0;

    if (siStartX < 0 || siStartY < 0 || siStartX >= g_row_info.lcd_width ||
        siStartY >= g_row_info.lcd_height ||
        siEndX < 0 || siEndY < 0 || siEndX >= g_row_info.lcd_width ||
        siEndY >= g_row_info.lcd_height) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispGetSysFont(&font, 0);

    //ģ�����趨ֵ���е���

    HandInputInfo.siHandInputFlag = true;
    HandInputInfo.siStartX = siStartX;
    HandInputInfo.siStartY = siStartY;
    HandInputInfo.siEndX = siEndX;
    HandInputInfo.siEndY = siEndY;

    high = (siEndY - siStartY) / (SDK_DISP_LINE5 - SDK_DISP_LINE2);

    HandInputInfo.siByteLen = font.len;
    HandInputInfo.siByteWith = (high > font.high ? high : font.high);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 siRow
                               s32 siStartCol
                               s32 siEndCol
** Returned value:
** Created By:		fusuipu  2013.06.04
** Remarks:
*****************************************************************************/
static s32 sdkDispHandInputClearRowRamAt(s32 siRow, s32 siStartCol, s32 siEndCol) {
    s32 six = 0, siy = 0, siendx = 0, siendy = 0;
    s32 high = 0, len = 0;

    len = HandInputInfo.siStartX + siStartCol;
    six = (len > (g_row_info.lcd_width - 1) ? (g_row_info.lcd_width - 1) : len);

    len = HandInputInfo.siStartX + siEndCol;
    siendx = (len > (g_row_info.lcd_width - 1) ? (g_row_info.lcd_width - 1) : len);

    high = (siRow - SDK_DISP_LINE3) * HandInputInfo.siByteWith;
    len = high > 0 ? high : 0;       //����������Ͻ�X��Y�ĸ߶�
    len += HandInputInfo.siStartY;   //�����������0��0��ľ��Ը߶�
    siy = (len > (g_row_info.lcd_height - 1) ? (g_row_info.lcd_height - 1) : len);

    len = siy + HandInputInfo.siByteWith;
    siendy = (len > (g_row_info.lcd_height - 1) ? (g_row_info.lcd_height - 1) : len);

    return sdkDispClearAt(six, siy, siendx, siendy);
}

/*****************************************************************************
** Descriptions:	������Ϊ��λ��sdkDispClearRowRamAt���ģʽ��Ϊ��X,Y��������ģʽ
** Parameters:          s32 siRow
                               s32 siStartCol
                               s32 siEndCol
** Returned value:
** Created By:		fusuipu  2013.06.04
** Remarks:
*****************************************************************************/
s32 Private_sdkDispClearRowRamAt(s32 siRow, s32 siStartCol, s32 siEndCol) {
    if (siRow < 0 || siStartCol < 0 || siEndCol < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (false == HandInputInfo.siHandInputFlag) {
        return sdkDispClearRowRamAt(siRow, siStartCol, siEndCol);
    }
    else {
        return sdkDispHandInputClearRowRamAt(siRow, siStartCol, siEndCol);
    }
}

/*****************************************************************************
** Descriptions:	������д�����������������
** Parameters:          s32 siRow
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:
*****************************************************************************/
static s32 Private_sdkDispHandinputClearRow(s32 siRow) {
    s32 high = 0;
    s32 six = 0, siy = 0, siendx = 0, siendy = 0;

    six = HandInputInfo.siStartX;
    high = (siRow - SDK_DISP_LINE3) * HandInputInfo.siByteWith;
    siy = high > 0 ? high : 0;       //����������Ͻ�X��Y�ĸ߶�
    siy += HandInputInfo.siStartY;   //�����������0��0��ľ��Ը߶�
    siy = (siy > (g_row_info.lcd_height - 1) ? (g_row_info.lcd_height - 1) : siy);

    siendx = HandInputInfo.siEndX;
    siendy = siy + HandInputInfo.siByteWith;
    siendy = (siendy > (g_row_info.lcd_height - 1) ? (g_row_info.lcd_height - 1) : siendy);

    return sdkDispClearAt(six, siy, siendx, siendy);
}

/*****************************************************************************
** Descriptions:	������Ϊ��λ�����ģʽ��Ϊ��X,Y��������ģʽ
** Parameters:          s32 siRow
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:
*****************************************************************************/
s32 Private_sdkDispClearRow(s32 siRow) {
    if (siRow < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (false == HandInputInfo.siHandInputFlag) {
        return sdkDispClearRow(siRow);
    }
    else {
        return Private_sdkDispHandinputClearRow(siRow);
    }
}

/*****************************************************************************
** Descriptions:	������Ϊ��λ�����ģʽ��Ϊ��X,Y��������ģʽ
** Parameters:          s32 siRow
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:
*****************************************************************************/
s32 Private_sdkDispClearRowRam(s32 siRow) {
    if (siRow < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (false == HandInputInfo.siHandInputFlag) {
        return sdkDispClearRowRam(siRow);
    }
    else {
        return Private_sdkDispHandinputClearRow(siRow);
    }
}

/*****************************************************************************
** Descriptions:	���ݵ�ǰ�ĺ����м��㵱ǰ��Ӧ��X��Y����
** Parameters:          s32 siRow
                               s32 eCol
                               u32 ucAtr
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:
*****************************************************************************/
static s32 sdkDispHandInput(s32 siRow, s32 eCol, const u8 *pasStr, u32 ucAtr) {
    s32 len = 0;
    s32 high = 0;
    s32 six = 0, siy = 0;
    s32 siendx = 0, siendy = 0;
    SDK_FONT font;


    sdkDispGetSysFont(&font, 0);

    if (NULL == pasStr) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((ucAtr & SDK_DISP_LDISP) != 0) //�����ģʽ
    {
        len = HandInputInfo.siStartX + eCol;
        six = (len > HandInputInfo.siEndX ? HandInputInfo.siEndX : len);
    }
    else if ((ucAtr & SDK_DISP_RDISP) != 0) //�Ҷ���ģʽ
    {
        len = (HandInputInfo.siEndX - strlen(pasStr) * HandInputInfo.siByteLen);
        six = (len > 0 ? len : 0);
    }
    else if ((ucAtr & SDK_DISP_CDISP) != 0) //����ģʽ
    {
        len = (HandInputInfo.siEndX - strlen(pasStr) * HandInputInfo.siByteLen -
               HandInputInfo.siStartX) / 2;
        Trace("fusuipu", "midlen = %d", len);
        six = (len > 0 ? len : 0);
        six = len + HandInputInfo.siStartX;
    }
    six = (six > (g_row_info.lcd_width - 1) ? (g_row_info.lcd_width - 1) : six);

    high = (siRow - SDK_DISP_LINE3) * HandInputInfo.siByteWith;
    siy = high > 0 ? high : 0;       //����������Ͻ�X��Y�ĸ߶�
    siy += HandInputInfo.siStartY;   //�����������0��0��ľ��Ը߶�
    siy = (siy > (g_row_info.lcd_height - 1) ? (g_row_info.lcd_height - 1) : siy);

    if ((ucAtr & SDK_DISP_FDISP) != 0)  //����ģʽ
    {
        sdkDispAt(six, siy, pasStr);
        return SDK_OK;
    }
    else {
        siendx = six + strlen(pasStr) * font.len;
        siendy = siy + font.high;

        sdkDispNoFiDspAt(six, siy, siendx, siendy);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��sdkDispRowEx��������Ϊ��λ����ʾ����ת��Ϊ��X,Y�������ʾģʽ��ʾ
** Parameters:          SDK_DISP_LINE5
                               0
                               "T9ƴ��"
                               SDK_DISP_FDISP | SDK_DISP_RDISP
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:         ��д����ģʽר��
*****************************************************************************/
s32 Private_sdkDispRow(s32 siRow, s32 eCol, const u8 *pasStr, u32 ucAtr) {
    if (false == HandInputInfo.siHandInputFlag) {
        return sdkDispRow(siRow, eCol, pasStr, ucAtr);
    }
    else {
        Private_sdkDispHandinputClearRow(
                siRow); //sdkDispRow�Դ�����ǰ��ʾ�еĹ��ܣ�sdkDispAtû�У����Լ��ϴ˹���
        sdkDispHandInput(siRow, eCol, pasStr, ucAtr);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��sdkDispRowEx��������Ϊ��λ����ʾ����ת��Ϊ��X,Y�������ʾģʽ��ʾ
** Parameters:          SDK_DISP_LINE5
                               0
                               "T9ƴ��"
                               SDK_DISP_FDISP | SDK_DISP_RDISP
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:         ��д����ģʽר��,��������д������֮��ֱ����ʾ
*****************************************************************************/
s32 Private_sdkDispRowEx(s32 siRow, s32 eCol, const u8 *pasStr, u32 ucAtr) {
    if (false == HandInputInfo.siHandInputFlag) {
        return sdkDispRow(siRow, eCol, pasStr, ucAtr);
    }
    else {
        if (0 == (ucAtr & SDK_DISP_LDISP)) //�ǿ���ģʽ
        {
            ucAtr &= ~(SDK_DISP_CDISP | SDK_DISP_RDISP);
            ucAtr |= SDK_DISP_LDISP;     //����дģʽ�У��̶�Ϊ��ģʽ
        }
        sdkDispHandInput(siRow, eCol, pasStr, ucAtr);
        sdkDispBrushScreen();
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��sdkDispFillRowRam��������Ϊ��λ����ʾ����ת��Ϊ��X,Y�������ʾģʽ��ʾ
** Parameters:          s32 siRow
                               s32 eCol
                               const u8 * pasStr
                               u32 ucAtr
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:
*****************************************************************************/
s32 Private_sdkDispFillRowRam(s32 siRow, s32 eCol, const u8 *pasStr, u32 ucAtr) {
    if (false == HandInputInfo.siHandInputFlag) {
        return sdkDispFillRowRam(siRow, eCol, pasStr, ucAtr);
    }
    else {
        sdkDispHandInput(siRow, eCol, pasStr, ucAtr);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��sdkDispFillRowRam��������Ϊ��λ����ʾ����ת��Ϊ��X,Y�������ʾģʽ��ʾ
** Parameters:          s32 siRow
                               s32 eCol
                               const u8 * pasStr
                               u32 ucAtr
** Returned value:
** Created By:		fusuipu  2013.06.03
** Remarks:
*****************************************************************************/
s32 Private_sdkDispFillRowRamEx(s32 siRow, s32 eCol, const u8 *pasStr, u32 ucAtr) {
    if (false == HandInputInfo.siHandInputFlag) {
        return sdkDispFillRowRam(siRow, eCol, pasStr, ucAtr);
    }
    else {
        if (0 == (ucAtr & SDK_DISP_LDISP)) //�ǿ���ģʽ
        {
            ucAtr &= ~(SDK_DISP_CDISP | SDK_DISP_RDISP);
            ucAtr |= SDK_DISP_LDISP;     //����дģʽ�У��̶�Ϊ��ģʽ
        }
        sdkDispHandInput(siRow, eCol, pasStr, ucAtr);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.06.05
** Remarks:
*****************************************************************************/
s32 Private_sdkDispLineMaxLen(void) {
    if (false == HandInputInfo.siHandInputFlag) {
        return SDK_MAX_LINE_CHAR;
    }
    else {
        return (HandInputInfo.siEndX - HandInputInfo.siStartX) / HandInputInfo.siByteLen;
    }
}

/*****************************************************************************
** Descriptions:	LCD��ʾͼ��
** Parameters:          const enum SDK_DISP_ICON eIcon ��ʾͼ������
                               u8 flag ��ʾ��־��	�μ�Remarks��һ��:1 ��ʾ 0����ʾ
                               s32 siX  �μ�Remarks
                               s32 siY  �μ�Remarks
** Returned value:
** Created By:		shijianglong  2013.06.25
** Remarks:             ������Ϊ350ʱ:��Ҫ��ʾ����ͼ��ʱ���β�yΪҪ��ʾ������,xδʹ�ã���Ϊ0��
                                                        ��ʾ����ͼ��ʱ��y,x��Ϊʹ�ã�����Ϊ0.

                        ������Ϊ��350ʱ(G&200P����):y,xΪͼ�������
                                Icon==ICON_BATTERY:
                                        flag = 99 3�����(��س���)//yangxing add 2007-8-10 8:34
                                        flag = 0�޵���
                                        flag = 1�����
                                        flag = 2�����
                                        flag = 3�����
                                        flag = 98 ����ʾ���ͼ��
                                Icon == ICON_SIGNAL:
                                        flag = 0���ź�
                                        flag = 1�ź�ǿ��1����ʾ
                                        flag = 2�ź�ǿ��2����ʾ
                                        flag = 3�ź�ǿ��3����ʾ
                                        flag = 4�ź�ǿ��4����ʾ
                                        flag = 5�ź�ǿ��5����ʾ
                                Icon == ICON_PLUG��ICON_CDMA��ICON_GPRS:
                                        flag = 0 ����ʾͼ��
                                        flag = ����ʾͼ��
                        ������ΪG&200Pʱ:
                                Icon == ICON_BATTERY:flag == 98 ����ʾ���ͼ�� ������Ч�����״̬SDK�Լ���ȡ
*****************************************************************************/
s32 sdkDispIcon(const SDK_DISP_ICON eIcon, u8 flag, s32 siX, s32 siY) {
    MC_BITMAP bitmapData = {0};
    strRect rect = {0};
    s32 rtn = SDK_ERR;
    s32 clearBattery = 0;
    s32 imageId = -1;

    //static s32 imageId1 = ICON_G2_X;
    //Trace("devlcd", "eIcon :%d\r\n", eIcon );
    switch (eIcon) {
        case SDK_DISP_BATTERY:                         //���ͼ��

            //Trace("devlcd", "flag :%d\r\n", flag );
            if (flag == 98) //����ʾ���ͼ��
            {
                imageId = ICON_G2_WAIMAN;
                clearBattery = 1;
                //return SDK_OK;
            }
#if 0 /*Modify by ���ƽ at 2014.08.26  12:5 */
                else if( flag == 99 ) //3�����(��س���)
                {
                    imageId = ICON_G2_WAIMAN;
                }
#endif /* if 0 */
            else {
                switch (ddi_sys_bat_status()) {
                    case DDI_POWER_NULL:        //��ص������㣬�ػ�
                    {
                        Trace("devlcd", "cflag :����޵���,��Ҫ�����ػ�\r\n");
                        imageId = ICON_G2_WUDIAN;
                    }
                        break;

                    case DDI_POWER_ZERO:        //����磬���0��
                    {
                        Trace("devlcd", "cflag :��ص����ո�,��Ҫ����\r\n");
                        imageId = ICON_G2_WUDIAN;
                        g_power_state = DDI_POWER_ZERO;
                    }
                        break;

                    case DDI_POWER_ONE:         //����磬���1��
                    {
                        Trace("devlcd", "cflag :���1�����\r\n");
                        imageId = ICON_G2_1GE;
                        g_power_state = DDI_POWER_ONE;
                    }
                        break;

                    case DDI_POWER_TWO:         //����磬���2��
                    {
                        Trace("devlcd", "cflag :���2�����\r\n");
                        imageId = ICON_G2_2GE;
                        g_power_state = DDI_POWER_TWO;
                    }
                        break;

                    case DDI_POWER_THREE:       //����磬���3��
                    {
                        Trace("devlcd", "cflag :���3�����\r\n");
                        imageId = ICON_G2_DIANMAN;
                        g_power_state = DDI_POWER_THREE;
                    }
                        break;

                    case DDI_POWER_AD_CHARGE:   //����磬�����
                    case DDI_POWER_AD_CHARGE_ONE: //����磬�����, ���1 ������

                        if (g_power_state >= DDI_POWER_ZERO && g_power_state < DDI_POWER_THREE) {
                            g_power_state++;
                        }
                        else {
                            g_power_state = DDI_POWER_ZERO;
                        }
                        Trace("devlcd", "g_power_state :%d\r\n", g_power_state);

                        if (DDI_POWER_ZERO == g_power_state) {
                            imageId = ICON_G2_CHONG0;
                        }
                        else if (DDI_POWER_ONE == g_power_state) {
                            imageId = ICON_G2_CHONG1;
                        }
                        else if (DDI_POWER_TWO == g_power_state) {
                            imageId = ICON_G2_CHONG2;
                        }
                        else if (DDI_POWER_THREE == g_power_state) {
                            imageId = ICON_G2_WAIMAN;
                        }
                        break;

                    case DDI_POWER_AD_NO_CHARGE: //����磬δ���(��ʱ����Ϊ�������)
                    {
                        imageId = ICON_G2_WAIMAN;
                        Trace("devlcd", "cflag :����磬δ���\r\n");
                        g_power_state = DDI_POWER_THREE;
                    }
                        break;

                    case DDI_POWER_AD_BATTERY_ERR: //����磬��ع���
                    {
                        Trace("devlcd", "cflag :��ع���\r\n");
                        imageId = ICON_G2_GUZHANG;
                    }
                        break;

                    default: {
                        //Trace("devlcd", "cflag :��ع���\r\n" );
                        imageId = ICON_G2_GUZHANG;
                    }
                        break;
                }
            }
            break;

        case SDK_DISP_PLUG:                            //��Դͼ��
        case SDK_DISP_SIGNAL:                             //�ź�ͼ��
        case SDK_DISP_GPRS:                         //GPRSͼ��
        case SDK_DISP_CDMA:                             //CDMAͼ��
        {
            return SDK_OK;
        }

        case SDK_DISP_BT:                             //����ͼ��
        {
            Trace("devlcd", "����\r\n");

            switch (sdkBtGetStatus()) {
                case SDK_BT_STATUS_WORKING: //�����շ�����
                case SDK_BT_STATUS_WAITING: //����״̬(�����ӣ��������շ�)
                    imageId = ICON_G2_BT_LINKING;
                    break;

                case SDK_BT_STATUS_IDLE: //Idle ģʽ(δ����)
                case SDK_ERR:
                    imageId = ICON_G2_BT;
                    break;

                case SDK_EIO:
                default:
                    return SDK_OK;
            }

            Trace("devlcd", "imageId = %d\r\n", imageId);
            break;
        }

        case SDK_DISP_AUDIO:                            //��ƵͨѶͼ��
        {
            Trace("devlcd", "��ƵͨѶ\r\n");
            imageId = ICON_G2_AUDIO;
            break;
        }

        case SDK_DISP_USB:                             //USBͨѶͼ��
        {
            Trace("devlcd", "USBͨѶ\r\n");
            imageId = ICON_G2_USB;
            break;
        }

        default:
            return SDK_PARA_ERR;
    }

#if MACHINEMODEL == SDK_SYS_MACHINE_200P
    imageId += ICON_K200P_1GE;
#endif
#if 0 /*Modify by ���ƽ at 2014.12.24  14:28 */
    imageId1++;

    if( imageId1 == ICON_K200P_1GE )
    {
        imageId1 = ICON_G2_1GE;
    }
#endif /* if 0 */

    if (SDK_OK != getIconBMPData(imageId, &bitmapData)) {
        Assert(0);
        return SDK_ERR;
    }
    //Trace( "zjp", "bitmapData.bmHeight = %d\r\n", bitmapData.bmHeight );
    //Trace( "zjp", "bitmapData.bmWidth = %d\r\n", bitmapData.bmWidth );


    if (g_row_info.is_show_stat_bar) {
        rect.m_x0 = siX;
#if 0 /*Modify by ���ƽ at 2014.08.26  13:56 */

        //ʹ��Ӧ���趨������
        if( g_row_info.show_lcd_top >= siY )
        {
            rect.m_y0 = siY;
        }
        else
        {
            return SDK_PARA_ERR;
        }
        rect.m_x1 = siX + bitmapData.bmWidth;
        rect.m_y1 = siY + bitmapData.bmHeight;

        if( rect.m_y1 >= g_row_info.show_lcd_top )
        {
            rect.m_y1 = g_row_info.show_lcd_top - 1;
        }
#else
        //������ʾ
        rect.m_y0 = (g_row_info.show_lcd_top - bitmapData.bmHeight) >> 1;
        rect.m_x1 = rect.m_x0 + bitmapData.bmWidth - 1;
        rect.m_y1 = rect.m_y0 + bitmapData.bmHeight - 1;
#endif /* if 0 */
    }
    else {
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:13 */
        rect.m_x0 = siX + g_row_info.show_lcd_left;

        if( rect.m_x0 > g_row_info.show_lcd_right )
        {
            return SDK_PARA_ERR;
        }
        rect.m_y0 = siY + g_row_info.show_lcd_top;

        if( rect.m_y0 > g_row_info.show_lcd_bottom)
        {
            return SDK_PARA_ERR;
        }
        rect.m_x1 = siX + bitmapData.bmWidth - 1 + g_row_info.show_lcd_left;

        if( rect.m_x1 > g_row_info.show_lcd_right )
        {
            rect.m_x1 = g_row_info.show_lcd_right;
        }
        rect.m_y1 = siY + bitmapData.bmHeight - 1 + g_row_info.show_lcd_top;

        if( rect.m_y1 > g_row_info.show_lcd_bottom )
        {
            rect.m_y1 = g_row_info.show_lcd_bottom;
        }
#else
        rect.m_x0 = siX;

        if (rect.m_x0 > g_row_info.show_lcd_right) {
            return SDK_PARA_ERR;
        }
        rect.m_y0 = siY;

        if (rect.m_y0 > g_row_info.show_lcd_bottom) {
            return SDK_PARA_ERR;
        }
        rect.m_x1 = siX + bitmapData.bmWidth - 1;

        if (rect.m_x1 > g_row_info.show_lcd_right) {
            rect.m_x1 = g_row_info.show_lcd_right;
        }
        rect.m_y1 = siY + bitmapData.bmHeight - 1;

        if (rect.m_y1 > g_row_info.show_lcd_bottom) {
            rect.m_y1 = g_row_info.show_lcd_bottom;
        }
#endif /* if 0 */
    }
#if 0 /*Modify by ���ƽ at 2014.09.10  14:27 */
    Trace("zjp", "bitmapData.bmHeight = %d\r\n", bitmapData.bmHeight);
    Trace("zjp", "bitmapData.bmWidth = %d\r\n", bitmapData.bmWidth);

    Trace("zjp", "rect.m_x0 = %d\r\n", rect.m_x0);
    Trace("zjp", "rect.m_y0 = %d\r\n", rect.m_y0);
    Trace("zjp", "rect.m_x1 = %d\r\n", rect.m_x1);
    Trace("zjp", "rect.m_y1 = %d\r\n", rect.m_y1);
#endif /* if 0 */
    rtn = ddi_lcd_clear_rect(&rect);

    if (!clearBattery) {
#if 0

        if( MACHINEMODEL == SDK_SYS_MACHINE_200P ) //200P ��Ҫת����ȡģ���ֽڵ���
        {
            strPicture lpstrPic;
            s32 lenV = ((bitmapData.bmHeight + 7) >> 3);
            s32 lenH = ((bitmapData.bmWidth + 7) >> 3);
            u8 * vBmpData = sdkGetMem(lenV * bitmapData.bmWidth);
            s32 i = 0;
            s32 j = 0;

            if( !vBmpData )
            {
                return SDK_ERR;
            }
            memset(vBmpData, 0, lenV * bitmapData.bmWidth);

            //TraceHex( "devlcd", "bitmapData.bmBits", bitmapData.bmBits, bitmapData.bmBitSize );
            for( i = 0; i < bitmapData.bmWidth; i++ )
            {
                for( j = bitmapData.bmHeight - 1; j >= 0; j-- )
                {
                    if( bitmapData.bmBits[lenH * j + (i >> 3)] & (0x1 << (i % 8)) )
                    {
                        //printf("��");
                        vBmpData[((bitmapData.bmHeight - 1 - j) >> 3) * bitmapData.bmWidth + i] |= (0x1 << ((bitmapData.bmHeight - 1 - j) % 8));
                    }
                    else
                    {
                        //printf("��");
                    }
                }

                //printf("\r\n");
            }

            //printf("1111111111111\r\n\r\n\r\n\r\n\r\n");
#if 0 /*Modify by ���ƽ at 2014.12.29  14:18 */

            //TraceHex( "devlcd", "vBmpData", vBmpData, lenV * bitmapData.bmWidth );
            for( j = 0; j < bitmapData.bmHeight; j++ )
            {
                for( i = 0; i < bitmapData.bmWidth; i++ )
                {
                    if(  vBmpData[i * lenV + (j >> 3)] & (0x1 << (j % 8)) )
                    {
                        printf("��");
                    }
                    else
                    {
                        printf("��");
                    }
                }

                printf("\r\n");
            }

#endif /* if 0 */
            lpstrPic.m_height = bitmapData.bmHeight;
            lpstrPic.m_width = bitmapData.bmWidth;
            lpstrPic.m_pic = vBmpData;
            rtn = ddi_lcd_show_picture(&rect, &lpstrPic);
            sdkFreeMem(vBmpData);
        }
        else
#endif
        {
            rtn = sdkDispDrawIcon(rect.m_x0, rect.m_y0, &bitmapData);
        }
        sdkFreeMem(bitmapData.bmBits);
        return rtn;
    }
    else {
        if (rtn == DDI_OK) {
            return SDK_OK;
        }
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	���ò���������ɫ
** Parameters:          u32 uiColor:��ɫֵ
** Returned value:
** Created By:		shijianglong  2013.07.11
** Remarks:
*****************************************************************************/
void sdkDispSetBackColor(u32 uiColor) {
#if LCD_PIXEL_BITS != 1  /*Modify by ���ƽ at 2015.01.13  14:49 */
    strBackGround strBG;

    strBG.m_mode = 0x00;
    strBG.m_purecolor = uiColor;

    strBG.m_rect.m_x0 = 0;
    strBG.m_rect.m_y0 = 0;
    strBG.m_rect.m_x1 = g_row_info.show_lcd_right;
    strBG.m_rect.m_y1 = g_row_info.show_lcd_bottom;
    ddi_lcd_ioctl(DDI_LCD_CTL_SET_BACKGROUND, (u32)(&strBG), 0);
#endif /* if 0 */
}

/*****************************************************************************
** Descriptions:	���ò���������ɫ
** Parameters:          u16 usColor:��ɫֵ
** Returned value:
** Created By:		shijianglong  2013.07.11
** Remarks:
*****************************************************************************/
void sdkDispSetFontColor(u32 uiColor) {
    if (DDI_OK == ddi_lcd_ioctl(DDI_LCD_CTL_SET_COLOR, uiColor, 0)) {
        g_row_info.brush_color = uiColor;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ������ɫ
** Parameters:          void
** Returned value:
** Created By:		tjb  2014.04.15
** Remarks:
*****************************************************************************/
s32 sdkDispGetFontColor(void) {
    u32 color;

    if (DDI_OK == ddi_lcd_ioctl(DDI_LCD_CTL_GET_COLOR, (u32)(&color), 0)) {
        return (s32) color;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	���������С
** Parameters:          &sysFont
** Returned value:
** Created By:		fusuipu  2013.08.05
** Remarks:             ���ڶ���������
*****************************************************************************/
static s32 sdkDispSetSysFont(const SDK_FONT *currfont, u8 flag) {
    strFont tempfont = {0};

    memcpy(&tempfont, &g_row_info.str_font, sizeof(strFont));

    if (0 == flag) {
        if (6 == currfont->len) {
            tempfont.m_ascsize = E_WORD_ASCII_6_12;
        }
        else if (8 == currfont->len) {
            tempfont.m_ascsize = E_WORD_ASCII_8_16;
        }
        else if (12 == currfont->len) {
            tempfont.m_ascsize = E_WORD_ASCII_12_24;
        }
        else if (16 == currfont->len) {
            tempfont.m_ascsize = E_WORD_ASCII_16_32;
        }
        else if (24 == currfont->len) {
            tempfont.m_ascsize = E_WORD_ASCII_24_48;
        }
        else {
            return SDK_ERR;
        }

        if (tempfont.m_ascsize == g_row_info.str_font.m_ascsize) {
            return SDK_OK;
        }
    }
    else {
        if (12 == currfont->len) {
            tempfont.m_nativesize = E_WORD_NATIVE_12_12;
        }
        else if (16 == currfont->len) {
            tempfont.m_nativesize = E_WORD_NATIVE_16_16;
        }
        else if (20 == currfont->len) {
            tempfont.m_nativesize = E_WORD_NATIVE_20_20;
        }
        else if (24 == currfont->len) {
            tempfont.m_nativesize = E_WORD_NATIVE_24_24;
        }
        else if (32 == currfont->len) {
            tempfont.m_nativesize = E_WORD_NATIVE_32_32;
        }
        else if (48 == currfont->len) {
            tempfont.m_nativesize = E_WORD_NATIVE_48_48;
        }
        else {
            return SDK_ERR;
        }

        if (tempfont.m_ascsize == g_row_info.str_font.m_ascsize) {
            return SDK_OK;
        }
    }

    if (DDI_OK == ddi_lcd_ioctl(DDI_LCD_CTL_SET_FONT, (u32)(&tempfont), 0)) {
        memcpy(&g_row_info.str_font, &tempfont, sizeof(strFont));
        return SDK_OK;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰ������ֿ�
** Parameters:          SDK_FONT *currfont
                    u8 flag false:ascii����   true:��������
** Returned value:
** Created By:		���ƽ  2014.04.28
** Remarks:
*****************************************************************************/
void sdkDispGetSysFont(SDK_FONT *currfont, u8 flag) //δ��������������ʹ���������壬���廹���������
{
    if (0 == flag) {
        switch (g_row_info.str_font.m_ascsize) {
            case E_WORD_ASCII_6_12: //6*12
            {
                currfont->len = 6;
                currfont->high = 12;
            }
                break;

            case E_WORD_ASCII_8_16: //8*16
            {
                currfont->len = 8;
                currfont->high = 16;
            }
                break;

            case E_WORD_ASCII_12_24: //12*24
            {
                currfont->len = 12;
                currfont->high = 24;
            }
                break;

            case E_WORD_ASCII_16_32: //16*32
            {
                currfont->len = 16;
                currfont->high = 32;
            }
                break;

            case E_WORD_ASCII_24_48: //24*48
            {
                currfont->len = 24;
                currfont->high = 48;
            }
                break;

            default: {
                currfont->len = 12;
                currfont->high = 24;
            }
                break;
        }
    }
    else {
        switch (g_row_info.str_font.m_nativesize) {
            case E_WORD_NATIVE_12_12: //12*12
            {
                currfont->len = 12;
                currfont->high = 12;
            }
                break;

            case E_WORD_NATIVE_16_16: //16*16
            {
                currfont->len = 16;
                currfont->high = 16;
            }
                break;

            case E_WORD_NATIVE_20_20: //20*20
            {
                currfont->len = 20;
                currfont->high = 20;
            }
                break;

            case E_WORD_NATIVE_24_24: //24*24
            {
                currfont->len = 24;
                currfont->high = 24;
            }
                break;

            case E_WORD_NATIVE_32_32: //32*32
            {
                currfont->len = 32;
                currfont->high = 32;
            }
                break;

            case E_WORD_NATIVE_48_48: //48*48
            {
                currfont->len = 48;
                currfont->high = 48;
            }
                break;

            default: {
                currfont->len = 24;
                currfont->high = 24;
            }
                break;
        }
    }
}

/*****************************************************************************
** Descriptions:	����ϵͳ�����С
** Parameters:          const s32 siFontSize
** Returned value:
** Created By:		fusuipu  2014.04.28
** Remarks:
*****************************************************************************/
s32 sdkDispSetFontSize(const SDK_DISP_FONT siFontSize) {
    SDK_FONT newfont = {0};

    newfont.high = (u32) siFontSize;
    newfont.len = (u32) siFontSize;
    newfont.len = newfont.len >> 1;
    return sdkDispSetSysFont(&newfont, 0);
}

/*****************************************************************************
** Descriptions:	��ȡ�����С
** Parameters:          Input: None
** Returned value:	��������ĸ߶�
** Created By:		fusuipu  2014.04.28
** Remarks:             ���֣�����Ŀ�ȵ��ڸ߶ȣ�ASC�������ȵ��ڸ߶�һ��
*****************************************************************************/
s32 sdkDispGetFontSize() {
    SDK_FONT newfont = {0};

    sdkDispGetSysFont(&newfont, 0);
    return newfont.high;
}

/*****************************************************************************
** Descriptions:	��������
** Parameters:          SDK_FONT *currfont
                    u8 flag
** Returned value:
** Created By:		tjb  2014.04.15
** Remarks:
*****************************************************************************/
void sdkDispSetFont(SDK_FONT *newfont, u8 flag) {
    if (newfont == NULL) {
        return;
    }
    sdkDispSetSysFont(newfont, flag);
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰ������
** Parameters:          SDK_FONT * currfont
                    u8 flag
** Returned value:
** Created By:		tjb  2014.04.15
** Remarks:
*****************************************************************************/
void sdkDispGetFont(SDK_FONT *currfont, u8 flag) {
    if (currfont == NULL) {
        return;
    }
    sdkDispGetSysFont(currfont, flag);
}

/*****************************************************************************
** Descriptions:	����Ļ��color--8����
** Parameters:          s32 siX:x����
                               s32 siY:y����
                               u8 uiColor:8���������
** Returned value:
** Created By:		shijianglong  2013.08.09
** Remarks:             �ڰ�����ͼ��ר�ã��Ƚ�ͼ��ת��λͼ���飬�ٵ��˺�����ʾ�������ڻ�һ����ĺ���sdkDispPutPixel��
*****************************************************************************/
s32 sdkDispDrawDot(s32 siX, s32 siY, u32 uiColor) //��ʾͼ����ʹ�� sdkDispIcon
{
    uiColor = uiColor & 0x000000ff;
#if LCD_PIXEL_BITS == 1 /*Modify by ���ƽ at 2014.05.08  11:33 */
    u32 i = 0;

    for (i = 0; i < 8; i++) {
        if (uiColor & (1 << i)) {
            ddi_lcd_show_pixel(siX + i, siY);
        }
    }
#else
    sdk_warning_fun_not_work(__FUNCTION__);
#endif
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	�ڻ��滭color--8����
** Parameters:          s32 siX:x����
                               s32 siY:y����
                               u32 uiColor:8���������
** Returned value:
** Created By:		shijianglong  2013.08.09
** Remarks:             �ڰ�����ʾͼ��ר��.����μ� sdkDispDrawDot ������
*****************************************************************************/
s32 sdkDispDrawDotRam(s32 siX, s32 siY, u32 uiColor) //��ʾͼ����ʹ�� sdkDispIcon
{
    sdkDispDrawDot(siX, siY, uiColor);
    return SDK_OK;
}

#if 0
/*****************************************************************************
** Descriptions:	��ȡ�������ʹ���
** Parameters:          SDK_PRINT_CH_FONT eChFont
                               u8 *pDevFont
** Returned value:
** Created By:		shijianglong  2013.04.09
** Remarks:
*****************************************************************************/
static s32 GetChFontType(SDK_CH_FONT eChFont, u8 * pDevFont)
{
    s32 i;

    *pDevFont = 0x41;

    for(i = SDK_PRN_CH12X12; i <= SDK_PRN_CH48X48; )
    {
        if(i == eChFont)
        {
            return SDK_OK;
        }
        i++;
        (*pDevFont)++;
    }

    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡasc�������ʹ���
** Parameters:          SDK_PRINT_ASC_FONT eAscFont
                               u8 *pDevFont
** Returned value:
** Created By:		shijianglong  2013.04.09
** Remarks:
*****************************************************************************/
static s32 GetAscFontType(SDK_ASC_FONT eAscFont, u8 * pDevFont)
{
    s32 i;

    *pDevFont = 0x41;

    for(i = 0; i <= SDK_PRN_ASCII5X7; )
    {
        if(i == eAscFont)
        {
            return SDK_OK;
        }
        i++;
        (*pDevFont)++;
    }

    return SDK_ERR;
}

#endif

/*****************************************************************************
** Descriptions:	��ʾһ���ַ���
** Parameters:          u8 ucAtr:��ʾ����
                                {
                                    SDK_DISP_FDISP/SDK_DISP_NOFDISP ����/����
                                    SDK_DISP_LDISP|SDK_DISP_CDISP|SDK_DISP_RDISP ��ʾ����ʼλ�� ��|��|��
                                    SDK_DISP_INCOL ������ʾ����һ��
                                }
                                const SDK_DISP_MULT_DATA *pData:һ����ʾ��������ṹ��
                               ...:�ɴ����SDK_DISP_MULT_DATA�ṹ��
** Returned value:	SDK_OK  �ɹ�
                                SDK_ERR ʧ�ܣ�����ԭ��
                                SDK_PARA_ERR���ΪNULL���߳������߶�
** Created By:		shijianglong  2013.03.08
** Remarks: ��������ȵ��ַ���������ַ�����ʾ���߶ȳ������߶ȷ��ز�����
*****************************************************************************/
s32 sdkDispFormatFillRam(u8 ucAtr, const SDK_DISP_MULT_DATA *pData, ...) {
    s32 rtn = SDK_OK;
    //u32 itemNmb = 0;
    strFont old_font, new_font;
    u32 need_set_font = 0;
    u32 x = 0, strLen = 0;
    u32 text_x; //����ԭ��ĺ�����
    u32 text_y; //����ԭ���������
    u8 tmpStr[64] = {0};

    //s32 ret = SDK_OK;
    s32 i = 0; //, len = 0;
    SDK_DISP_MULT_DATA *pstTempData = NULL;

    /*�жϲ����Ƿ�Ƿ�*/
    if (pData == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ucAtr &= ucAtr & (~(SDK_DISP_RDISP | SDK_DISP_CDISP)) | SDK_DISP_LDISP; //ȥ���Ҷ���;���,Ĭ��Ϊ�����
    memcpy(&old_font, &g_row_info.str_font, sizeof(strFont)); //����ϵͳ����

    while (1) {
        memcpy(&pstTempData, (void *) ((int) &pData + i * sizeof(pstTempData)),
               sizeof(&pstTempData));

        if (pstTempData != NULL) {
            //itemNmb++;
            //Trace( "zjp", "multData[%d].pasText = %s\r\n", itemNmb, multData.pasText );
            if (SDK_ERR == sdkDispConvertNativeFont(pstTempData->eChFont,
                                                    &new_font.m_nativesize)) //��ȡ��������
            {
                Assert(0);
                rtn = SDK_PARA_ERR;
                break;
            }

            if (new_font.m_nativesize != g_row_info.str_font.m_nativesize) {
                g_row_info.str_font.m_nativesize = new_font.m_nativesize;
                need_set_font = 1;
            }

            if (SDK_ERR == sdkDispConvertAsciiFont(pstTempData->eAscFont, &new_font.m_font,
                                                   &new_font.m_ascsize)) //��ȡascii����
            {
                Assert(0);
                rtn = SDK_PARA_ERR;
                break;
            }

            if (new_font.m_ascsize != g_row_info.str_font.m_ascsize ||
                new_font.m_font != g_row_info.str_font.m_font) {
                g_row_info.str_font.m_ascsize = new_font.m_ascsize;
                g_row_info.str_font.m_font = new_font.m_font;
                need_set_font = 1;
            }

            if (need_set_font == 1) //����������
            {
                need_set_font = 2;
                ddi_lcd_ioctl(DDI_LCD_CTL_SET_FONT, (u32)(&g_row_info.str_font), 0);
            }
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:5 */

            if( pstTempData->siX + g_row_info.show_lcd_left > g_row_info.show_lcd_right )
            { //�˳�
                Assert(0);
                rtn = SDK_PARA_ERR;
                break;
            }

            if( pstTempData->siY + g_row_info.show_lcd_top > g_row_info.show_lcd_bottom )
            { //�˳�
                Assert(0);
                rtn = SDK_PARA_ERR;
                break;
            }
            text_x = pstTempData->siX + g_row_info.show_lcd_left;
            text_y = pstTempData->siY + g_row_info.show_lcd_top;

            g_row_info.row_x_offset_temp = (u16)pstTempData->siX;

            //Trace( "devlcdtest", "textInfo[i].m_attrbution = %d\r\n", textInfo[i].m_attrbution );
            if( ucAtr & SDK_DISP_CDISP ) //����
            {
                strLen = sdkDispGetRowStrXY(SDK_DISP_LINE1, ALIGN_CENTER, pstTempData->pasText, &x, NULL);
            }
            else if( ucAtr & SDK_DISP_RDISP ) //�Ҷ���
            {
                strLen = sdkDispGetRowStrXY(SDK_DISP_LINE1, ALIGN_RIGHT, pstTempData->pasText, &x, NULL);
            }
            else
            { //Ĭ�������
                strLen = sdkDispGetRowStrXY(SDK_DISP_LINE1, ALIGN_LEFT, pstTempData->pasText, &x, NULL);
            }
#else

            if (pstTempData->siX > g_row_info.show_lcd_right) { //�˳�
                Assert(0);
                rtn = SDK_PARA_ERR;
                break;
            }

            if (pstTempData->siY > g_row_info.show_lcd_bottom) { //�˳�
                Assert(0);
                rtn = SDK_PARA_ERR;
                break;
            }
            text_x = g_row_info.show_lcd_left;
            text_y = g_row_info.show_lcd_top;
            g_row_info.show_lcd_left = 0;
            g_row_info.show_lcd_top = 0;
            g_row_info.row_x_offset_temp = (u16) pstTempData->siX;

            //Trace( "devlcdtest", "textInfo[i].m_attrbution = %d\r\n", textInfo[i].m_attrbution );
            if (ucAtr & SDK_DISP_CDISP) //����
            {
                strLen = sdkDispGetRowStrXY(SDK_DISP_LINE1, ALIGN_CENTER, pstTempData->pasText, &x,
                                            NULL);
            }
            else if (ucAtr & SDK_DISP_RDISP) //�Ҷ���
            {
                strLen = sdkDispGetRowStrXY(SDK_DISP_LINE1, ALIGN_RIGHT, pstTempData->pasText, &x,
                                            NULL);
            }
            else { //Ĭ�������
                strLen = sdkDispGetRowStrXY(SDK_DISP_LINE1, ALIGN_LEFT, pstTempData->pasText, &x,
                                            NULL);
            }
            g_row_info.show_lcd_left = text_x;
            g_row_info.show_lcd_top = text_y;
            text_x = pstTempData->siX;
            text_y = pstTempData->siY;
#endif
            //Trace( "devlcdtest", "x = %d\r\n", x );
            //Trace( "devlcdtest", "len = %d\r\n", len );
            memcpy(tmpStr, pstTempData->pasText, strLen);
            tmpStr[strLen] = 0;
            //Trace( "devlcdtest", "tmpStr = %s\r\n", tmpStr );


            if (ucAtr & SDK_DISP_FDISP) //����
            {
                //Trace("devlcd", "x = %d\r\n", x );
                //Trace("devlcd", "text_y = %d\r\n", text_y );
                //Trace("devlcd", "tmpStr = %s\r\n", tmpStr );
                ddi_lcd_show_text(x, text_y, tmpStr);
            }
            else //����
            {
                strRect temp_rect;

                sdkDispGetStrWH(tmpStr, &temp_rect.m_x1, &temp_rect.m_y1);
                temp_rect.m_x0 = x;
                temp_rect.m_y0 = text_y;
                temp_rect.m_x1 += x;
                temp_rect.m_y1 += text_y - 1;
                //Trace("devlcd", "temp_rect.m_x0 = %d\r\n", temp_rect.m_x0 );
                //Trace("devlcd", "temp_rect.m_x1 = %d\r\n", temp_rect.m_x1 );
                //Trace("devlcd", "temp_rect.m_y0 = %d\r\n", temp_rect.m_y0 );
                //Trace("devlcd", "temp_rect.m_y1 = %d\r\n", temp_rect.m_y1 );
                ddi_lcd_show_text(x, text_y, tmpStr);
                ddi_lcd_ioctl(DDI_LCD_CTL_INVERT_RECT, (u32)(&temp_rect), 0);
                //Trace("devlcd", "text_y = %d\r\n", text_y );
                //Trace("devlcd", "tmpStr = %s\r\n", tmpStr );
            }
        }
        else {
            break;
        }
        i++;
    }

    g_row_info.row_x_offset_temp = 0;

    if (need_set_font > 0) //�޸Ĺ�ϵͳ����,��Ҫ��ԭ
    {
        if (DDI_OK == ddi_lcd_ioctl(DDI_LCD_CTL_SET_FONT, (u32)(&old_font), 0)) {
            memcpy(&g_row_info.str_font, &old_font, sizeof(strFont));
        }
    }
    return rtn;
}

/*****************************************************************************
** Descriptions:	��ȡӢ��������ռ�ĵ������
** Parameters:          SDK_PRINT_FONT uiFont
                               SDK_FONT *uiFontSize
** Returned value:
** Created By:		���ƽ  2014.11.03
** Remarks:
*****************************************************************************/
s32 sdkDispGetEnDots(SDK_PRINT_FONT uiFont, SDK_FONT *uiFontSize) {
    s32 Dots = 0;

    //�ж�ASC����,������
    switch (uiFont.uiAscFont) {
        //����
        case SDK_PRN_ASCII6X8:
            Dots = 6;
            uiFontSize->high = 8;
            break;

        case SDK_PRN_ASCII6X12:
            Dots = 6;
            uiFontSize->high = 12;
            break;

        case SDK_PRN_ASCII8X16:
            Dots = 8;
            uiFontSize->high = 16;
            break;

        case SDK_PRN_ASCII12X24:
            Dots = 12;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X24:
            Dots = 16;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X32:
            Dots = 16;
            uiFontSize->high = 32;
            break;

        case SDK_PRN_ASCII16X48:
            Dots = 16;
            uiFontSize->high = 48;
            break;

            //����Ӻ�
        case SDK_PRN_ASCII8X16B:
            Dots = 8;
            uiFontSize->high = 16;
            break;

        case SDK_PRN_ASCII12X24B:
            Dots = 12;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X24B:
            Dots = 16;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X32B:
            Dots = 16;
            uiFontSize->high = 32;
            break;

        case SDK_PRN_ASCII16X48B:
            Dots = 16;
            uiFontSize->high = 48;
            break;

            //��Բ
        case SDK_PRN_ASCII12X24Y:
            Dots = 12;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X24Y:
            Dots = 16;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X32Y:
            Dots = 16;
            uiFontSize->high = 32;
            break;

        case SDK_PRN_ASCII16X48Y:
            Dots = 16;
            uiFontSize->high = 48;
            break;

            //��Բ�Ӻ�
        case SDK_PRN_ASCII12X24YB:
            Dots = 12;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X24YB:
            Dots = 16;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_ASCII16X32YB:
            Dots = 16;
            uiFontSize->high = 32;
            break;

        case SDK_PRN_ASCII16X48YB:
            Dots = 16;
            uiFontSize->high = 48;
            break;

            //����
        case SDK_PRN_ASCII20X32B:
            Dots = 20;
            uiFontSize->high = 32;
            break;

        case SDK_PRN_ASCII28X48B:
            Dots = 28;
            uiFontSize->high = 48;
            break;

        case SDK_PRN_ASCII24X48:
            Dots = 24;
            uiFontSize->high = 48;
            break;

        case SDK_PRN_ASCII16X16B:
            Dots = 16;
            uiFontSize->high = 16;
            break;

        case SDK_PRN_ASCII5X7:
            Dots = 5;
            uiFontSize->high = 7;
            break;

        default:
            Assert(0);
            Dots = SDK_ERR;
            break;
    }

    uiFontSize->len = Dots;
    return Dots;
}

/*****************************************************************************
** Descriptions:	��ȡ����������ռ�ĵ���
** Parameters:          SDK_PRINT_FONT uiFont
                               SDK_FONT *uiFontSize
** Returned value:
** Created By:		���ƽ  2014.11.03
** Remarks:
*****************************************************************************/
s32 sdkDispGetCnDots(SDK_PRINT_FONT uiFont, SDK_FONT *uiFontSize) {
    s32 Dots = 0;

    switch (uiFont.uiChFont) {
        case SDK_PRN_CH12X12:
            Dots = 12;
            uiFontSize->high = 12;
            break;

        case SDK_PRN_CH16X16:
            Dots = 16;
            uiFontSize->high = 16;
            break;

        case SDK_PRN_CH20X20:
            Dots = 20;
            uiFontSize->high = 20;
            break;

        case SDK_PRN_CH24X24:
            Dots = 24;
            uiFontSize->high = 24;
            break;

        case SDK_PRN_CH32X32:
            Dots = 32;
            uiFontSize->high = 32;
            break;

        case SDK_PRN_CH48X48:
            Dots = 48;
            uiFontSize->high = 48;
            break;

        default:
            Assert(0);
            Dots = SDK_ERR;
            break;
    }

    uiFontSize->len = Dots;
    return Dots;
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰ�ַ�����ռ���ܵ���
** Parameters:          const SDK_PRINT_DATA *pData
** Returned value:
** Created By:		fusuipu  2014.09.28
** Remarks:
*****************************************************************************/
static s32 sdkDispMultGetAviDotStr(const SDK_DISP_MULT_DATA *pData, s32 siTotalDots,
                                   SDK_PRINT_FONT uiFont) {
    s32 i = 0, len = 0;
    s32 en_dots = 0;
    s32 cn_dots = 0;
    s32 dot_len = 0;

    SDK_FONT font;

    if (NULL == pData || siTotalDots < 0) {
        return SDK_PARA_ERR;
    }
    memset(&font, 0, sizeof(SDK_FONT));
    en_dots = sdkDispGetEnDots(uiFont, &font);
    cn_dots = sdkDispGetCnDots(uiFont, &font);
    len = strlen(pData->pasText);

    for (i = 0; i < len;) {
        if (pData->pasText[i] > 0x80) {
            dot_len += cn_dots;

            if (dot_len > siTotalDots) {
                break;
            }
            i += 2;
        }
        else {
            dot_len += en_dots;

            if (dot_len > siTotalDots) {
                break;
            }
            i++;
        }
    }

    return i;
}

/*****************************************************************************
** Descriptions:	�����ʾ����ǰ�ڵ��Ƿ���Ҫ����
** Parameters:      s32 siRowGap
                    const SDK_PRINT_DATA *pData
** Returned value:	�ڼ����ֽ���Ҫ��ʼ����
** Created By:		fusuipu  2014.09.28
** Remarks:             0��ʾ�����ڵ���Ҫ����
*****************************************************************************/
static s32 sdkDispMultGetChangeLinePlace(const SDK_DISP_MULT_DATA *pData) {
    s32 len = 0;
    SDK_PRINT_FONT font;

    if (NULL == pData) {
        return SDK_PARA_ERR;
    }
    memset(&font, 0, sizeof(SDK_PRINT_FONT));
    font.uiAscFont = pData->eAscFont;
    font.uiAscZoom = SDK_PRN_ZOOM_N;
    font.uiChFont = pData->eChFont;
    font.uiChZoom = SDK_PRN_ZOOM_N;

    if (pData->siX >= g_row_info.lcd_width) {
        return 0; //ֱ�ӻ���
    }
    len = sdkDispMultGetAviDotStr(pData, g_row_info.lcd_width - pData->siX, font);

    if (len < (s32) strlen(pData->pasText)) {
        return len;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�жϵ�ǰ�ڵ������Ƿ�ᳬ��һ������ʾ
** Parameters:          SDK_DISP_MULT_DATA *pData
** Returned value:
** Created By:		fusuipu  2014.10.21
** Remarks:
*****************************************************************************/
static bool sdkDispIsStrOverScreen(SDK_DISP_MULT_DATA *pData, s32 *psiMaxHigh) {
    SDK_PRINT_FONT font;
    SDK_FONT en_font;
    SDK_FONT cn_font;
    u32 i = 0;
    bool en = false, cn = false;

    if (NULL == pData || NULL == psiMaxHigh) {
        return false;
    }
    memset(&font, 0, sizeof(SDK_PRINT_FONT));
    font.uiAscFont = pData->eAscFont;
    font.uiAscZoom = SDK_PRN_ZOOM_N;
    font.uiChFont = pData->eChFont;
    font.uiChZoom = SDK_PRN_ZOOM_N;

    memset(&en_font, 0, sizeof(SDK_FONT));
    memset(&cn_font, 0, sizeof(SDK_FONT));
    sdkDispGetEnDots(font, &en_font);
    sdkDispGetCnDots(font, &cn_font);

    if (pData->siY >= g_row_info.lcd_height) {
        return true;
    }

    for (i = 0; i < strlen(pData->pasText); i++) {
        if (pData->pasText[i] > 0x80) {
            cn = true;
            i += 2;
        }
        else if (pData->pasText[i] < 0x80) {
            en = true;
            i++;
        }

        if (true == cn && true == en) {
            break;
        }
    }

    *psiMaxHigh = (cn_font.high > en_font.high ? cn_font.high : en_font.high);

    if (true == en) {
        *psiMaxHigh = en_font.high;
    }

    if (true == cn) {
        *psiMaxHigh = (cn_font.high > en_font.high ? cn_font.high : en_font.high);
    }

    if (pData->siY + (*psiMaxHigh) > g_row_info.lcd_width) {
        return true;
    }
    return false;
}

/*****************************************************************************
** Descriptions:	�����ʾ�ӿ�
** Parameters:          u8 ucAtr                                :��ʾ����
                    SDK_DISP_MULT_RET_INFO *pstRetInfor     :��ʾ�����Ϣ
                    const SDK_DISP_MULT_DATA *pData         :��Ҫ��ʾ���ַ���
                    ...                                     :�ɱ����
** Returned value:	SDK_OK                                  :��ʾ�ɹ�
                    SDK_ERR                                 :��ʾʧ��
                    SDK_PARA_ERR                            :��������
                    SDK_DISP_MULTI_OVER_DISP                :��ʾ���ݳ���һ��
** Created By:		fusuipu  2014.10.20
** Remarks:             ������ֵΪSDK_DISP_MULTI_OVER_DISPʱ����ϸ����Ϣ�����pstRetInfor
                    ����
*****************************************************************************/
s32 sdkDispFormatFillRamEx(u8 ucAtr, SDK_DISP_MULT_RET_INFO *pstRetInfor,
                           const SDK_DISP_MULT_DATA *pData, ...) {
    SDK_DISP_MULT_DATA stTempData[5]; //�ٶ�һ�������10���ڵ㣬ʵ�����ĵ�������������3�����㹻��
    SDK_DISP_MULT_DATA *pstBuf[5];
    SDK_DISP_MULT_DATA *psttemp = NULL;
    SDK_DISP_PIXEL pixel = sdkDispGetScreenPixel();
    u8 temp[256] = {0};
    u32 i = 0, j = 0;
    u32 start_node = 0;
    s32 len = 0;
    u32 total_node = 0;
    s32 cur_nod_high = 0, max_high = 0;
    static bool is_first_record = true;

    if (NULL == pstRetInfor || NULL == pData) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(stTempData, 0, sizeof(stTempData));

    //��ȡ�ɱ��β�ָ��ֵ
    while (1) {
        pstBuf[total_node] = NULL;
        memcpy(&psttemp, (void *) ((int) &pData + total_node * sizeof(SDK_DISP_MULT_DATA * )),
               sizeof(SDK_DISP_MULT_DATA * )); //��д������A9�в����ڴ�����//zhongjiahong 2014.01.06 11:28

        if (NULL == psttemp || total_node >= (sizeof(stTempData) / sizeof(SDK_DISP_MULT_DATA))) {
            break;
        }
        memcpy(&stTempData[total_node], psttemp, sizeof(SDK_DISP_MULT_DATA));
        pstBuf[total_node] = &stTempData[total_node];
        total_node++;
    }

    //�жϴ�����ϴ�ӡ
    i = 0;

    if (true == is_first_record) {
        is_first_record = false;
        pstRetInfor->siOverNode = 0;
        pstRetInfor->siStartByte = 0;
    }

    while (NULL != pstBuf[i]) {
        //�����ӵ�ǰ�ڵ��ַ������ܴ�ӡ����������ӡֽ�������
        if (true == sdkDispIsStrOverScreen(pstBuf[i], &cur_nod_high)) {
            return SDK_DISP_MULTI_OVER_DISP;
        }
        max_high = (max_high > cur_nod_high ? max_high : cur_nod_high);
        len = sdkDispMultGetChangeLinePlace(pstBuf[i]);

        if (len >= 0) {
            memset(temp, 0, sizeof(temp));
            memcpy(temp, &pstBuf[i]->pasText[len], strlen(pstBuf[i]->pasText) - len);
            memset(&pstBuf[i]->pasText[len], 0, strlen(pstBuf[i]->pasText) - len);

            //��¼��һ�����нڵ����Ϣ
            if (0 == i) {
                pstRetInfor->siStartByte += len;
            }
            else {
                pstRetInfor->siOverNode += i;
                pstRetInfor->siStartByte = len;
            }

            //����һ���ڵ���ΪNULL,��Ϊβ����־
            if (0 == len) {
                psttemp = pstBuf[i];
                pstBuf[i] = NULL;
            }
            else {
                psttemp = pstBuf[i + 1];
                pstBuf[i + 1] = NULL;
            }
            sdkDispFormatFillRam(ucAtr, pstBuf[0], pstBuf[1], pstBuf[2], pstBuf[3], pstBuf[4]);
            //�ָ�û����ʾ�Ľڵ㲿�֣�������֯��ʾ����
            strcpy(pstBuf[i]->pasText, temp);

            if (0 == len) {
                pstBuf[i] = psttemp;
            }
            else {
                pstBuf[i + 1] = psttemp;
            }
            j = 0;

            //����ǰδ��ӡ�ڵ�ǰ��
            for (start_node = i; start_node < (sizeof(stTempData) /
                                               sizeof(SDK_DISP_MULT_DATA)); start_node++, j++) {
                pstBuf[j] = pstBuf[start_node];

                if (NULL == pstBuf[start_node]) {
                    break;
                }
                //���¼�����һ�ֽڵ������
                pstBuf[j]->siX = pstBuf[j]->siX - pixel.siX;
                pstBuf[j]->siX = (pstBuf[j]->siX < 0 ? 0 : pstBuf[j]->siX);
                pstBuf[j]->siY += (max_high + 3); //3Ϊ��϶������
            }

            pstBuf[0]->siX = 0;

            //�ص���ʣ�µĽڵ��ӡ���ݴ����ӡ����
            return sdkDispFormatFillRamEx(ucAtr, pstRetInfor, pstBuf[0], pstBuf[1], pstBuf[2],
                                          pstBuf[3], pstBuf[4]);
        }
        else if (NULL == pstBuf[i + 1]) {
            return sdkDispFormatFillRam(ucAtr, pstBuf[0], pstBuf[1], pstBuf[2], pstBuf[3],
                                        pstBuf[4]);
        }
        i++;
        sdkmSleep(10);
    }

    return SDK_ERR;
}

#if 0 /*Modify by ���ƽ at 2015.03.16  11:52 */
/*****************************************************************************
** Descriptions:	������,����ָ������ɫ������
** Parameters:      SDK_DISP_SRECT stRect
                    siIsFullColor   =falseʱ�����
                    u32 uiColor: �����ɫ��
** Returned value:
** Created By:		tangrongkang  2013.05.06
** Remarks:
*****************************************************************************/
s32 sdkDispDrawRect(const SDK_DISP_SRECT * pstRect, bool siIsFullColor, u32 uiColor)
{
    if( pstRect == NULL )
    {
        return SDK_PARA_ERR;
    }
    return sdkDispRect(pstRect->x0, pstRect->y0, pstRect->x1, pstRect->y1, true, siIsFullColor, uiColor);
}

#endif /* if 0 */

/*****************************************************************************
** Descriptions:	��ָ������ɫ����ָ������
** Parameters:          SDK_DISP_SRECT stRect
                               u16 usColor
** Returned value:
** Created By:		tangrongkang  2013.05.06
** Remarks:
*****************************************************************************/
s32 sdkDispSetRectColor(const SDK_DISP_SRECT *pstRect, const u32 uiColor) {
    if (pstRect == NULL) {
        return SDK_PARA_ERR;
    }
    return sdkDispRect(pstRect->x0, pstRect->y0, pstRect->x1, pstRect->y1, false, true, uiColor);
}

/*****************************************************************************
** Descriptions:    ���þ���ڵ�
** Parameters:          SDK_DISP_SRECT* prect
                               s32 x0
                               s32 y0
                               s32 x1
                               s32 y1
** Returned value:
** Created By:		tangrongkang  2013.04.25
** Remarks:
*****************************************************************************/
s32 sdkDispSetRect(SDK_DISP_SRECT *pRect, const s32 siX0, const s32 siY0, const s32 siX1,
                   const s32 siY1) {
    SDK_DISP_PIXEL pixel;

    pixel.siX = g_row_info.lcd_width;
    pixel.siY = g_row_info.lcd_height;

    if (siX0 < 0 || siX0 > pixel.siX || siX1 < 0 || siX1 > pixel.siX || siY0 < 0 ||
        siY0 > pixel.siY || siY1 < 0 || siY1 > pixel.siY || siX0 > siX1 || siY0 > siY1) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (NULL == pRect) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    pRect->x0 = siX0;
    pRect->y0 = siY0;
    pRect->x1 = siX1;
    pRect->y1 = siY1;
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ָ�������?з��?
** Parameters:          s32 siStartX
                    s32 siStartY
                    s32 siEndX
                    s32 siEndY
** Returned value:
** Created By:		fusuipu  2014.12.10
** Remarks:
*****************************************************************************/
s32 sdkDispNofdispAt(s32 siStartX, s32 siStartY, s32 siEndX, s32 siEndY) {
    strRect temp_rect;

    if (siStartX > siEndX || siStartY > siEndY) {
        return SDK_PARA_ERR;
    }
#if ORIGIN_IS_TOP_LEFT_CORNER /*Modify by ���ƽ at 2014.07.08  15:19 */
    temp_rect.m_x0 = siStartX + g_row_info.show_lcd_left;
    if( temp_rect.m_x0 > g_row_info.show_lcd_right )
    {
        return SDK_PARA_ERR;
    }
    temp_rect.m_y0 = siStartY + g_row_info.show_lcd_top;
    if( temp_rect.m_y0 > g_row_info.show_lcd_bottom)
    {
        return SDK_PARA_ERR;
    }    
    temp_rect.m_x1 = siEndX + g_row_info.show_lcd_left;
    if( temp_rect.m_x1 > g_row_info.show_lcd_right )
    {
        temp_rect.m_x1 = g_row_info.show_lcd_right;
    }    
    temp_rect.m_y1 = siEndY + g_row_info.show_lcd_top;
    if( temp_rect.m_y1 > g_row_info.show_lcd_bottom )
    {
        temp_rect.m_y1 = g_row_info.show_lcd_bottom;
    }
#else
    temp_rect.m_x0 = siStartX;
    if (temp_rect.m_x0 > g_row_info.show_lcd_right) {
        return SDK_PARA_ERR;
    }
    temp_rect.m_y0 = siStartY;
    if (temp_rect.m_y0 > g_row_info.show_lcd_bottom) {
        return SDK_PARA_ERR;
    }
    temp_rect.m_x1 = siEndX;
    if (temp_rect.m_x1 > g_row_info.show_lcd_right) {
        temp_rect.m_x1 = g_row_info.show_lcd_right;
    }
    temp_rect.m_y1 = siEndY;
    if (temp_rect.m_y1 > g_row_info.show_lcd_bottom) {
        temp_rect.m_y1 = g_row_info.show_lcd_bottom;
    }

#endif /* if 0 */
    ddi_lcd_ioctl(DDI_LCD_CTL_INVERT_RECT, (u32)(&temp_rect), 0);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡLCD��ʾ��ÿһ������ʾ�������ĸ����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.01.07
** Remarks:
*****************************************************************************/
s32 Private_sdkMulitGetLcdOneLineLetterMaxNum(bool uiIsAsc) {
    SDK_FONT font;
//    u8 buf[128] = {0};
    s32 max_len;
    s32 x = g_row_info.lcd_width; //, y = g_row_info.lcd_height;

    if (true == uiIsAsc) {
        sdkDispGetSysFont(&font, 0);
    }
    else {
        sdkDispGetSysFont(&font, 1);
    }

    if (SDK_SYS_LANG_FAR == sdkSysGetLang()) {
        x *= 2;
    }
    max_len = ((u32) x / font.len);             //��ͳpos��ߵ����뷨��������Ϣֻռ����
#if 0 /*Modify by ���ƽ at 2014.05.29  11:10 */

    if(max_len > 128)
    {
        max_len = 128;
    }
    Trace("fsppro", "max_len_per_line:%d\r\nwithn:%d, font.len:%d", max_len, x, font.len);
#endif  /* if 0 */
    return max_len;
}

/*****************************************************************************
** Descriptions:	��ȡLCD��ʾ��ÿһ������ʾ�����asc��ĸ����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.01.07
** Remarks:         Ϊ����sdk1.03�Ľӿڼ��ݣ�����Ӵ˽ӿ�
*****************************************************************************/
s32 sdkSysGetMaxLetterPerLine(void) {
    return Private_sdkMulitGetLcdOneLineLetterMaxNum(true);
}

/*****************************************************************************
** Descriptions:	��ȡϵͳ��ɫֵ
** Parameters:          SDK_COLOR_ID eColorId
** Returned value:
** Created By:		fusuipu  2014.05.06
** Remarks:
*****************************************************************************/
s32 sdkSysGetColorValue(SDK_COLOR_ID eColorId) {
    u32 color_value[2][2] = {{SDK_BLACK_COLOR_ID, 1},
                             {SDK_WHITE_COLOR_ID, 0}};
    u8 i = 0;

    for (i = 0; i < 2; i++) {
        if (eColorId == color_value[i][0]) {
            return color_value[i][1];
        }
    }

    return SDK_ERR;
}

