#include "HashTable.h"
#include "windows.h"

char* Utf8ToGb2312(char *p)
{
	DWORD dwNum = MultiByteToWideChar(CP_UTF8,0,p,-1,NULL,0);
	char *psText;                                                         //�洢�ļ���
	wchar_t *pwText=(wchar_t*)malloc(dwNum*sizeof(wchar_t));
	dwNum = MultiByteToWideChar(CP_UTF8,0,p,-1,pwText,dwNum);
	dwNum = WideCharToMultiByte(CP_ACP,0,pwText,-1,NULL,0,NULL,NULL);
	psText=(char*)malloc(dwNum*sizeof(char));
	dwNum = WideCharToMultiByte(CP_ACP,0,pwText,-1,psText,dwNum,NULL,NULL);
	free(pwText);
	return psText;
}

int hash(CharString str,int position)                                       //��ϣ����
{
	int hash=0;
	char a,b;
	a=str.GetAChar(position);
	position++;
	b=str.GetAChar(position);
	hash=((unsigned char)a-129)*190+((unsigned char)b-64)-(unsigned char)b/128;
	return hash;  
}

character *initdictionary(FILE *fp)                                        //�ʵ�ĳ�ʼ��
{
	character *root;                                                       //�������ڵ�
	character *currentnode;                                                //ָ��ǰ�ڵ��ָ��
	root=new character;
	root->flag=0;
	root->next=(character **)malloc(sizeof(character *)*23500);            //���ٴ洢�ռ䣬���ڹ�ϣ��
	root->length=23500;
	root->endtag=false;
	for(int i=0;i<23500;i++)
		root->next[i]=NULL;
	char *UniCodetemp;
	UniCodetemp=new char[41];
	UniCodetemp=fgets(UniCodetemp,40,fp);

	while(UniCodetemp=fgets(UniCodetemp,40,fp))                            //���дӴʿ��ж�ȡ
	{
		char *tang;
		tang=UniCodetemp;
		currentnode=root;
		char *GBtemp;
		GBtemp=Utf8ToGb2312(UniCodetemp);                                  //ת��ɹ������
		CharString word;
		int position=0;
		word=GBtemp;                                                       //�õ����ж�Ӧ�Ĵ���
		int lengthofword=0;
		while(word.GetAChar(position)!='\n'&&word.GetAChar(position)!='\0')//���������һ��һ�����ּӽ��ʵ�
		{
			lengthofword++;
			int hashcode;
			hashcode=hash(word,position);             
			CharString currentcharacter;                                   //��ǰ����
			for(int i=0;i<2;i++)                                           //��ȡ��ǰ����
			{
				currentcharacter.addChar(word.GetAChar(position));
				position++;
			}
			currentcharacter.addChar('\0');

			if(currentnode->flag==1)                                        //��ǰ�ڵ�ĺ�̽��٣���ʹ�ù�ϣ��洢ָ���̵�ָ��
			{
				currentnode=addcharacter(currentnode,currentcharacter,hashcode);
			}
			else                                                           //��ǰ�ڵ�ĺ�̽϶࣬�Ѿ����ù�ϣ��洢ָ���̵�ָ��
			{
				bool errorflag=0;
				currentnode=addcharacterlarge(currentnode,currentcharacter,hashcode,errorflag);
				if(errorflag==1)
					break;
			}
		}
		currentnode->endtag=true;                                          //һ������������ֽ����ı�־
		if(lengthofword==1)
		{
			currentnode->endtag=false;                                     //�ʵ��д��ڻ��з��Ϳո�������������������һ���ֵĴʳ���
		}
	}
	printf("The dictionary has been initialized successfully.\n");
	root->endtag=false;
	return root;
}

character *addcharacter(character *currentnode,CharString currentcharacter,int hashcode)
{
	character *temp;
	temp=new character;
	temp->a=currentcharacter.GetData();
	temp->flag=1;
	temp->length=0;
	temp->next=NULL;
	temp->hashmax=0;
	temp->endtag=false;
	if(currentnode->length==0)                                    //��ǰ�ڵ����޺��
	{
		currentnode->next=new character*[20];
		currentnode->next[0]=temp;
		currentnode->length=1;
		currentnode->hashtable=new int[20];
		currentnode->hashtable[0]=hashcode;
		currentnode->hashmax=hashcode;
		currentnode=currentnode->next[0];
	}
	else if(currentnode->length<=19)                             //��ǰ�ڵ��̽��٣������ù�ϣ��
	{
		int existflag=0;
		for(int i=0;i<currentnode->length;i++)
		{
			if(currentnode->next[i]->a==currentcharacter)        //�ú��ֶ�Ӧ��̽ڵ��Ѵ���
			{
				delete temp;
				currentnode=currentnode->next[i];
				existflag=1;
				break;
			}
		}
		if(existflag==0)                                        //�ú��ֶ�Ӧ��̽ڵ㲻����
		{
			currentnode->next[currentnode->length]=temp;        //��ָ��������ֵ�ָ�����ָ������
			currentnode->hashtable[currentnode->length]=hashcode;
			if(hashcode>currentnode->hashmax)
				currentnode->hashmax=hashcode;
			currentnode->length=currentnode->length+1;
			currentnode=currentnode->next[(currentnode->length)-1];
		}
	}
	else                                                         //�ڵ��࣬ת�����ù�ϣ��
	{
		int existflag=0;
		for(int i=0;i<currentnode->length;i++)
		{
			if(currentnode->next[i]->a==currentcharacter)        //��̽ڵ��Ѵ���
			{
				delete temp;
				currentnode=currentnode->next[i];
				existflag=1;
				break;
			}
		}
		if(existflag==0)                                         //��̽ڵ㲻����
		{
			currentnode->flag=0;
			if(hashcode>currentnode->hashmax)
				currentnode->hashmax=hashcode;
			character **tempnext;
			tempnext=(character **)malloc(sizeof(character *)*(currentnode->hashmax+100));
			for(int i=0;i<currentnode->hashmax+100;i++)
			{
				tempnext[i]=NULL;
			}

			for(int i=0;i<=19;i++)
			{
				if(tempnext[currentnode->hashtable[i]]!=NULL)
					printf("ERROR ERROE ERROE\n");
				tempnext[currentnode->hashtable[i]]=currentnode->next[i];
			}
			tempnext[hashcode]=temp;
			currentnode->length=currentnode->hashmax+100;
			delete currentnode->hashtable;
			delete currentnode->next;
			currentnode->next=NULL;
			currentnode->next=tempnext;
			currentnode=currentnode->next[hashcode];
			currentnode->length=0;
		}
	}
	return currentnode;
}

character *addcharacterlarge(character *currentnode,CharString currentcharacter,int hashcode,bool &errorflag)
{
	character *temp;
	temp=new character;
	temp->a=currentcharacter.GetData();
	temp->flag=1;
	temp->length=0;
	temp->next=NULL;
	temp->hashmax=0;
	temp->endtag=false;
	if(hashcode>=currentnode->length)                              //��ϣ���С ��Ҫ��չ
	{
		currentnode->hashmax=hashcode;
		currentnode->next=(character **)realloc(currentnode->next,sizeof(character *)*(hashcode+200));
		for(int i=currentnode->length;i<hashcode+200;i++)
			currentnode->next[i]=NULL;
		currentnode->length=hashcode+200;
		currentnode->next[hashcode]=temp;
		currentnode=currentnode->next[hashcode];
		currentnode->length=0;
	}
	else                                                           //��ϣ���㹻��ֱ�Ӽ���
	{
		if(currentnode->next[hashcode]!=NULL)                      //�ú��ֶ�Ӧ�Ľڵ��Ѵ���
		{
			if(currentnode->next[hashcode]->a==currentcharacter)   //��ͬһ������
			{
				currentnode=currentnode->next[hashcode];
			}
			else                                                   //����ͬһ�����֣�����ϣ��ͻ��������������ϲ��ᷢ��
			{                                                      //��ϣ��������Ʊ�֤��GB2312�ĺ��ֲ����ͻ����Ϊ����
				errorflag=1;
				printf("ERROR ERROR ERROR ERROR ERROR\n");
			}
		}
		else                                                       //�ú��ֶ�Ӧ�Ľڵ㲻���ڣ���Ҫ�½��ڵ㲢����ָ������
		{
			currentnode->next[hashcode]=temp;
			if(hashcode>currentnode->hashmax)
				currentnode->hashmax=hashcode;
			currentnode=currentnode->next[hashcode];
			currentnode->length=0;
		}
	}
	return currentnode;
}

void divideword(CharString *maintext,character *root,FILE *output)
{
	CharStringLink result;                             //�洢�ִʽ�����ַ�������
	int position=0;
	while(maintext->GetAChar(position)!='\0')
	{
		if(maintext->GetAChar(position)>=0&&maintext->GetAChar(position)<=127)  //�Զ�����Ӣ�ġ����֡�ASCII�ַ�
		{
			position++;
		}
		else                                            //���ֻ��ַ�
		{
			quiry(maintext,position,root,result);
		}
	}
	result.print_link(output);                          //���ļ��д�ӡ�ִʽ��
}

void quiry(CharString *maintext,int &original_position,character *root,CharStringLink &result)
{
	CharString currentword;                        //��ǰ����
	character *currentnode;                        //����ʵ����еĵ�ǰ�ڵ�
	currentnode=root;
	int endposition=0;
	int looptimes=0;
	int position=original_position;
	while(maintext->GetAChar(position)<0||maintext->GetAChar(position)>128)
	{
		looptimes++;
		CharString currentcharacter;               //�洢��ǰ����
		for(int i=0;i<2;i++)                       //��ȡ��ǰ����
		{
			currentcharacter.addChar(maintext->GetAChar(position));
			position++;
		}
		currentcharacter.addChar('\0');            //�õ���ǰ����
		int hashcode;
		hashcode=hash(currentcharacter,0);         //�Ըú������ϣֵ
		bool find=0;
		switch(currentnode->flag)                  //�жϵ�ǰ�ڵ��ָ������ʹ��˳����ǹ�ϣ��洢ָ��
		{
		case 1:                                    //˳���
			for(int i=0;i<currentnode->length;i++) //��������
			{
				if(currentnode->next[i]->a==currentcharacter)    //���ָú���
				{
					find=1;
					currentword.concat(currentword,currentcharacter);
					currentnode=currentnode->next[i];
					if(currentnode->endtag==1)
						endposition=looptimes;
					break;
				}
			}
			break;
		case 0:                                    //��ϣ��
			if(hashcode>currentnode->length||hashcode<0)   //ȷ�ϸú��ֵĹ�ϣֵ��������ϣ��Ĵ�С����ֹ����Խ�����
				break;
			else
			{
				if(currentnode->next[hashcode]!=NULL)      //���ָú���
				{
					find=1;
					currentword.concat(currentword,currentcharacter);
					currentnode=currentnode->next[hashcode];
					if(currentnode->endtag==1)
						endposition=looptimes;
				}
			}
			break;
		}
		if(find==0)
			break;
	}
	if(currentnode->endtag==0)                      //������ѯ�ֵ�
	{
		if(endposition!=0)
		{
			currentword=currentword.substring(currentword,0,2*endposition);
			original_position+=2*endposition;
			result.add(currentword);               //��ִʽ�����ַ��������м��������
		}
		else
			original_position+=2;;
	}
	else if(currentnode->endtag==1)
	{
		original_position+=2*endposition;
		result.add(currentword);
	}
}