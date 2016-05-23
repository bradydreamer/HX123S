#ifndef _DLLEMVAVLOPERATE_H_
#define _DLLEMVAVLOPERATE_H_


extern int compare_EMVtagitems(const void *pa, const void *pb, void *param);

extern int dllemvb_avl_create(void);

extern void dllemvb_avl_destory(void);
extern int dllemvb_avl_find(unsigned char *tag,unsigned char taglen,void *param);
extern int dllemvb_avl_read(unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short *tagdatalen,void *param);
extern int dllemvb_avl_insert(EMVTAGCVLITEM *storeditem,unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short tagdatalen,void *param);
//extern int dllemvb_avl_insertadd(unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short tagdatalen,void *param);
extern void dllemv_avl_settagtype(unsigned int tagtype);

extern unsigned int dllemv_avl_gettagtype(void);
extern int dllemvb_avl_set(unsigned char *tag,unsigned char tagdata);
extern int dllemvb_avl_createsettagvalue(unsigned char *tag,unsigned char *tagdata,unsigned short tagdatalen);
extern EMVTAGCVLITEM *dllemvb_avl_gettagitempointer(unsigned char *tag);
extern unsigned char dllemvb_avl_gettagvalue_spec(unsigned char *tag,unsigned char *tagdata,unsigned short strpos,unsigned short len);
extern void dllemvb_avl_setvalue_and(unsigned char *tag,unsigned short  position,unsigned char anddata);
extern void dllemvb_avl_setvalue_or(unsigned char *tag,unsigned short  position,unsigned char ordata);
extern unsigned char dllemvb_avl_checkiftagexist(unsigned char *tag);
extern EMVTAGCVLITEM *dllemvb_avl_gettagitempointerandexiststatus(unsigned char *tag,unsigned char *tagexistflag);
extern void dllemvb_avl_delete(unsigned char *tag);  // 这个要仔细测试一下

extern void dllemvb_avl_printtagvalue(unsigned char *title,unsigned char *tag,unsigned short strpos,unsigned short len);
extern void dllemvb_avl_printtagallvalue(unsigned char *title,unsigned char *tag);
extern unsigned char dllemvb_avl_gettagvalue_all(unsigned char *tag,unsigned char *tagdata,unsigned short *len);
//extern void print_whole_tree (void);
extern void dllemvb_avl_readtreetagdatatospecfile(unsigned char *tag, int *taglen);

extern unsigned char dllemvb_avl_gettagvalue(unsigned char *tag);

extern unsigned char dllemvb_avl_checktreestatus(void);

#endif





