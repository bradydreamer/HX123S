/*
EMV���ݽṹ����
AIP 2�ֽ� Ӧ�ý�������
	�ֽ� 1:
		bit 8: 1 = RFU
		bit 7: 1 = ֧��SDA
		bit 6: 1 = ֧��DDA
		bit 5: 1 = ֧�ֳֿ�����֤
		bit 4: 1 = ִ���ն˷��չ���
		bit 3: 1 = ֧�ַ�������֤
		bit 2: RFU (0)
		bit 1: 1 = ֧��CDA
		�ֽ�2: RFU ("00")
			
AFL 4�ֽ� Ӧ���ļ���λ��
    �ֽ�1:���ļ���ʶ��. SFCXXXXX
    �ֽ�2:�ļ���Ҫ��ȡ�ĵ�һ����¼�ļ�¼��
    �ֽ�3:�ļ���Ҫ��ȡ������һ����¼�ļ�¼��
    �ֽ�4:�ӵ�һ����¼��ʼ�������ѻ�������֤��������¼��
    	
AFLNUM 1�ֽ�.��¼����

TermInfo.TVR[5]	//�ն���֤���
		b8  b7  b6  b5  b4  b3  b2  b1
	TVR[0]:		
		1	X	x	x	x	x	x	x	δ�����ѻ�������֤
		x	1	x	x	x	x	x	x	�ѻ���̬������֤ʧ��
		x	X	1	x	x	x	x	x	IC������ȱʧ
		x	X	x	1	x	x	X	x	��Ƭ�������ն��쳣�ļ���
		x	X	x	x	1	x	X	x	�ѻ���̬������֤ʧ��
		x	X	x	x	x	1	X	x	���϶�̬������֤/Ӧ����������ʧ��
		x	X	x	x	x	x	0	x	RFU
		x	X	x	x	x	x	X	0	RFU
	TVR[1]:
		1	X	x	x	x	x	X	x	IC�����ն�Ӧ�ð汾��һ��
		x	1	x	x	x	x	X	x	Ӧ���ѹ���
		x	X	1	x	x	x	X	x	Ӧ����δ��Ч
		x	X	x	1	x	x	X	x	��Ƭ������������ķ���
		x	X	x	x	1	x	X	x	�¿�
		x	X	x	x	x	0	X	x	RFU
		x	X	x	x	x	x	0	x	RFU
		x	X	x	x	x	x	X	0	RFU
	TVR[2]:
		1	X	x	x	x	x	x	x	�ֿ�����֤ʧ��
		x	1	x	x	x	x	x	x	δ֪��CVM
		x	X	1	x	x	x	x	X	PIN���Դ�������
		x	X	x	1	x	x	x	X	Ҫ������PIN����������̲����ڻ���������
		x	X	x	x	1	x	x	X	Ҫ������PIN��������̴��ڣ���δ����PIN
		x	X	x	x	x	1	x	X	��������PIN
		x	X	x	x	x	x	0	X	RFU
		x	X	x	x	x	x	x	0	RFU
	TVR[3]:
		1	X	x	X	x	X	x	X	���׳�������޶�
		x	1	x	X	x	X	x	X	���������ѻ���������
		x	X	1	X	x	X	x	X	���������ѻ���������
		x	X	x	1	x	X	x	X	���ױ����ѡ����������
		x	X	x	X	1	X	x	X	�̻�Ҫ����������
		x	X	x	X	x	0	x	X	RFU
		x	X	x	X	x	X	0	X	RFU
		x	X	x	X	x	X	x	0	RFU
	TVR[4]:
		1	X	x	x	x	X	x	X	ʹ��ȱʡTDOL
		x	1	x	x	x	X	x	X	��������֤ʧ��
		x	X	1	x	x	X	x	X	���һ��GENERATE AC����֮ǰ�ű�����ʧ��
		x	X	x	1	x	X	x	X	���һ��GENERATE AC����֮��ű�����ʧ��
		x	X	x	x	0	X	x	X	RFU
		x	X	x	x	x	0	x	X	RFU
		x	X	x	x	x	X	0	X	RFU
		x	X	x	x	x	X	x	0	RFU
		
TermInfo.TSI[2]:����״̬��Ϣ(TSI)
	TSI[0]:
		1	X	x	x	X	x	X	x	�ѻ�������֤�ѽ���
		x	1	x	x	X	x	X	x	�ֿ�����֤�ѽ���
		x	X	1	x	X	x	X	x	��Ƭ���չ����ѽ���
		x	X	x	1	X	x	X	x	��������֤�ѽ���
		x	X	x	x	1	x	X	x	�ն˷��չ����ѽ���
		x	X	x	x	X	1	X	x	�ű������ѽ���
		x	X	x	x	X	x	0	x	RFU
		x	X	x	x	x	x	X	0	RFU
	TSI[1]:
		0	X	x	x	x	X	X	x	RFU
		x	0	x	x	x	X	X	x	RFU
		x	X	0	x	x	X	X	x	RFU
		x	X	x	0	x	X	X	x	RFU
		x	X	x	x	0	X	X	x	RFU
		x	X	x	x	x	0	X	x	RFU
		x	X	x	x	x	X	0	x	RFU
		x	X	x	x	x	x	X	0	RFU
		
CVM�б����ݸ�ʽ��
	�ֽ�1-4�����X(������)
	�ֽ�5-8�����Y(������)
	�ֽ�9:(CVM CODE)
		bit 8: 0=ֻ�з��ϴ˹淶��ȡֵ(�����Ϊ1,˵�����Զ����ֵ)
		bit 7: 1=�����CVMʧ��,Ӧ�ú�����
			   0=�����CVMʧ��,��ֿ�����֤ʧ��
		bit 6-1:
		000000 = CVMʧ�ܴ���
		000001 = ��Ƭִ������PIN�˶�
		000010 = ��������PIN��֤
		000011 = ��Ƭִ������PIN�˶�+ǩ��(ֽ��)
		000100 = EMV����//V4.1�޸ĳ��Ѽ����ܵ�PIN�˶�
		000101 = EMV����//V4.1�޸ĳ��ѻ����ܵ�PIN�˶�+ǩ��
		011110 = ǩ��(ֽ��)
		011111 = ����CVM
		000110-011101 = �����������֧��ϵͳ
		100000-101111 = ���������Զ�����֧��ϵͳ
		110000-111110 = ������������
		111111 = RFU
		PBOC����:
		100000 = �ֿ���֤����ʾ
	�ֽ�10(CVM Condition Code):
		00-����
		01-�����ATM�ֽ���
		02-�������ATM�ֽ������ֵ���ֽ���ֽ���
		03-����ն�֧�����CVM
		04-������˹�ֵ���ֽ���
		05-����Ƿ��ֽ���
		06-������׻��ҵ���Ӧ�û����벢��С��Xֵ
		07-������׻��ҵ���Ӧ�û����벢�Ҵ���Xֵ
		08-������׻��ҵ���Ӧ�û����벢��С��Yֵ
		09-������׻��ҵ���Ӧ�û����벢�Ҵ���Xֵ
		0A-7F:RFU
		80-FF:RFU ����������֧��ϵͳ
	��һ��CVM������2��CVM���CVM�����ֽڱ�ʾ		
	
TermInfo.TermCapab[3]//	�ն����ܱ� ����Ĭ��Ϊ \xE0\xF8\xC8
	TermInfo.TermCapab[0]:�ֽ�1����Ƭ������������
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X �ֹ���������
		X  1  X  X  X  X  X	 X ����
		X  X  1  X  X  X  X	 X �Ӵ�ʽIC��
		X  X  X  0  X  X  X	 X RFU
		X  X  X  X  0  X  X	 X RFU
		X  X  X  X  X  0  X	 X RFU
		X  X  X  X  X  X  0	 X RFU
		X  X  X  X  X  X  X	 0 RFU
	TermInfo.TermCapab[1]:�ֽ�2��CVM����
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X IC������PIN��֤
		X  1  X  X  X  X  X	 X ����PIN������֤
		X  X  1  X  X  X  X	 X ǩ��(ֽ)
		X  X  X  1  X  X  X	 X �ѻ�����PIN
		X  X  X  X  1  X  X	 X ����CVM
		X  X  X  X  X  0  X	 X RFU
		X  X  X  X  X  X  0	 X RFU
		X  X  X  X  X  X  X	 1 �ֿ���֤����֤
	TermInfo.TermCapab[2]:�ֽ�3����ȫ����
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X ��̬������֤SDA
		X  1  X  X  X  X  X	 X ��̬������֤DDA
		X  X  1  X  X  X  X	 X �̿�
		X  X  X  1  X  X  X	 X ����������֤/Ӧ����������CDA
		X  X  X  X  0  X  X	 X RFU
		X  X  X  X  X  0  X	 X RFU
		X  X  X  X  X  X  0	 X RFU
		X  X  X  X  X  X  X	 0 RFU
	
TermInfo.TermAddCapab[5]//	�����ն����ܱ�		
	//Ĭ�ϲ�����xFF\x00\xF0\xB0\x01
	TermInfo.TermAddCapab[4]:�ֽ�1��������������
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X �ֽ�
		X  1  X  X  X  X  X	 X ��Ʒ
		X  X  1  X  X  X  X	 X ����
		X  X  X  1  X  X  X	 X ����
		X  X  X  X  1  X  X	 X ��ѯ
		X  X  X  X  X  1  X	 X ת��
		X  X  X  X  X  X  1	 X ����
		X  X  X  X  X  X  X	 1 ����
	TermInfo.TermAddCapab[3]:�ֽ�2��������������
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X ����
		X  0  X  X  X  X  X	 X RFU
		X  X  0  X  X  X  X	 X RFU
		X  X  X  0  X  X  X	 X RFU
		X  X  X  X  0  X  X	 X RFU
		X  X  X  X  X  0  X	 X RFU
		X  X  X  X  X  X  0	 X RFU
		X  X  X  X  X  X  X	 0 RFU
	TermInfo.TermAddCapab[2]:�ֽ�3���ն�������������
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X ���ּ�
		X  1  X  X  X  X  X	 X ��ĸ�������ַ���
		X  X  1  X  X  X  X	 X �����
		X  X  X  1  X  X  X	 X ���ܼ�
		X  X  X  X  0  X  X	 X RFU
		X  X  X  X  X  0  X	 X RFU
		X  X  X  X  X  X  0	 X RFU
		X  X  X  X  X  X  X	 0 RFU
	TermInfo.TermAddCapab[1]:�ֽ�4���ն������������
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X ��ӡ��������Ա
		X  1  X  X  X  X  X	 X ��ӡ�����ֿ���
		X  X  1  X  X  X  X	 X ��ʾ��������Ա
		X  X  X  1  X  X  X	 X ��ʾ�����ֿ���
		X  X  X  X  0  X  X	 X RFU
		X  X  X  X  X  0  X	 X RFU
		X  X  X  X  X  X  1	 X �����10
		X  X  X  X  X  X  X	 1 �����9
	TermInfo.TermAddCapab[0]:�ֽ�5���ն������������
		B8 B7 B6 B5 B4 B3 B2 B1 
		1  X  X  X  X  X  X	 X �����8
		X  1  X  X  X  X  X	 X �����7
		X  X  1  X  X  X  X	 X �����6
		X  X  X  1  X  X  X	 X �����5
		X  X  X  X  1  X  X	 X �����4
		X  X  X  X  X  1  X	 X �����3
		X  X  X  X  X  X  1	 X �����2
		X  X  X  X  X  X  X	 1 �����1(���ı���)
		
TermInfo.TermType //�ն����� ϵͳ����Ϊ22
	������					�������Ʒ�
					���ڻ���	�̻�	�ֿ���
�з���Ա�ģ�
		��������		  11         21       --
	�������������ѻ�	  12		 22		  --
		�����ѻ�		  13         23       --

	�����ģ�
		��������		  14         24       34
	�������������ѻ�	  15		 25		  35
		�����ѻ�		  16         26       36
��ע���ն�����Ϊ'14','15','16'�Ҿ����ֽ�֧������(�����ն����ܵ��ֽ�1�����ֽ�λ=��1��)
	���ն˱���Ϊ��ATM���������͵��ն˶���������ATM]
	
PDOL:tag 9F38
		ָ���ڴ���ѡ���������ն����뿨Ƭ������,�����ն����ݶ���(��ǩ�ͳ���)
	
Readrecord���������ѻ�������֤:
	1.����SFI��1��10���ļ�,��¼��TAG(70)�ͼ�¼���Ȳ������ѻ�������֤����,������Ӧ������
	�е���������(SW1,SW2����)�������ѻ�������֤
	2.����SFI��11��30���ļ�,��¼��TAG(70)�ͼ�¼���������ѻ�������֤����,�����Ӧ������
	�е���������(SW1,SW2����)�������ѻ�������֤
	3.���TAG����'70',����Ϊ�ѻ�������֤�Ѿ�ִ�в�ʧ��,�ն˱�������TSI��"�ѻ�������֤��ִ��λ"
	�Լ�TVR��Ӧ��"�ѻ���̬������֤ʧ��λ","�ѻ���̬������֤ʧ��λ"��"CDAʧ��λ"
����cardinfo���ݺ����Ƿ����Щ���������:(ICCDataTable_Stored.flagm=1)
	{"AppExpireDate",0,"\x5F\x24",255,3,1,47,1,0},	//Ӧ��ʧЧ����
	{"AFL",0,"\x94\x00",0,252,0,51,1,0},			//AFL�ļ�
	{"PAN",0,"\x5A\x00",0,10,2,357,1,0},			//���ʺ�
	{"CDOL1",0,"\x8C\x00",0,252,0,390,1,0},			//CDOL1
	{"CDOL2",0,"\x8D\x00",0,252,0,643,1,0},			//CDOL2 20
	

typedef struct {
	BYTE ASI;  //Application Selection Indicator.0-needn't match exactly(partial match up to the length);1-match exactly
	//Ӧ��ѡ��ָʾ��. = 1 ָʾ�ն��ϵ�AID�뿨Ƭ�ϵ�AID�Ƿ���ȫƥ��,(���Ⱥ����ݶ�����һ��)
					  = 0.����ƥ��,��ƬAID��ǰ�沿����AID��ͬ,���ȿ��Ը���
	BYTE AIDLen;	//Ӧ�ñ�ʶ����
	BYTE AID[16];//5-16	//Ӧ�ñ�ʶ������. AID = RID (Ӧ���ṩ�̱�ʶ)+PIX(ר��Ӧ����չ)
	BYTE bLocalName;//If display app list using local language considerless of info in card.0-use card info;1-use local language.
	//�Ƿ����ն˵�Ӧ������
		= 1���ն˵�Ӧ������
		= 0�ÿ���Ӧ������
	BYTE AppLocalNameLen;	//�ն�Ӧ�����Ƶĳ���
	BYTE AppLocalName[16];	//�ն�Ӧ�����Ƶ�����
}TERMAPP;

typedef struct{
	//BYTE* Name;
	//BYTE  Source;//0-ICC;1-Terminal
	BYTE  Tag[3];//Second byte '00' means only first one byte tag.add third byte to make int variable oven aligned.
	BYTE  Len1;  //Len1>Len2: fix len with Len2;Len1<Len2 and Len2!=255: Len1<= len <=Len2; Len2=255: len=Len1+Len2
	BYTE  Len2;
	BYTE  bAmt;//0- non numeric;1-numeric;2-compact numric
	unsigned int address;//var address 
	BYTE  flagM;//'1'--bit0: mandatory;bit1: SDA mandatory; bit2: DDA mandatory;
	BYTE  bExist;//0-not presented,1-have been existed.	
}DATAELEMENT;	//��ǩ�����ļ�


SDA:��̬������֤�õ������ݶ���
	:
	��ǩ		����		ֵ							��ʽ
	 -			  5 		ע���Ӧ���ṩ�̱�ʶ      	b
	 '8F'		  1			��֤���Ĺ�Կ����			b
	 ��90��		  Nca		�����й�Կ֤��				b
	 '92'		  Ni-Nca+36 �����й�Կ������(�����)	b
	 '9F32'		  1��3		�����й�Կָ��				b
	 '93'		  Ni		ǩ���ľ�̬Ӧ������			b
	 -			  �䳤		ָ���ľ�̬����				b
��֤ʱ���������ЩTAG����.������Ϊ�ѻ���֤��������ʧ��.
	��̬������֤����:
		1.�ж���֤��Ҫ�������Ƿ����,������TAG��exist
		2.���ն˻ָ���֤���Ĺ�Կ.
		����:����RID�͹�Կ�������ն˵Ĺ�Կ��ȡ����һ��,���ûƥ�����Ϊ
		�ѻ���֤��������ʧ��.
		3.���ն˻ָ������й�Կ.
		����:a.��鷢���й�Կ֤�����֤���Ĺ�Կģ�����Ƿ���ͬ.
			 b.����֤���Ĺ�Կ�Է���֤��ָ�,ȡ��֤���������
			 c.���ָ������������Ƿ���ȷ,�ָ�������β,����ͷ,֤���ʽ,����
			   �б�ʶ,֤����Ч��,�ͷ������㷨��ʶ
			 d.���㾲̬��֤���ݵĹ�ϣ���,����ָ��Ĺ�ϣ����Ƚ��Ƿ�һ��.
			 e.���ָ������еķ����й�Կģ�������ಿ��(�����)��ϳɷ����й�Կ
		4.ǩ����̬Ӧ�����ݵ���֤
		�ն��÷����й�Կ��ǩ���ľ�̬Ӧ�����ݻָ��������й�ϣֵ�Ƚ�,��֤ǩ���Ƿ���ȷ
		����:a.�ն˼��ǩ����̬Ӧ�������뷢���й�Կģ�����Ƿ���ͬ
			 b.�ն����÷����й�Կ��ǩ���ľ�̬Ӧ�����ݽ��лָ�
			 c.���ָ����ݵĸ����Ƿ���ȷ���ָ�������β��ͷ�����ݸ�ʽ
			 d.�ն��������Ӵ�ǩ����̬Ӧ�����ݻָ��������ݡ���AFLָ���������ѻ�������֤��	
			   ��¼���ݡ��Լ��ɾ�̬������֤��ǩ�б��ʾ�����ݣ����������ϣ���.
			 e.������Ĺ�ϣ������ǩ����̬Ӧ�����ݻָ����Ĺ�ϣ�����Ƚϣ�һ����SDA�ɹ�  
�������ݸ�ʽ��IC����ȫ�淶.
			 
			 
//�ӷ����й�Կ֤��ָ����ݵĸ�ʽ
typedef struct{
	BYTE DataHead;		//'6A'�ָ�����ͷ
	BYTE CertFormat;	//'02'֤���ʽ
	BYTE IssuID[4];		//���ʺ��������3-8����(�Ҳ�F)�����б�ʶ
	BYTE ExpireDate[2];	//֤��ʧЧ����MMYY
	BYTE CertSerial[3];	//֤�����к�
	BYTE HashInd;		//��ϣ�㷨��ʶ
	BYTE IPKAlgoInd;	//�����й�Կ�㷨��ʶ
	BYTE IPKLen;		//�����й�Կ���ֽڳ���
	BYTE IPKExpLen;		//�����й�Կָ�����ֽڳ���
	BYTE IPKLeft[212];	//�����й�Կ��Կ����ߵ��ֽ�NCA-36�����Ҳ�BB
	BYTE HashResult[20];//��ϣ���
	BYTE DataTrail;		//'BC'�ָ����ݽ�β	
}IPK_RECOVER;

//��IC����Կ֤��ָ����ݵĸ�ʽ
typedef struct{
	u8 DataHead;//'6A'		//�ָ�����ͷ
	u8 CertFormat;//'04'	//֤���ʽ
	u8 AppPAN[10];			//���ʺ�(���ұ߲���'F')
	u8 ExpireDate[2];		//MMYY֤��ʧЧ����
	u8 CertSerial[3];		//֤�����к�
	u8 HashInd;				//��ϣ�㷨��ʶ
	u8 ICCPKAlgoInd;		//IC����Կ�㷨��ʶ
	u8 ICCPKLen;			//IC����Կ����
	u8 ICCPKExpLen;			//IC����Կָ������
	u8 ICCPKLeft[206];//NI-42	//IC����Կ��IC����Կ��������ֽ�
	u8 HashResult[20];		//��ϣ���
	u8 DataTrail;			//'BC'�ָ����ݽ�β	
}ICCPK_RECOVER;

//��ǩ���ľ�̬Ӧ�����ݻָ����ݵĸ�ʽ
typedef struct {	
	u8 DataHead;//'6A'			//��ʽͷ'6A'
	u8 DataFormat;//'03'		//ǩ�����ݸ�ʽ'03'
	u8 HashInd;					//��ϣ�㷨����'01'
	u8 DataAuthCode[2];			//������֤����"������9F45TAG��"
	u8 PadPattern[222];//NI-26	//����ֽ�
	u8 HashResult[20];			//���
	u8 DataTrail;//'BC'			//'BC'�ָ����ݽ�β
}SIGN_STAT_APPDATA_RECOVER;	

//��ǩ����̬Ӧ�����ݻָ������ݸ�ʽ
typedef struct {
	u8 DataHead;//'6A'
	u8 DataFormat;//'05'
	u8 HashInd;//'01'
	u8 ICCDynDataLen;
	u8 ICCDynData[223];//LDD	<NIC-25
	u8 PadPattern[223];//NIC-LDD-25,padded with 'BB'
	u8 HashResult[20];		
	u8 DataTrail;//'BC'
}SIGN_DYN_APPDATA_RECOVER;
*/
/*****************************************************************
EMV �ײ�ICCard�������ӿں���   2008/03/31  ��Т������
*****************************************************************/
#ifndef _EMV_H_
#define _EMV_H_

//#include <stddef.h>

//#include <cutils/log.h>
#include "dllemvmaths.h" 

//#define PAYWAVECVN17SUP    (1<<2)  

#define ARM_ADS_GCC
//#undef ARM_ADS_GCC

#define EMV_LEVEL2TEST    		//EMVLEVEL2送检程序编译开关
//#undef  EMV_LEVEL2TEST			

#ifndef EMVB_DEBUG
//#define EMVB_DEBUG
#endif

//extern void uart_printf(char *fmt,...);
//extern int memcmp(const void *buffer1,const void *buffer2,int count);
extern void emv_uart_printf(char *fmt,...);

#ifdef ARM_ADS_GCC
//#define printf ALOGD
#define ATTRIBUTE
//#define uart_printf ALOGD
#else
#define ATTRIBUTE __attribute__((packed))
#define printf uart_printf
#endif


#ifndef TEMPTEST
//#define TEMPTEST
#endif

//#define HX_DEBUG
#ifndef SXL_DEBUG
//#define SXL_DEBUG
#endif
//#define TESTREADCARDTIME
#ifndef SXL_DEBUG1
//#define SXL_DEBUG1
#endif

#define EMV_ENGLISH                                 49
#define EMV_CHINESE                                 50

#define MAG_LAST_FAIL_IC        0x92
#define MAG_NO_IC                    0x90
#define MAG_LAST_SUCCESS_IC        0x91

#define CARDTYPE_EMV 0x00
#define CARD_QPBOC   0x01
#define CARD_UPCARD  0x03
#define CARD_PAYPASS 0x08
#define CARD_PAYWAVE 0x09


#define IC_SUCCESS                0x05
#define IC_FAIL                    0x95
#define qPBOC_USED                0x07        //20110518 modify based on qPBOC spceification
#define MSD_USED                0x91        //20110514 FOR MSD
#define CLPBOC_USED             0x98


#define CONTACTLESSFAIL         0xF0        //�ǽӿ�����ʧ��


#define MAXPAYPASSCCCTIMES 5

/*******************************************************
        2008-03-31 ��Т��
�޸���Ϣ:

���巵�ص���������        2008-03-31 ��Т��
�޸���Ϣ:emv��������

********************************************************/
#define EMVTRANSINIT               9 //���ײ�����ʼ��
#define EMVAPPSELECT              10//����Ӧ���б�
#define EMVCHOOSEAPP               11//Ӧ��ѡ��
#define EMVINITAPP                12//Ӧ�ó�ʼ��
#define EMVREADRECORD             13//��Ӧ�ü�¼
#define EMVDATAAURH                 14//��̬������֤
#define EMVPROCESSRESTRICT         15//��������
#define EMVCARDHOLDERVERF          16//�ֿ�����֤
#define EMVTERMRISKMANAGE          17//�ն˷��չ���
#define EMVTERMIANALYS             18//�ն���Ϊ����
#define EMVTRANSPROCESS            19//���״���
#define EMVTRANSANALY               20
#define EMVDEALBAGDATA              21
#define EMVSENDBAG                  22
#define EMVFINALSELECT               23  //Ӧ������ѡ��
#define EMVUSERSELECTAPP             24
#define EMVRESELECTAPP              25  //����ѡ��Ӧ��
#define EMVCOMPLETETRANS            26  //EMV��ɽ���
#define EMVPRECOMPLETETRANS         27
#define EMV_TRANSCOMPLETEPROMAT    29   //���������ʾ
/*******************************************************
���巵�ص���������        2009-07-03 huxiang
�޸���Ϣ:qPBOC��������

********************************************************/
#define qPBOCPRETRANS               30 //Ԥ���״���
#define qPBOCDETECTCARD             31 //��⿨Ƭ
#define qPBOCSELECTAPP              32 //ѡ��Ӧ��
#define ECREADECDATA                33 //��ȡECӦ������
#define ECGETECBALANCE              34 //���׽�����õ������ֽ����
#define INPUTECPIN                  35 //�����ֽ���ֵ����Ҫ����PIN

#define EMVREQUESTAMOUNT                50
#define EMVREQUESTOFFPIN                51
#define EMVREQUESTPIN                    52
#define EMVREQUESTSENDBAG                53
#define EMVREQUESTECPIN                    54

#define PAYPASS_CHECKAVN                70
#define PAYPASS_COMPUTECRYPTOCHECKSUM   71
#define PAYPASS_ODAMETHODSELECT         72

#define PAYWAVE_DISPLAY                 73
#define PAYWAVEDETECTCARD               74
#define PAYPASSONLINEPIN                75
#define PAYWAVE_MULTICARD               76
#define PAYWAVE_MULTICARD_DETECTEDCARD  77

#define PAYPASS_PREGETACBALANCEREAD 78
#define PAYPASS_MSCHECKCVM          79
#define PAYPASS_DEALDATAAUTHAFTERGAC 80
#define PAYPASS_GETACBALANCEREAD     81


/*******************************************************
���巵�ص���������        2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define APPSEL_PSEFOUND        2
#define APPSEL_PSENOTFOUND  3
#define RESELECT            4
#define OK_AddOfflineAdvice 5

#define ERR_EMV_NoBatchData     21
#define ERR_EMV_ScriptLen          22
#define ERR_EMV_ScriptReturn    23
#define ERR_EMV_ScriptFormat    24
#define ERR_EMV_TermDataMissing 25
#define ERR_EMV_ComOpen         26
#define ERR_EMV_ComSend         27
#define ERR_EMV_ComRecv         28
#define ERR_EMV_MsgData         29
#define ERR_EMV_IssuerAuthen    30
#define ERR_EMV_FileSys         31
#define ERR_EMV_SendAuthRQ      32
#define ERR_EMV_SendReconci     33

#define ERR_EMV_IccDataFormat   34
#define ERR_EMV_IccCommand      35
#define ERR_EMV_IccReturn          36
#define ERR_EMV_NoAppSel        37
#define ERR_EMV_IccDataMissing  38
#define ERR_EMV_IccDataRedund   39
#define ERR_EMV_AppReselect     40
#define ERR_EMV_GetChallenge    41
#define ERR_EMV_CVRFail         42
#define ERR_EMV_CVRNoSupport    43
#define ERR_EMV_InterAuth       44
#define ERR_EMV_InputCancel     45
#define ERR_EMV_PSENoMatchApp   46
#define ERR_EMV_ListNoMatchApp  47
#define ERR_EMV_NotAccept           48
#define ERR_EMV_EmvDataFormat   49
#define ERR_EMV_CancelTrans     50
#define ERR_EMV_CardBlock       51 //added on Oct 10,2003
#define ERR_EMV_CDADifferCID    52 //added on May 25,2004


#define ERR_CANCEL                53
#define ERR_APP_TIMEOUT        54
#define FAIL            55
#define ERR_NEEDMAGCARD 56    //GACʱ��ص���6985
#define ERR_BLOCKORFORMATERR 57
#define EMV_PARSESUC          58     //sxl �����ɹ�
#define EMV_APPBLOCK          59
#define EMV_EXTRACTMATCH      60
#define EMV_PARTICALMATCH     61
#define EMV_NOTMATCH           62
#define EMV_USERSELECT         63    //�ṩӦ���б���û�ѡ��
#define ERR_EMVINITAPP_NOTACCEPED 64
#define EMV_REQUESTPLAINTEXTPIN   65 //EMV��������PIN
#define EMV_REQUESTONLINEENCPIN   66 //������������PIN
#define EMV_REQUESTOFFENCPIN      67 //�����ѻ�����PIN
#define EMV_OFFLINEPINSUC         68 //�ѻ�������ȷ
#define EMV_OFFLINEPINERR         69 //�ѻ��������
#define EMV_TRY_AGAIN_PIN        70 //�����Ե�
#define EMV_LAST_PIN_TRY          71 //���һ��
#define EMV_APPSELECTTRYAGAIN     72 //��ѡӦ��
#define EMV_APPSELECTTRYAGAINSUC  73
#define EMV_SHOWRANDNUM            74
#define EMV_ONLINEWAIT             75
#define EMV_NEEDREFER              76  //��ҪREFER
#define EMV_NEEDREVERSAL           77 //��Ҫ���ͳ���
#define EMV_APPROVED                78 //������׼
#define EMV_DECLINED                79//���׾ܾ�
#define EMV_TRANS_COMPLETED        80//�������
#define EMV_REQUESTAMOUNT          81//����������
#define EMV_VERIFYIDCARD          82 //PBOC֤����֤
#define EMV_DISPEMVRECORD         83 //��ʾ���׼�¼
#define EMV_RETURNDISPLAYLOG      84//��ʾ�꽻����־����
#define EMV_NORECORD               85//û�н�����־
#define EMV_READLOGEND             86//��������־����
#define ERR_EMV_CDADifferCIDTC    87//CDA TC
#define ERR_EMV_CDAFORMAT          88//CDA TCʱ��ʽ����
#define ERR_EMV_CVRFailALWAYS      89//CVR FAIL
#define EMV_SELECTNEXTDDF          90 //Ҫȥ����һ��DDF
#define EMVENC_LAST_PIN_TRY          91 //OFFLINE ENC 
#define EMVENC_TRY_AGAIN_PIN         92//OFFLLINE ENC
#define ERR_EMV_InputERROR           93

#define EMV_NEEDSENDCOMMAND      97
#define EMVERRDEAL                98//���״�����
#define EMVTERMINATEDEAL         99//����ʧ�ܴ���
#define TRANS_ONLINE_APPROVE     100
#define TRANS_ONLINE_DECLINE     101
#define EMVTRANS_COMPLETE        102
#define TRANS_OFFLINE_APPROVE    103
#define TRANS_OFFLINE_DECLINE    104
#define TRANS_TERMINATE_TRANS_ERR 105
#define TRANS_TERMINATE_TRANS_NOTACCEPT 106
#define TRANS_EMV_TRANS_COMPLETED  107
#define EMV_ICCGETPAN              108
#define EMV_COUNTFLOORLIMIT        109
#define qPBOC_READRECORD         110//qPBOC������¼����
#define qPBOC_USEMSD             111 //ʹ��MSD����
#define qPBOC_USEPBOC            112 //ʹ��PBOC����
#define qPBOC_APPEFFECT          113 //Ӧ����δ��Ч
#define qPBOC_APPEXPIRED         114 //Ӧ��ʧЧ
#define EC_NOTSUPPORT            115 //�ÿ��Ǵ������ֽ����֧�ָù���
#define EC_QUANCUNERR            116 //Ȧ����� //huxiang add 091127
#define EMV_NOECTRADE            117
#define qPBOC_RESTARTRANDE       118

#define PAYWAVE_DISPMESSAGE      120
#define PAYWAVE_PAYWAVEIUP       121
#define PAYPASS_CCCWAIT          122
#define PAYPASS_PREDEALFAILED    123
#define PAYPASS_ENDAPPLICATION   124
#define PAYPASS_ERRICCRETURN     125
#define PAYPASS_CCCERR           126
#define PAYPASS_CCCWAITERR       127  //�����Ǵ�reset ��ʼ��
#define PAYWAVE_DETECTEDOTHERMODE 128
#define PAYPASS_TRYANOTHERMODE    129

#define qPBOC_READAIDPARAMETERS   169
#define qPBOC_OTHERAPCARD         170
#define qPBOC_RESETCOMMAND        171
#define qPBOC_RESETFAIL           172
#define qPBOC_DETECTCARDOVERTIME  173
#define qPBOC_MULTICARD           174
#define qPBOC_OTHERERR            175
#define qPBOC_COMPLETE            176
#define qPBOC_RECEIVEDOVERTIME    177
#define qPBOC_READCARDERR         178
#define qPBOC_RECEIVEBAGERR       179
#define PAYPASS_READAFTERBALANCEOK      180  //jiguang
#define PAYPASS_RESELECTAPP      181  //jiguang add
#define ERR_EMV_IccCommandERR      182
#define PAYPASS_DEALWAIT           183
#define PAYPASS_PARSEERR 184
#define PAYPASS_CARDDATAMISSING 185
#define PAYPASS_CARDDATAERR     186
#define PAYPASS_SW1SW2ERR       187
#define PAYPASS_PHONECCCPCIIONLYE 188
#define PAYPASS_DECLINED           189
#define ERR_LASTRECORD             190
#define QPBOC_GETBALANCERETURN       191
#define QPBOC_BLACKLISTCARD          192
#define PAYWAVE_INTERCARD            193
#define PAYEAVE_DOUBLEDIP            194

/*******************************************************
��ǩ��       2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
//card variable
#define TermDataBase   100


#define MV_AppCrypt                0
#define    MV_AppCurcyCode            1
#define    MV_AppCurcyExp            2
#define    MV_AppDiscretData        3
#define    MV_AppEffectDate        4
#define    MV_AppExpireDate        5
#define    MV_AFL                    6
#define    MV_ICC_AID                7
#define    MV_AIP                    8
#define    MV_AppLabel                9
#define    MV_AppPreferName        10
#define    MV_PAN                    11
#define    MV_PANSeqNum            12
#define    MV_AppPriority            13
#define    MV_AppReferCurcy        14
#define    MV_AppReferCurcyExp        15
#define    MV_ATC                    16
#define    MV_AUC                    17
#define    MV_ICC_AppVer            18
#define    MV_CDOL1                19
#define    MV_CDOL2                20
#define    MV_CardholderName        21
#define    MV_CardholderNameExt    22
#define    MV_CVMList                23
#define    MV_ICCAPKI                24
#define    MV_CryptInfoData        25
#define    MV_DataAuthenCode        26
#define    MV_DFName                27
#define    MV_DDFName                28
#define    MV_DDOL                    29
#define    MV_ICCDynNum            30
#define    MV_ICCPIN_EPKCert        31
#define    MV_ICCPIN_EPKExp        32
#define    MV_ICCPIN_EPKRem        33
#define    MV_ICCPKCert            34
#define    MV_ICCPKExp                35
#define    MV_ICCPKRem                36
#define    MV_IACDenial            37
#define    MV_IACOnline            38
#define    MV_IACDefault            39
#define    MV_IssuAppData            40
#define    MV_ICTI                    41
#define    MV_IssuCountryCode        42
#define    MV_IPKCert                43
#define    MV_IPKExp                44
#define    MV_IPKRem                45
#define    MV_LangPrefer            46
#define    MV_LOATCReg                47
#define    MV_LCOL                    48
#define    MV_PINTryCount            49
#define    MV_PDOL                    50
#define    MV_ServiceCode            51
#define    MV_SignDynAppData        52
#define    MV_SignStatAppData        53
#define    MV_SDATagList            54
#define    MV_Track1Discret        55
#define    MV_Track2Discret        56
#define    MV_Track2Equivalent        57
#define    MV_TDOL                    58
#define    MV_UCOL                    59
#define    MV_IssuerURL            60
#define    MV_VLPAvailableFund        61
#define    MV_VLPIssuAuthorCode    62
//lechan 2005-12-26 16:09
#define MV_CardholderIdType 63
#define MV_CardholderIdNo 64
//huxiang  add for qPBOC
#define MV_CardTransPredicable  65
#define MV_UNPREDICTUDOL        66
#define MV_OffAmountDisplay     67
#define MV_PAYWAVEFFI           68

#define MV_PAYPASSTRACK2DATA    73
#define MV_PAYPASSPUNATCTRACK2  74
#define MV_PAYPASSPCVC3TRACK2   75
#define MV_PAYPASSNATCTRACK2    76

#define MV_PAYPASSCVC3TRACK1    77
#define MV_PAYPASSNATCTRACK1    78
#define MV_PAYPASSTRACK1DATA    79
#define MV_PAYPASSCVC3TRACK2    80
#define MV_PAYPASSPCVC3TRACK1   81
#define MV_PAYPASSPUNATCTRACK1  82
#define MV_PAYWAVEAPD           83
#define MV_PAYPASSMAGVER        84

#define MV_PAYPASSPREBALANCE        85   //jiguang 20121220
#define MV_PAYPASSDRDOL        86     //jiguang 20121221
#define MV_PAYPASSPOSCII    87     //jiguang 20121221
#define MV_PAYPASSPOSTBALANCE 88  //jiguang 
#define MV_PAYPASSAPPCAPINFO  89

//other variable needed for host-terminal message
//#define MV_MessageType			80
//#define MV_EncPINData			81
//#define MV_IssuAuthenData		82
//#define MV_IssuScript			83
//#define MV_IssuScriptResult		84
//#define MV_CAPK                 85
//#define MV_ExceptFile           86

//terminal variable
#define    MV_AcquireID            100
#define    MV_TermCapab            101
#define    MV_TermAddCapab            102
#define    MV_IFDSerial            103
#define    MV_TermID                104
#define    MV_MerchCateCode        105
#define    MV_MerchID                106
#define    MV_TermCountryCode        107
#define    MV_TRMData                108
#define    MV_TermType                109
#define    MV_TermAppVer            110
#define    MV_TransCurcyCode        111
#define    MV_TransCurcyExp        112
#define    MV_TransReferCurcyCode    113
#define    MV_TransReferCurcyExp    114
#define    MV_TACDenial            115
#define    MV_TACOnline            116
#define    MV_TACDefault            117
#define    MV_TransType            118
#define    MV_TransTypeValue        119
#define    MV_VLPTransLimit        120
#define    MV_VLPTACDenial            121
#define    MV_VLPTACOnline            122
#define    MV_VLPTACDefault        123
#define    MV_Language                124
#define    MV_bTermDDOL            125
#define    MV_bForceAccept            126
#define    MV_bForceOnline            127
#define    MV_bBatchCapture        128
#define    MV_bTermSupportVLP        129
#define    MV_MaxTargetPercent        130
#define    MV_TargetPercent        131
#define    MV_TermDDOL                132
#define    MV_TermTDOL                133
#define    MV_MerchNameLocate        134
#define    MV_TransLogMaxNum        135
#define    MV_Threshold            136
#define    MV_TermFloorLimit        137
#define    MV_AmtTrans                138
#define    MV_AmtNet                139
#define    MV_BatchTransNum        140
#define    MV_TransNum                141
#define    MV_TransIndex            142
#define    MV_TransSeqCount        143
#define    MV_AmtAuthorBin            144
#define    MV_AmtAuthorNum            145
#define    MV_AmtOtherBin            146
#define    MV_AmtOtherNum            147
#define    MV_AmtReferCurrency        148
#define    MV_TERM_AID                149
#define    MV_AuthorCode            150
#define    MV_AuthorRespCode        151
#define    MV_CVMResult            152
#define    MV_POSEntryMode            153
#define    MV_PIN                    154
#define    MV_TVR                    155
#define    MV_TSI                    156
#define    MV_VLPIndicator            157
#define    MV_TransDate            158
#define    MV_TransTime            159
#define    MV_TCHashValue            160
#define    MV_UnpredictNum            161
#define    MV_IssuerAuthenData        162
#define MV_TransCateCode        163


//lechan 2006-1-7 11:45����
#define MV_TradeType    164
#define MV_Script71Data 165
#define MV_Script72Data 166
#define MV_ScriptResult 167
#define MV_MerchantName    168
//huxiang add for qPBOC
#define MV_TermTransPredicable  169
#define MV_paypassMerchantCustomData 170
#define MV_OffSpendAmount       171
#define MV_FormFactIndicator    172
#define MV_CustomerExclusiveData 173

//jiguang add for Paypass ver3.0.1 20121217
/**************User Interface Request Data********************/
#define UIRD_MessageIdentifier 0
#define UIRD_Status 1
#define UIRD_HoldTime 2
#define UIRD_LanguagePreference 5
#define UIRD_ValueQualifier 13
#define UIRD_Value 14
#define UIRD_CurrencyCode 20
//========Message  Identifier========
#define MI_CARDREADOK 0x17
#define MI_TRYAGAIN 0x21
#define MI_APPROVED 0x03
#define MI_APPROVEDSIGN 0x1A
#define MI_DECLINED 0x07
#define MI_ERROROTHERCARD 0x1C
#define MI_INSERTCARD 0x1D
#define MI_SEEPHONE 0x20
#define MI_AUTHORISINGPLEASEWAIT 0x1B
#define MI_CLEARDISPLAY 0x1E
//===========Status============
#define Status_NOTREADY 0x00
#define Status_IDLE 0x01
#define Status_READYTOREAD 0x02
#define Status_PROCESSING 0x03
#define Status_CARDREADSUCCESSFUL 0x04
#define Status_PROCESSINGERROR 0x05
//============Value Qualifier==========
#define VQ_NONE 0x00
#define VQ_AMOUNT 0x10
#define VQ_BALANCE 0x20

/**************Error Indication********************/
#define EI_L1 0
#define EI_L2 1
#define EI_L3 2
#define EI_SW12 3
#define EI_MsgOnError 5
//===========L1============
#define L1_OK 0x00
#define L1_TIMEOUTERROR 0x01
#define L1_TRANSMISSIONERROR 0x02
#define L1_PROTOCOLERROR 0x03
//==========L2=============
#define L2_OK 0x00
#define L2_CARDDATAMISSING 0x01
#define L2_CAMFAILED 0x02
#define L2_STATUSBYTES 0x03
#define L2_PARSINGERROR 0x04
#define L2_MAXLIMITEXCEEDED 0x05
#define L2_CARDDATAERROR 0x06
#define L2_MAGSTRIPENOTSUPPORTED 0x07
#define L2_NOPPSE 0x08
#define L2_PPSEFAULT 0x09
#define L2_EMPTYCANDIDATELIST 0x0A
#define L2_IDSREADERROR 0x0B
#define L2_IDSWRITEERROR 0x0C
#define L2_IDSDATAERROR 0x0D
#define L2_IDSNOMATCHINGAC 0x0E
#define L2_TERMINALDATAERROR 0x0F
//===========L3============
#define L3_OK 0x00
#define L3_TIMEOUT 0x01
#define L3_STOP 0x02
#define L3_AMOUNTNOTPRESENT 0x03

/**************Outcome Parameter Set********************/
#define OPS_Status 0
#define OPS_Start 1
#define OPS_OnlineResponseData 2
#define OPS_CVM 3
#define OPS_SetBit 4
#define OPS_AlternateInterfacePreference 5
#define OPS_FieldOffRequest 6
#define OPS_RemovalTimeout 7
//============Status=============
#define Status_APPROVED 0x10
#define Status_DECLINED 0x20
#define Status_ONLINEREQUEST 0x30
#define Status_ENDAPPLICATION 0x40
#define Status_SELECTNEXT 0x50
#define Status_TRYANOTHERINTERFACE 0x60
#define Status_TRYAGAIN 0x70
//===============Start===========
#define Start_A 0x00
#define Start_B 0x10
#define Start_C 0x20
#define Start_D 0x30
//==============CVM===========
#define Cvm_NOCVM 0x00
#define Cvm_OBTAINSIGNATURE 0x10
#define Cvm_ONLINEPIN 0x20
#define Cvm_CONFIRMATIONCODEVERIFIED 0x30

/*******************************************************
emvtask      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define EMVNONE 0
#define EMVMAINFACE 1    //������״̬
#define EMVREADLOG 2    //���ò˵����������־
#define MAGTRADE 3
#define EMVHAVECARDIN 4
#define EMVREADLOGCARDIN 5
#define WAITOUTCARD   6
/*******************************************************
����Դ�       2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define EMV_OK                    0
#define EMV_ERR                    1
#define EMV_OTHER              2

/*******************************************************
֧���û�ѡ��Ӧ��      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define bProvideConfirm  1
/*******************************************************
ASI�б�ƥ��       2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define PARTIAL_MATCH  0
#define EXACT_MATCH    1

/*******************************************************
EMV������      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
/*******************************************************
����EMV���������ʽ��      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define COMMANDERR 0xAA
#define COMMTIMEOUT 0xBB




/*******************************************************
����EMV���������ʽ��      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define typePDOL    0
#define typeCDOL1    1
#define typeCDOL2    2
#define typeDDOL    3
#define typeTDOL    4
#define typeDRDOL   5

#define GenerateAC1  1
#define GenerateAC2  2
/*******************************************************
��������      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define CASH        0
#define GOODS        1
#define SERVICE     2
#define CASHBACK    3
#define REFUND      4
#define CASHDEPOSIT 5
/*******************************************************
CVR RESULT      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define CVR_UNKNOWN        0
#define CVR_FAIL        1
#define CVR_SUCCESS        2


/*******************************************************
Ӧ������      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define EMVTYPE   0
#define PBOCTYPE  1
#define qPBOCTYPE 2
#define MSDTYPE   3
#define PAYPASSAPP   0
#define PAYWAVEAPP   1
#define EMVAPP       2

/*******************************************************
�ֿ�����֤���е��Ĳ���      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
#define CARDHOLDERVER_INIT 0
#define CARDHOLDERVER_ANALY 1
#define CARDHOLDERVER_PERFORM  2


#define VERIFYSTEP_INIT  0
#define VERIFYSTEP_PLAINTEXTPIN 1
#define VERIFYSTEP_ENCONLINEPIN 2
#define VERIFYSTEP_ENCOFFLINEPIN 3
#define VERIFYSTEP_VERIFYIDCARD  4
#define VERIFYSTEP_PINERR         5
#define VERIFYSTEP_GETCHANLLENGE 6


//#define PINPAD_VALID 1


//........Analyse.c
#define OFFLINE 10
#define ONLINE  11
#define DENIAL  13

//transaction result
#define OFFLINE_APPROVE        10
#define OFFLINE_DECLINE        11
#define ONLINE_APPROVE        12
#define ONLINE_DECLINE        13
#define ONLINE_WAIT            14    //lechan 2006-8-12 17:14
#define TERMINATE_TRANS_ERR   15
#define TERMINATE_TRANS_NOTACCEPT 16 //��Ҫ��ʾNOT ACCEPT,����ֹ����
#define FINAL_ONLINEAPPROVE        17
#define FINAL_ONLINEDECLINE        18
#define FINAL_ONLINEERRCODE        19
#define FINAL_ONLINEERRBACK        20
#define FINAL_PAYPASSUSEOTHERMODE  21
#define ONLINE_RESULTUNKNOW        22

#define CryptType_TC      10
#define CryptType_ARQC    11
#define CryptType_AAR     12
#define CryptType_AAC     13


#define EMVRMAINACC  1
#define EMVRTRADEAMOUNT 12
#define INDEXLEN  128

#define ICC_REFER         0
#define ISSUER_REFER      1


#define TRANSLOGMAXNUM 38

#define NORMALSALE 0
#define CASHBACKSALE 1


/**********************************
��������Ľṹ
***********************************/
/*
#define EMVFIRST 0
#define WAITCARDDATA 1
#define WAITCARDDATA1 2
#define WAITCARDDATA2 3
#define WAITCARDDATA3 4
#define WAITCARDDATA4 5
#define WAITCARDDATA5 6
#define WAITCARDDATA6 7
#define WAITCARDDATA7 8
#define WAITCARDDATA8 9
#define WAITCARDDATA9 10
#define WAITCARDDATA10 11
#define WAITCARDDATA11 12
#define WAITCARDDATA12 13
#define WAITCARDDATA13 14
#define WAITCARDDATA14 15
#define WAITCARDDATA15 16
#define WAITCARDDATA16 17
#define WAITCARDDATA17 18
#define WAITCARDDATA18 19
*/




#define MAXSCRIPTLEN 128



//display-string macro definition
#define VERSION_INFO                            0
//standard message:0x01-0x13.
#define AMOUNT                                      1
#define AMOUNT_OK                                  2
#define APPROVED                                 3
#define CALL_YOUR_BANK                           4
#define CANCEL_OR_ENTER                           5
#define CARD_READ_ERROR                           6
#define DECLINED                                  7
#define ENTER_AMOUNT                              8
#define ENTER_PIN                                 9
#define INCORRECT_PIN                            10
#define INSERT_CARD                              11
#define NOT_ACCEPTED                             12
#define PIN_OK                                     13
#define PLEASE_WAIT                                 14
#define PROCESSING_ERROR                         15
#define REMOVE_CARD                              16
#define USE_CHIP_READ                            17
#define USE_MAG_STRIPE                           18

//main and setup menu title
#define UPDATE_PARA                                 20
#define BATCH_UP                                 21
#define PRINT_RECEIPT                            22

#define SET_SYSTIME                                 30
#define SET_TIDMID                                 31
#define SELECT_LANGUAGE                          32
#define SET_TAC                                     33
#define SET_TRMPARA                                 34
#define SET_TRANSTYPE                            35
#define SET_VLPPARA                                 36


#define TRANS_REFUSED                            40
#define BYPASS_REFERRAL                          41
#define ACCEPT_TRANS                             42
#define DECLINE_TRANS                             43
#define FORCE_ONLINE                             44
#define TITLE_APP_SELECT                         45
#define MAGCARD_READ_ERROR                       46
#define EMV_LEVEL2_TEST                          47
#define SAND_TECH_LTD                             48
#define EMV_ENGLISH                                 49
#define EMV_CHINESE                                 50
#define SETUP_DATE                               51
#define YYMMDD                                     52
#define OLD_FL                                     53
#define NEW_FL                                     54
#define SETUP_TIME                               55
#define HHMM                                     56
#define TERMINAL_ID                                 57
#define OLD_ID                                     58
#define NEW_ID                                   59
#define MERCHANT_ID                                 60
#define TAC_DENIAL                                 61
#define TAC_ONLINE                                 62
#define TAC_DEFAULT                                 63
#define OLD_TAC                                     64
#define NEW_TAC                                     65
#define FLOOR_LIMIT                                 66
#define THRESHOLD                                 67
#define TP                                         68
#define MTP                                         69
#define NEED_PIN                                 70
#define ENTER_FOR_YES                             71
#define CANCEL_FOR_NO                             72
#define NO_APP_TRANS                             73
#define PRN_NO_PAPER                             74
#define PRN_PRINTING                             75
#define PUT_ON_BASE                                 76
#define PRN_ERROR                                 77
#define NO_BATCH_DATA                            78
#define ONLINE_ERROR                             79
#define READ_TRACK1_OK                             80
#define READ_TRACK2_OK                             81
#define READ_TRACK3_OK                             82
#define BATCH_UP_OK                                 83
#define TRANS_COMPLETED                          84
#define TRANS_TERMINATED                         85
//#define DIALING									 86
#define SERVICE_NOT_ALLOWED                      87
#define TITLE_TRANS_TYPE                         88
#define EMV_TRY_AGAIN                                89
#define WAIT_FOR_PIN                             90
#define STR_VLP                                     91
#define VLP_TRANS_LIMIT                             92
#define LAST_PIN_TRY                             93
#define TRANS_AMOUNT                             94
#define CASHBACK_AMOUNT                 95
#define PBOC_VER                96
#define INITEMV_SYSDATA                97
#define APP_BLOCK    98
#define CARD_READ_TIMEOUT 99
#define CARD_qPBOC_IN                             100
#define TERM_CVMNOSUPPORT                         101
#define CARD_qPBOC_OUT                             102
#define CARD_USEMSD                              103
#define CARD_USEPBOC                             104



//PAYPASS selected mode
#define PAYPASS_SELECTED_MODE_C 0x20


/*******************************************************
�����ṹ        2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/
//1��1.1 ����APDU��ʽ
typedef struct {
    unsigned char Command[4];
    unsigned short Lc;
    unsigned char DataIn[300];
    unsigned short Le;
    unsigned char EnableCancel;
} APDU_SEND;


//1��1.2 Ӧ��APDU��ʽ
typedef struct {
    unsigned char ReadCardDataOk;
    unsigned short LenOut;
    unsigned char DataOut[300];
    unsigned char SWA;
    unsigned char SWB;
} APDU_RESP;


//1.1.3 ��״̬
typedef struct {
    unsigned char RxState;
    unsigned char RxStateT;
} CARD_STATE;









/*******************************************************
Ӧ��ѡ��ṹ        2008-03-31 ��Т��
�޸���Ϣ:
********************************************************/
//1��2��1 ���ļ�ѡ��Ӧ���ļ���ʽ
typedef struct {
    unsigned char Type;
    unsigned char DFNameExist;
    unsigned char DFNameLen;
    unsigned char DFName[16];
    unsigned char FCIPropExist;
    unsigned char SFIExist;
    unsigned char SFI;
    unsigned char LangPreferExist;
    unsigned char LangPreferLen;
    unsigned char LangPrefer[8];
    unsigned char ICTIExist;
    unsigned char ICTI;     //���������������
    unsigned char AppLabelExist;
    unsigned char AppLabelLen;
    unsigned char AppLabel[16];
    unsigned char PriorityExist;
    unsigned char Priority;
    unsigned char PDOLExist;
    unsigned char PDOLLen;
    //unsigned char PDOL[252];
    unsigned char PreferNameExist;
    unsigned char PreferNameLen;
    unsigned char PreferName[16];
    unsigned char IssuerDiscretExist;  //�������Զ�������
    unsigned char IssuerDiscretLen;
    //unsigned char IssuerDiscret[254];  //sxl?�鿴�Ͽ���Щ�����Ƿ���ʹ��
    unsigned char LogEntryExist;
    unsigned char LogEntryLen;
    //unsigned char LogEntry[222];
    unsigned char paypassselectppse61tagexist;
} SELECT_RET;
//1��2��2 ��������¼Ӧ���ʽ
typedef struct {
    unsigned char Type;              //1:DDF,2:ADF
    unsigned char DFNameExist;      //0-non exist;1-exist.
    unsigned char DFNameLen;
    unsigned char DFName[16];       //5-16,ADF or DDF name according to Type.
    unsigned char AppLabelExist;
    unsigned char AppLabelLen;
    unsigned char AppLabel[16];
    unsigned char PreferNameExist;
    unsigned char PreferNameLen;
    unsigned char PreferName[16];
    unsigned char PriorityExist;
    unsigned char Priority;         //tag'87'
    unsigned char DirDiscretExist;
    unsigned char DirDiscretLen;
    //unsigned char DirDiscret[254];   //sxl?�鿴�Ͽ���Щ�����Ƿ���ʹ��
} RECORD_PSE;
//1��2��3 Ӧ�ñ�ǩ�ṹ
typedef struct {
    unsigned char ASI;               //0-partial match up to the length;1-match exactly
    unsigned char AIDLen;
    unsigned char AID[16];          //5-16
    unsigned char bLocalName;       //If display app list using local language considerless of info in card.0-use card info;1-use local language.
    unsigned char AppLocalNameLen;
    unsigned char AppLocalName[16];
    unsigned char KernelID;
} TERMAPP;


typedef struct {  //sxl?ȱ�ٺܶ����,�鿴�Ͽ���Щ�����Ƿ���ʹ��
    unsigned char AIDLen;
    unsigned char AID[16];
    //5-16
    unsigned char AppLabelLen;
    unsigned char AppLabel[16];
    unsigned char PreferNameLen;
    unsigned char PreferName[16];
    unsigned char Priority;
    unsigned char LangPreferLen;
    unsigned char LangPrefer[8];        //2-8
    unsigned char ICTI; //Issuer Code Table Index.lang used for display app list according to ISO8859.but not include Chinese,Korea,etc.
    unsigned char ICTILen;
    unsigned char AidInTermLen;
    unsigned char AidInTerm[16];        //��Ӧ���ն˵�AID�ͳ���
    unsigned char KernelIdentifierLen;
    unsigned char KernelIdentifier[8];    //Kernel Identifier   jiguang 20121217
    unsigned char ReqKernelID;
} LISTAPPDATA;


//1��2��5  Ӧ�ñ�ǩ����
/*
typedef struct {   
	unsigned char AIDLen;
	unsigned char AID[16];//5-16
	unsigned char AppLabelLen;
	unsigned char AppLabel[16];
	unsigned char PreferNameLen;
	unsigned char PreferName[16];
	unsigned char Priority; //tag'87'
	unsigned char LangPreferLen;
	unsigned char LangPrefer[8];//2-8
	unsigned char ICTI; //Issuer Code Table Index.lang used for display app list according to ISO8859.but not include Chinese,Korea,etc.
	unsigned char ICTILen;   
	unsigned char PDOLLen;
	unsigned char PDOL[127];
	unsigned char DirDiscretLen;
	unsigned char DirDiscret[100];
	unsigned char IssuerDiscretLen;
	unsigned char IssuerDiscret[222];   //sxl  
	unsigned char bLocalName;//If display app list using local language considerless of info in card.0-use card info;1-use local language.
	unsigned char AppLocalNameLen;
	unsigned char AppLocalName[16];
	unsigned char AidInTermLen;
	unsigned char AidInTerm[16];		//��Ӧ���ն˵�AID�ͳ��� 
	unsigned char KernelIdentifierLen;
	unsigned char KernelIdentifier[8];    //Kernel Identifier   jiguang 20121217
	unsigned char ReqKernelID;
	
	
	}APPDATA;		//Ӧ��������Ϣ
*/







//
typedef struct {
    unsigned char Tag[3];
    //Second byte '00' means only first one byte tag.add third byte to make int variable oven aligned.
    unsigned char Len1; //Len1>Len2: fix len with Len2;Len1<Len2 and Len2!=255: Len1<= len <=Len2; Len2=255: len=Len1+Len2
    unsigned char Len2;
    unsigned char bAmt;
    //0- non numeric;1-numeric;2-compact numric
    unsigned short address;
    //var address in struct
    unsigned char flagM;
    //'1'--bit0: mandatory;bit1: SDA mandatory; bit2: DDA mandatory;
    unsigned char bExist;//0-not presented,1-have been existed.
} DATAELEMENT;


#define RFD_AID


typedef struct {
    unsigned char Tag[3];
    //Second byte '00' means only first one byte tag.add third byte to make int variable oven aligned.
    unsigned char updatecondition;
} PAYPASSTAGBASELIB;


typedef struct {
/*
	unsigned char AidLen;
	unsigned char Aid[16];
	unsigned char Asi;
*/
    unsigned char AidLen;
    unsigned char Aid[16];
    //5-16
    unsigned char Asi;  //Application Selection Indicator.0-needn't match exactly(partial match up to the length);1-match exactly
    unsigned char AppVerNum[2];
    unsigned char TacDefault[5];
    unsigned char TacOnline[5];
    unsigned char TacDecline[5];
    unsigned char FloorLimit[4];
    unsigned char Threshold[4];
    unsigned char MaxTargetPercent;
    unsigned char TargetPercent;
    unsigned char TermDDOLLen;
    unsigned char TermDDOL[128];
    unsigned char TermPinCap;
    unsigned char vlptranslimit[6]; //huxiang add 091013 for Ec and qPBOC
    unsigned char termcvm_limit[6]; //huxiang add 091013 for Ec and qPBOC       cvm limit
    unsigned char clessofflineamt[6]; //huxiang add 091013 for Ec and qPBOC   transaction limit
    unsigned char clessofflinelimitamt[6]; //huxiang add 091013 for Ec and qPBOC floor limit
    unsigned char bShowRandNum;
    unsigned char bLocalName;
    //If display app list using local language considerless of info in card.0-use card info;1-use local language.
    unsigned char AppLocalNameLen;
    unsigned char AppLocalName[16];

    unsigned char bForceOnline;


#ifdef RFD_AID

    unsigned char terminaltype;
    unsigned char terminalcapability[3];
    unsigned char terminaladdcapability[5];
    unsigned char reservedlen;
	unsigned char reserved[128]; //total 485
	unsigned char appID;				//应用标识
	unsigned char TermTransPredicable[4];	//HHL: 20110226
	unsigned char paywavepredealparam[2];
	unsigned char ondevRTCL[6];
	unsigned char KernelID;
	unsigned char KernelConfigure;
	unsigned char magcvmrequirer;           
	unsigned char magnocvmrequirer;
	unsigned char PaypassMagVer[2];         //paypass magstripe version
	unsigned char tradetypevalue;
	#endif
	
}EmvSysIni_STRUCT;


typedef struct {
    unsigned char RID[5];
    unsigned char CAPKI;
    unsigned char SMCapkFlag;
    unsigned char HashInd;
    unsigned char ArithInd;
    unsigned char ModulLen;
    unsigned char Modul[248];
    unsigned char ExponentLen;
    unsigned char Exponent[3];
    unsigned char CheckSumLen;
    unsigned char CheckSum[20];
    unsigned char ExpireDate[4];    //ʧЧ��  // ��Ч�ڵĴ���
    unsigned char ParamInd;         //��Կ������ʶ
    unsigned char DGTLSGNTRlen;     //SM flag
    unsigned char DGTLSGNTR[64];
} CAPK_STRUCT;

typedef struct {
    unsigned char TACDenial[5];
    unsigned char TACOnline[5];
    unsigned char TACDefault[5];
} ISSUER_TAC;

//�ӷ����й�Կ֤��ָ����ݵĸ�ʽ
typedef struct {
    unsigned char DataHead;        //'6A'�ָ�����ͷ
    unsigned char CertFormat;        //'02'֤���ʽ
    unsigned char IssuID[4];        //���ʺ��������3-8����(�Ҳ�F)�����б�ʶ
    unsigned char ExpireDate[2];    //֤��ʧЧ����MMYY
    unsigned char CertSerial[3];    //֤�����к�
    unsigned char HashInd;            //��ϣ�㷨��ʶ
    unsigned char IPKAlgoInd;        //�����й�Կ�㷨��ʶ
    unsigned char IPKLen;            //�����й�Կ���ֽڳ���
    unsigned char IPKExpLen;        //�����й�Կָ�����ֽڳ���
    unsigned char IPKLeft[212];    //�����й�Կ��Կ����ߵ��ֽ�NCA-36�����Ҳ�BB
    unsigned char HashResult[20]; //��ϣ���
    unsigned char DataTrail;        //'BC'�ָ����ݽ�β
} IPK_RECOVER;

typedef struct {
    unsigned char DataHead;
    //'6A'
    unsigned char CertFormat;
    //'04'
    unsigned char AppPAN[10];
    unsigned char ExpireDate[2];
    unsigned char CertSerial[3];
    unsigned char HashInd;
    unsigned char ICCPKAlgoInd;
    unsigned char ICCPKLen;
    unsigned char ICCPKExpLen;
    unsigned char ICCPKLeft[206];
    //NI-42
    unsigned char HashResult[20];
    unsigned char DataTrail;//'BC'
} ICCPK_RECOVER;

typedef struct {
    unsigned char DataHead;
    //'6A'
    unsigned char DataFormat;
    //'05'
    unsigned char HashInd;
    //'01'
    unsigned char ICCDynDataLen;
    unsigned char ICCDynData[223];
    //LDD	<NIC-25
    unsigned char PadPattern[223];
    //NIC-LDD-25,padded with 'BB'
    unsigned char HashResult[20];
    unsigned char DataTrail;//'BC'
} SIGN_DYN_APPDATA_RECOVER;

typedef struct {
    unsigned char DataHead;
    //'6A'
    unsigned char DataFormat;
    //'03'
    unsigned char HashInd;
    unsigned char DataAuthCode[2];
    unsigned char PadPattern[222];
    //NI-26
    unsigned char HashResult[20];
    unsigned char DataTrail;//'BC'
} SIGN_STAT_APPDATA_RECOVER;

typedef struct {
    unsigned char RID[5];
    unsigned char CAPKI;
    unsigned char CERTSerial[3];
} IPK_REVOKE;


typedef struct {
    unsigned char DataHead;
    //'6A'
    unsigned char CertFormat;
    //'04'
    unsigned char AppPAN[10];
    unsigned char ExpireDate[2];
    unsigned char CertSerial[3];
    unsigned char HashInd;
    unsigned char ICCPIN_EPKAlgoInd;
    unsigned char ICCPIN_EPKLen;
    unsigned char ICCPIN_EPKExpLen;
    unsigned char ICCPIN_EPKLeft[206];
    //NI-42
    unsigned char HashResult[20];
    unsigned char DataTrail;//'BC'
} ICCPIN_EPK_RECOVER;


typedef struct {
    unsigned char IACDenial[5];
    unsigned char IACOnline[5];
    unsigned char IACDefault[5];
} Card_TAC;

typedef struct {
    unsigned char ICCDynNumLen;
    unsigned char ICCDynNum[8];
    unsigned char CryptInfo;
    unsigned char AppCrypt[8];
    unsigned char HashResult[20];
} ICC_DYN_DATA;


typedef struct {
    unsigned char AppPriority[16];
    unsigned char AppSeq[16];
    unsigned char seqtype;
    unsigned char SelectedNum;
    unsigned char SelectedResult;
} DISPSEQLISTDATA;


typedef struct{
	unsigned int  TransAmt;        /* Transaction amount */
    unsigned int TransCount;     /* Transaction Sequence Counter. inc by 1 for each trans*/
    unsigned char  TransType;        /* Transaction type BCD */
	unsigned char  TSI[2];           /* Transaction Status Information */
    unsigned char  PANLen;
    unsigned char  PAN[10];          /* Primary account No. */
	unsigned char  PANSeq;           /* EMVTest: sequence num with same PAN */
    unsigned char  TransDate[3];     /* Trancaction Date(Form:"YY/MM/DD") */
    unsigned char  TransTime[3];     /* Trancaction Time(Form:"HH/MM/SS") */
} TRANS_LOG;


//paypass Pre-processing struct
typedef struct _PAYPASSPREPROCEFLAG {
    unsigned char TermCTLEF:1;    //terminal contactless transaction limit exceeded flag
    unsigned char TermCFLEF:1;   //terminal contactless floor limit exceed flag
    unsigned char TermCVMRLEF:1; //terminal CVM required limit exceeded flag
    unsigned char PaywaveCANAI:1;
    //Contactless Application Not Allowed indicator  jiguang
    unsigned char DRLsupport:1;
    unsigned char OndevCTLEF:1;
} PAYPASSPREPROCEFLAG;


typedef struct _DISCRETIONARYITEMITEM {
    unsigned char errindicationflag;
    unsigned char emvdataflag;
    unsigned char msgdataflag;
} DISCRETIONARYITEMITEM;

typedef struct _OUTPARAMETERSET {
    unsigned char OutcomeParSet[8];
    unsigned char status;
    unsigned char start;
    unsigned char CVM;
    unsigned char UIRequestOnOutPresent;
    unsigned char UIRequestOnRestartPresent;
    unsigned char DataRecordPresent;
    unsigned char DisDataPresent;
    unsigned char ReceiptinoutParameter;
    unsigned char AIPinoutParameter;
    unsigned char FieldoffinoutParameter;
    unsigned char RemovalTimeoutinoutParameter;
    unsigned char OnlineResDatainoutParameter;
    unsigned char opssendflag;
    struct _DISCRETIONARYITEMITEM discretionaryitem;
    unsigned char selectsucdata;
    unsigned short FCIdatalen;
    unsigned char FCIdata[256];
    unsigned char SelectCombDatalen;
    unsigned char SelectCombData[20];
} OUTPARAMETERSET;


#define USERINTERFACEREQDATASIZE  22
typedef struct _USERINTERFACEREQDATA {
    unsigned char MessageID;
    unsigned char Status;
    unsigned char HoldTime[3];
    unsigned char LanguagePreference[8];
    unsigned char ValueQualifier;
    unsigned char value[6];
    unsigned char CurrencyCode[2];
    unsigned char LanguagePreferenceLen;
    unsigned char sendMsgFlag;
} USERINTERFACEREQDATA;


typedef struct _ERRORID   //DF8115 - 6
{
    unsigned char L1;
    unsigned char L2;
    unsigned char L3;
    unsigned char SW12[2];
    unsigned char MsgOnError;
} ERRORID;


#define MAXPHONEMESSAGETABLENUM 5
typedef struct {
    unsigned char PCIIMASK[3];
    unsigned char PCIIVALUE[3];
    unsigned char MESSAGE;
    unsigned char STATUS;
} PHONEMESSAGETABLE;


typedef struct {
    unsigned int phonemessagetablenum;
    PHONEMESSAGETABLE phonemessagetable[MAXPHONEMESSAGETABLENUM];
} PHONEMESSAGETABLEINFO;


typedef struct {
    unsigned char tag[4];
    unsigned char updatecondition;
    unsigned short tagdatalen;
    unsigned short tagdatapos;

} PAYPASS_DEFTAGDATA;


#define PAYPASSTAGINFOFILE       "paypasstag"
#define PAYPASSTAGDATAFILE       "paypasstad"

#define MAXPAYPASSDEFTAGNUM 30
#define PAYPASS_DEFTAGINFOLEN 1024
#define PAYPASS_TERMTAGDATABAKLEN 1024
typedef struct {
    unsigned int PaypassDefTagNum;
    unsigned int tagdatalen;
    PAYPASS_DEFTAGDATA PaypssDefTagData[MAXPAYPASSDEFTAGNUM];
} PAYPASS_DEFTAG;  //30 TERMINFO tag ���������������˵�ǹ��ģ����е�TERMINFO���ᳬ��1K





//TermDataBase
//#define PAYPASSTERMICCTAGEXISTFILE  "ticctagflag"
#define PAYPASS_TAGBASEFILELEN 1024


#define PAYPASS_UPDATECONDITION_ACT  0x10
#define PAYPASS_UPDATECONDITION_RA   0x20
#define PAYPASS_UPDATECONDITION_K    0x40
#define PAYPASS_KNOWNTAG             0x80


#define PAYPASS_DETAG                0x04
#define PAYPASS_TAGVALUEEMPTY        0x08


typedef unsigned char (*core_checkifcardintornlog)(void);

typedef unsigned char (*core_RECOVERAC)(APDU_SEND *apdu_s);

typedef unsigned char (*core_deletetornlog)(void);

typedef void (*core_addtornlog)(unsigned int AppUnionStructaddr);

typedef void (*core_senduserinterfacerequestdata)(unsigned int step);

typedef void (*core_sendoutparamset)(unsigned int step);

typedef void (*core_sendDisData)(void);

typedef void (*core_sendendapplicationdatarecord)(void);

typedef unsigned char (*core_GetTagUpdateCondition)(unsigned char *tag, unsigned char taglen);

typedef unsigned char (*core_copytornrecorddata)(void);

typedef unsigned char (*core_freetornlogtempdata)(void);

typedef void (*core_promptremovecardsoundled)(void);


//����Ҫ��֤�����ݷŵ���ǰ�棬�ӿ��ȡ�ٶ�
//paypass recovery data
#define TORNRECORDFLAG   "\x55\xaa\x66\xbb"
typedef struct _TornTransactionLogRecordSIZE {
    unsigned char flag[4];          //\x55\xaa\x55\xaa˵���м�¼
    unsigned char recordseqnum[6];
    unsigned char transdatetime[7];
    unsigned char AmtAuthNum[6];    //tag'9F02' Authorised amount of BCD numeric
    unsigned char AmtOtherNumexist;
    unsigned char AmtOtherNum[6];         //tag'9F03' Other(cashback) amount of BCD numeric
    unsigned char PANLen;
    unsigned char PAN[10];
    unsigned char PANSeqexist;
    unsigned char PANSeq;                    //identify card with same pan.
    unsigned char PayPassPreBalanceExist;   //
    unsigned char PayPassPreBalance[6];     //paypass tag '9f50'  jiguang 20121220 add
    unsigned char CDOL1RelatedLen;
    unsigned char CDOL1RelatedData[256];  //�����ݶ���ô��
    unsigned char CVMResult[3];            //tag'9F34' cardholder verification methods perform result
    unsigned char DRDOLRelatedLen;
    unsigned char DRDOLRelatedData[256];  //�����ݶ���ô��
    unsigned char DSSummary1Len;    //8  or 16
    unsigned char DSSummary1[16];
    unsigned char IDSStatus;
    unsigned char InterfaceDeviceSerialNumberExist;
    unsigned char InterfaceDeviceSerialNumber[8];
    unsigned char PDOLRelatedLen;
    unsigned char PDOLRelatedData[256];
    unsigned char ReferenceControlParameter;
    unsigned char TermCapab[3];              //tag'9F33' terminal capability
    unsigned char CountryCodeexist;
    unsigned char CountryCode[2];          //tag'9F1A' Terminal country code BCD
    unsigned char TermType;                  //tag'9F35' Terminal type
    unsigned char TVR[5];                  //tag'95'   Terminal Verification Results
    unsigned char TransCateCodeexist;
    unsigned char TransCateCode;          //tag '9F53' Transaction Category Code, Mastercard M/Chip private data.
    unsigned char TransCurcyCodeexist;
    unsigned char TransCurcyCode[2];      //tag'5F2A'
    unsigned char TransDate[3];           //tag'9A'   YYMMDD
    unsigned char TransTime[3];           //tag'9F21',HHMMSS,BCD
    unsigned char TransTypeValue;         //tag'9C',
    unsigned char UnpredictNum[4];        //tag'9F37' Terminal created for each transaction.
    unsigned char CDOL1Len;
    unsigned char CDOL1[256];
}ATTRIBUTE TornTransactionLogRecordSize;

typedef struct _TornTransactionLogRecord {
    unsigned char flag[4];          //\x55\xaa\x55\xaa˵���м�¼
    unsigned char recordseqnum[6];
    unsigned char transdatetime[7];
    unsigned char AmtAuthNum[6];    //tag'9F02' Authorised amount of BCD numeric
    unsigned char AmtOtherNumexist;
    unsigned char AmtOtherNum[6];         //tag'9F03' Other(cashback) amount of BCD numeric
    unsigned char PANLen;
    unsigned char PAN[10];
    unsigned char PANSeqexist;
    unsigned char PANSeq;                    //identify card with same pan.
    unsigned char PayPassPreBalanceExist;   //
    unsigned char PayPassPreBalance[6];     //paypass tag '9f50'  jiguang 20121220 add
    unsigned char CDOL1RelatedLen;
    unsigned char CDOL1RelatedData[256];  //�����ݶ���ô��
    unsigned char CVMResult[3];            //tag'9F34' cardholder verification methods perform result
    unsigned char DRDOLRelatedLen;
    unsigned char DRDOLRelatedData[256];  //�����ݶ���ô��
    unsigned char DSSummary1Len;    //8  or 16
    unsigned char DSSummary1[16];
    unsigned char IDSStatus;
    unsigned char InterfaceDeviceSerialNumberExist;
    unsigned char InterfaceDeviceSerialNumber[8];
    unsigned char PDOLRelatedLen;
    unsigned char PDOLRelatedData[256];
    unsigned char ReferenceControlParameter;
    unsigned char TermCapab[3];              //tag'9F33' terminal capability
    unsigned char CountryCodeexist;
    unsigned char CountryCode[2];          //tag'9F1A' Terminal country code BCD
    unsigned char TermType;                  //tag'9F35' Terminal type
    unsigned char TVR[5];                  //tag'95'   Terminal Verification Results
    unsigned char TransCateCodeexist;
    unsigned char TransCateCode;          //tag '9F53' Transaction Category Code, Mastercard M/Chip private data.
    unsigned char TransCurcyCodeexist;
    unsigned char TransCurcyCode[2];      //tag'5F2A'
    unsigned char TransDate[3];           //tag'9A'   YYMMDD
    unsigned char TransTime[3];           //tag'9F21',HHMMSS,BCD
    unsigned char TransTypeValue;         //tag'9C',
    unsigned char UnpredictNum[4];        //tag'9F37' Terminal created for each transaction.
    unsigned char CDOL1Len;
    unsigned char CDOL1[256];
    unsigned char Reserved[2048 - sizeof(TornTransactionLogRecordSize) - 2];
    unsigned char crc[2];
}ATTRIBUTE TornTransactionLogRecord;


typedef struct {
    unsigned int index;
    unsigned int exit;
    unsigned char PANLen;
    unsigned char PAN[10];
    unsigned char PANseq;
    unsigned char recordseqnum[6];
    unsigned char transdatetime[7];
} TORNTRANSACTIONMAP;


typedef struct _TORNTRANSACTIONMAPLIST {
    TORNTRANSACTIONMAP TornTransactionInfo;
    struct _TORNTRANSACTIONMAPLIST *next;
} TORNTRANSACTIONMAPLIST;


typedef struct {
    unsigned char *flashusedflag;
    unsigned int TornMaxNum;
    unsigned int currentusedindex;
    TORNTRANSACTIONMAPLIST *tornhead;
} TORNTRANSACTIONMAPTOTALINFO;








//=�����ֽ� NowTask ectrade huxiang add for EC-------
#define EMV_EC_CASH                0x2000                        //�ֽ��ֵ
#define EMV_EC_QCACC                0x2001                        //ָ���˻�Ȧ��
#define EMV_EC_QCACCNO            0x2002                        //��ָ���˻�Ȧ��
#define EMV_READLOG            0x2003                        //����־
#define PAYWAVE_CASH            0x2004
/*******************************************************
����ı���      2008-03-31 ��Т��
�޸���Ϣ:

********************************************************/




/*******************************************************
��������        2008-03-31 ��Т��
�޸���Ϣ:
********************************************************/
//2.1.1 ����TAG���ݵĳ���



/*******************************************************
Ӧ��ѡ����        2008-03-31 ��Т��
�޸���Ϣ:
********************************************************/


extern void ContactlessIccIsoCommand(APDU_SEND *ApduSend,APDU_RESP *apdu_r); 
extern void IccIsoCommand(APDU_SEND *ApduSend,APDU_RESP *apdu_r); 
extern unsigned char Paypass_TornRecordread(unsigned int index,TornTransactionLogRecord *torntransactionlogrecord);
extern unsigned char Paypass_TornRecordsave(unsigned int index,TornTransactionLogRecord *torntransactionlogrecord);
extern int Paypass_getspectagdata(unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short *tagdatalen,unsigned char *updatecondition);
extern int Paypass_CheckSpecTag(unsigned char *tag,unsigned char taglen,unsigned short *tagdatalen);

extern unsigned char Paypass_copytornrecorddata(void);

extern unsigned char Paypass_freetornlogtempdata(void);

extern unsigned char Emv_CheckDateFormat(unsigned char *date);


#define PAYPASSSTEP_S51_11_0    51110
#define PAYPASSSTEP_S51_12_0    51120
#define PAYPASSSTEP_S52_5       5205
#define PAYPASSSTEP_S52_7       5207
#define PAYPASSSTEP_S52_8       5208
#define PAYPASSSTEP_S52_9       5209
#define PAYPASSSTEP_S52_11      5211
#define PAYPASSSTEP_S52_13      5213
#define PAYPASSSTEP_S52_14      5214
#define PAYPASSSTEP_S52_18      5218
#define PAYPASSSTEP_S52_19      5219
#define PAYPASSSTEP_S53_5       5305
#define PAYPASSSTEP_S53_7       5307
#define PAYPASSSTEP_S53_8       5308
#define PAYPASSSTEP_S53_13      5313
#define PAYPASSSTEP_S53_14      5314
#define PAYPASSSTEP_S53_15      5315
#define PAYPASSSTEP_S1_7_0      1070
#define PAYPASSSTEP_S1_7_1      1071
#define PAYPASSSTEP_S1_8_0      1080
#define PAYPASSSTEP_S3_5_0      3050
#define PAYPASSSTEP_S3_9_1      3091
#define PAYPASSSTEP_S3_9_2      3092
#define PAYPASSSTEP_S3_12_0     3120
#define PAYPASSSTEP_S3_14_0     3140
#define PAYPASSSTEP_S3_90_1     3901
#define PAYPASSSTEP_S3_90_2     3902
#define PAYPASSSTEP_S3_E41_0  103400
#define PAYPASSSTEP_S3_E18_0  103180
#define PAYPASSSTEP_S4_E4_0   104040
#define PAYPASSSTEP_S4_E5_0   104050
#define PAYPASSSTEP_S4_E6_0   104060
#define PAYPASSSTEP_S4_E10_1  104101
#define PAYPASSSTEP_S4_E10_2  104102
#define PAYPASSSTEP_S4_E13_0  104130
#define PAYPASSSTEP_S4_E15_0  104152
#define PAYPASSSTEP_S4_E27_1  104271
#define PAYPASSSTEP_S4_E27_2  104272
#define PAYPASSSTEP_S5_E17_1  105171
#define PAYPASSSTEP_S5_E17_2  105172
#define PAYPASSSTEP_S5_E27_1  105271
#define PAYPASSSTEP_S5_E27_2  105272
#define PAYPASSSTEP_S5_E31_0  105310
#define PAYPASSSTEP_S7_8        7080
#define PAYPASSSTEP_S16_E1_0  116010
#define PAYPASSSTEP_S16_E2_0  116020
#define PAYPASSSTEP_S16_E3_0  116030
#define PAYPASSSTEP_CVM_E4_0  600040
#define PAYPASSSTEP_CVM_E8_0  600080

#define PAYPASSSTEP_CVM_E18_0 600180
#define PAYPASSSTEP_CVM_E18_1 600181
#define PAYPASSSTEP_S78_20_0   78200
#define PAYPASSSTEP_S13_4_0    13040
#define PAYPASSSTEP_S13_5_0    13050
#define PAYPASSSTEP_S13_10_0   13100
#define PAYPASSSTEP_S13_13_0   13130
#define PAYPASSSTEP_S13_14_4   13144
#define PAYPASSSTEP_S13_42_0   13420
#define PAYPASSSTEP_S13_44_0   13440
#define PAYPASSSTEP_S13_43_0   13430
#define PAYPASSSTEP_S13_45_0   13450
#define PAYPASSSTEP_S13_32_0   13320
#define PAYPASSSTEP_S13_33_0   13330
#define PAYPASSSTEP_S13_23_0   13230
#define PAYPASSSTEP_S13_25_0   13250
#define PAYPASSSTEP_S13_26_0   13260
#define PAYPASSSTEP_S14_21_1   14211
#define PAYPASSSTEP_S14_33_0   14330
#define PAYPASSSTEP_S14_34_0   14340
#define PAYPASSSTEP_S16_E7     11670
#define PAYPASSSTEP_S10_E5_0  110050
#define PAYPASSSTEP_S10_E6_0  110060
#define PAYPASSSTEP_S10_E14_0 110140
#define PAYPASSSTEP_S10_E16_0 110160
#define PAYPASSSTEP_S10_E18_0 110180
#define PAYPASSSTEP_S10_E50_0 110500
#define PAYPASSSTEP_S10_E53_0 110530
#define PAYPASSSTEP_S9_E9_0   109090
#define PAYPASSSTEP_S9_E10_0  109100
#define PAYPASSSTEP_S9_E14_0  109140
#define PAYPASSSTEP_S9_E15_0  109150
#define PAYPASSSTEP_S9_E17_0  109170
#define PAYPASSSTEP_S10_E20_0 110200
#define PAYPASSSTEP_S10_E22_0 110220
#define PAYPASSSTEP_S10_E24_0 110240
#define PAYPASSSTEP_S9_E27_0  109270
#define PAYPASSSTEP_S11_E46_0 111460
#define PAYPASSSTEP_S11_E90_0 111900
#define PAYPASSSTEP_S11_E95_0 111950
#define PAYPASSSTEP_S11_E110_0 1111100
#define PAYPASSSTEP_S11_E112_0 1111120
#define PAYPASSSTEP_S11_E113_0 1111130
#define PAYPASSSTEP_S11_E114_1 1111141
#define PAYPASSSTEP_S11_E114_2 1111142
#define PAYPASSSTEP_S11_E114_3 1111143
#define PAYPASSSTEP_S11_E114_4 1111144
#define PAYPASSSTEP_S11_E114_5 1111145
#define PAYPASSSTEP_S11_E115_1 1111151
#define PAYPASSSTEP_S11_E115_2 1111152
#define PAYPASSSTEP_S11_E115_3 1111153
#define PAYPASSSTEP_S11_E115_4 1111154
#define PAYPASSSTEP_S11_E115_5 1111155
#define PAYPASSSTEP_S11_E115_6 1111156
#define PAYPASSSTEP_S11_E115_7 1111157
#define PAYPASSSTEP_S11_E115_8 1111158
#define PAYPASSSTEP_S11_E115_9 1111159
#define PAYPASSSTEP_S11_E119_0 1111190
#define PAYPASSSTEP_S11_E120_1 1111201
#define PAYPASSSTEP_S11_E120_2 1111202


#define PAYPASS_SAVEREQDATA_LANGPREFERENCE 1


#define BLACKLISTSM3_ENABLE_BLACKLISTENABLE  1


#endif

