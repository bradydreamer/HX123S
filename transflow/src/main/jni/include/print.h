/**
* 初始化打印
* 初始化打印的设置，获取打印状态等
*
*/
s32 initPrint();

/**
* 设置中文字体大小
*(s,n,l,sn,sl,nl)
*s（小字体）：一行打印24个(16x16)；
*n（标准字体）：一行打印16个(24x24)；
*l（大字体）：一行打印12个；(32x32)
*sn：小字体宽度，标准字体高度；(16x24)
*sl：小字体宽度，大字体高度；(16x32)
*nl：标准字体宽度，大字体高度；(24x32)
*默认为 n
*/
s32 setChnFontSize(char* fontSize);

/**
* 设置ascll字体大小
*s,n,l,sn,sl,nl)
*s（小字体）：一行打印48个(8x16)；
*n（标准字体）：一行打印32个(12x24)；
*l（大字体）：一行打印24个；(16x32)
*sn：小字体宽度，标准字体高度(8x24)
*sl：小字体宽度，大字体高度(8x32)
*nl：标准字体宽度，大字体高度(12x32)
*默认为 n
*/
s32 setAscFontSize(char* fontSize);

/**
* 设置打印灰度
*灰度/浓度，文字粗细（对于只支持加粗的
*打印机进行如下操作：1~5为标准，6~10为加粗）
*[1，10]
*默认5  
*/
s32 setPrintGray(u32 grayLevel);

/**                                          
* 设置行间距                            
*
*@param lineSpace
*行间距大小
*[0, 60]
*默认6                                    
*/
s32 setPrintLineSpace(u32 lineSpace);

/**                                          
* 设置条码配置   
*@param width                      
*单条条码宽度（从效果来看，建议宽度>=2）
*[1，8],默认2                                  
*@param height
*期望高度               
*[1，320]且必须是8的倍数,默认64 
*                                         
*/
s32 setBarCodeConfig(u32 width, u32 height);

/**                                          
* 设置二维码配置                            
*@param width
*不超过打印边界(384)
*@param height
*(int)
*默认100
*@param correctionLevel 纠错级别
*[0,3]
*一般为2                               
*/
s32 setQrCodeConfig(u32 width, u32 height, u32 correctionLevel);


/**                                          
* 打印文本                        
* @param dispSite
* 显示位置
* (l,c,r)l:居左c:居中r:居右   
* @param text
* 待打印的路径和文件名                
*/
s32 writePrintText(char dispSite, char* text);

/**                                          
* 打印图片                        
* 用于打印logo、电子签名、条码和二维码等
* @param dispSite
* 显示位置
* (l,c,r)l:居左c:居中r:居右   
* @param imagePath
* 待打印的路径和文件名                
*/
s32 writePrintImage(char dispSite, char* imagePath);

/**                                          
* 暂停时间                        
* 用于多联打印
* @param times
*（单位秒）                            
*/
s32 writePauseTime(u32 times);

/**
* 开始打印
* 打印出来缓存的内容
*
*/
s32 startPrint();