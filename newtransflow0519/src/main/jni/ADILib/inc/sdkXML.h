#ifndef __SDK_XML_H__
#define __SDK_XML_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define SDK_XML_VER   "1.0beta3"  //
#define UTF8_BLOCK 256

//�ڵ�����
typedef enum {
    SDK_XML_ELEMENT = 0,                //xmlԪ��ֵ
    SDK_XML_COMMENT               //xmlע��
} SDK_XML_NODETYPE;               //���ӽڵ�����

typedef enum {
    SDK_XML_ANSI = 0,                                   //ascii����
    SDK_XML_UTF_8,                                   //utf-8����
    SDK_XML_UTF_8_NO_MARK,                                   //utf-8 no nark����
    SDK_XML_UTF_16,                                   //utf-16����
    SDK_XML_UTF_16_BIG_ENDIAN                                   //UTF_16_BIG_ENDIAN����

} SDK_XML_ENCODE;                  //��������

typedef enum {
    SDK_UTF8_BOM = 0,                //utf-8 ��bom
    SDK_UTF8_NBOM                   //utf-8 ��bom
} SDK_UTF8_TYPE;               //utf-8�ļ�����


typedef void *SDK_XML_HAND;
typedef void *SDK_XML_NODE;
typedef void *SDK_XML_ATTRIBUTE;

extern int sdkXmlCreatFile(char const *pasFile, SDK_XML_ENCODE EnCode);

extern SDK_XML_HAND sdkXmlLoadFromFile(char const *pasFile);

extern SDK_XML_NODE sdkXmlGetRootNode(SDK_XML_HAND hDoc);

extern SDK_XML_NODE sdkXmlFindChildInNode(const SDK_XML_NODE hNode, const char *pName, int index);

extern int sdkXmlReadNodeValue(const SDK_XML_NODE hNode, char *pasDest);

extern int sdkXmlGetChildCount(const SDK_XML_NODE hNode, const char *pName);

extern void sdkXmlRemoveChild(const SDK_XML_NODE hParent, SDK_XML_NODE Node);

extern SDK_XML_NODE sdkXmlAddChildInNode(const SDK_XML_NODE hNode, const char *pName,
                                         SDK_XML_NODETYPE type);

extern SDK_XML_NODE sdkXmlSetChildValue(const SDK_XML_NODE hNode, const char *pValue);

extern SDK_XML_ATTRIBUTE sdkXmlAddAttritube(const SDK_XML_NODE hNode, const char *pName);

extern SDK_XML_ATTRIBUTE sdkXmlSetAttritubeValue(const SDK_XML_ATTRIBUTE attribute,
                                                 const char *pValue);

extern SDK_XML_ATTRIBUTE sdkXmlFindAttritube(const SDK_XML_NODE hNode, const char *pName);

extern int sdkXmlReadAttributeValue(const SDK_XML_ATTRIBUTE attribute, char *pasDest);

extern void sdkXmlRemoveAttribute(const SDK_XML_NODE hNode, SDK_XML_ATTRIBUTE attribute);

extern bool sdkXmlSave(const char *pasFile, const SDK_XML_NODE hParent, SDK_XML_ENCODE EnCode);

extern bool sdkXmlClose(const SDK_XML_HAND hDoc);

extern int sdkFileGb2312ToUtf8(char const *pasGbfile, char const *pasUtf8file, SDK_UTF8_TYPE type);

extern int sdkFileUtf8ToGb2312(char const *pasUtf8file, char const *pasGbfile);

extern int sdkXmlGetVersion(char *pasVer);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
