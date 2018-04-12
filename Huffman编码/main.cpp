#include<iostream>
#include<iomanip>
#include<fstream>  
#include<stdlib.h>
#include<conio.h>
#include<string>
#include<bitset>
#include"Huffman.h"
#include"externHuffman.h"

//全局变量
long g_count = 0; //计算压缩后的bit数

//字符宽度控制
#define WIDTH 10

//作者：周秦春
//存储字符及其出现频率
struct CharWithWight
{

	char ch;
	int weight = 0;
};
//作者：周秦春
//二进制写入的缓冲区
struct BinaryBuffer
{
	char ch = 0;
	unsigned int bit = 0;
	bool endif = false;
};

//作者：周秦春
//存储字符及其编码
struct CharWithCode
{
	char ch;
	string code;
};


//打印菜单
void menu();
//读取文件
bool Stat(CharWithWight* chars, int& num, char *fileName);
//第一个功能
void readFileGetHuffCode();
//压缩文件
void ziped();
void codeHelp(Code<char> code, char *fileName);
//二进制写入
void writeInBinary(BinaryBuffer &buffer, string code, ofstream &pFile);
//解压文件
void unZip();
//将该二进制文件转换为二进制字符串
void readToCode(string &code, ifstream &rfile);
//借助二进制字符串和字符与二进制对照表进行解压
void unzipHelp(string &code, CharWithCode *charCodes, ofstream &wfile,int num,long number);

int main() 
{
	menu();
	return 0;
}

//测试通过 周秦春
//作者：周秦春
//日期：2016/11/26/17:34
//程序主框架，打印菜单与选择
void menu() 
{
	do
	{
		system("cls");
		cout << "                          Huffman编码器                             " << endl;
		cout << endl;
		cout << endl;
		cout << endl;

		cout << "操作命令说明：" << endl;
		cout << "统计输入文件字符频度并对字符集编括码并输出至文件（基本要求）： 1" << endl;
		cout << "  对整个文件编括码并保存编码结果到一个二进制文件（中级要求）： 2" << endl;
		cout << "            文件解码并将解码结果保存为一文本文件（高级要求）： 3" << endl;
		cout << "                                                       退出 ： 4" << endl;
		cout << "请输入你的选择：" << endl;

		char choice = 0;

		do
		{
			cin.get(choice);
			
			while (cin.get()!='\n');

			if (choice != '4'&&choice != '1'&&choice != '2'&&choice != '3')
			{
				cout << "请重新输入！" << endl;
			}
		} while (choice != '4'&&choice != '1'&&choice != '2'&&choice != '3');

		switch (choice)
		{
		case '1':
			readFileGetHuffCode();
			break;
		case '2':
			ziped();
			break;
		case '3':
			unZip();
			break;
		default:
			cout << "谢谢使用!" << endl;
			exit(0);
		}

		cout << "按任意键继续...";
		getch();
	} while (true);
}

//测试通过 周秦春
//作者：周秦春
//日期：2016/11/29/20:18
//读入文本文件并统计频度不为零的字符集s，相应频度集 w，及个数num
bool Stat(CharWithWight* chars, int& num , char *fileName)
{
	
	ifstream fRfile(fileName, ifstream::in);
	if (!fRfile)
	{
		cout << "打开文件失败！" << endl;
		return false;
	}

	CharWithWight *charWithWights = new CharWithWight[300];
	char oneCh;
	
	bool *exit = new bool[300];
	memset(exit, 0, 300);
	num = 0;
	
	while ((oneCh = fRfile.get()) != EOF)
	{
		charWithWights[oneCh].ch = oneCh;
		charWithWights[oneCh].weight++;
		
		//计算有多少种字符
		if (!exit[oneCh])
		{
			exit[oneCh] = 1;
			num++;
		}
	}

	int count = 0;
	for(int j = 0; j < 300; j++)
	{
		if (0 != charWithWights[j].weight)
		{
			memcpy(&chars[count], &charWithWights[j], sizeof(CharWithWight));
			count++;
		}
	}
	
	
	delete exit;
	delete charWithWights;
	fRfile.close();

	return true;
}

//测试通过 周秦春
//作者：周秦春
//日期：2016/11/29/20：38
//从文件中读取字符，并将其进行Huffman编码并输出
void readFileGetHuffCode()
{
	int num = 0;
	CharWithWight chars[256];

	char fileName[200];
	cout << "请输入要读的文件名！" << endl;
	cin.getline(fileName, 199);
	cin.clear();

	//int &a = num;
	if (!Stat(chars,num,fileName))
	{
		return;
	}

	//输出字符频度
	cout << "字符数统计：" << endl;
	for (int i = 0; i < num; i++)
	{
		switch (chars[i].ch)
		{
		case ' ':
			cout <<setw(WIDTH)<< "空格" << chars[i].weight;
			break;
		case '\n':
			cout <<setw(WIDTH)<< "回车" << chars[i].weight;
			break;
		case '\t':
			cout <<setw(WIDTH)<< "制表符" << chars[i].weight;
			break;
		default:
			cout << setw(WIDTH)<< chars[i].ch << chars[i].weight;
			break;
		}

		//格式对齐
		if (i != 0 && 0 == i % 3)
		{
			cout << endl;
		}
	}

	cout << endl;

	//Huffman数组
	HuffTree<char>	**treeArray = new HuffTree<char>*[num];
	for (int i = 0; i < num; i++)
	{
		treeArray[i] = new HuffTree<char>(chars[i].ch,chars[i].weight);
	}
	HuffTree<char> *codeTree = buildTree<char>(treeArray, num);

	//Huffman编码
	Code<char> code(num);
	huffmanCode<char>(codeTree, num, code);

	//输出编码
	cout << "编码：" << endl;
	for (int i = 0; i < num; i++)
	{
		switch (code.vals[i])
		{
		case ' ':
			cout <<setw(WIDTH)<< "空格" << code.outcome[i];
			break;
		case '\n':
			cout << setw(WIDTH)<<"回车" << code.outcome[i];
			break;
		case '\t':
			cout <<setw(WIDTH)<< "制表符" << code.outcome[i];
			break;
		default:
			cout << setw(WIDTH)<<code.vals[i]<< code.outcome[i];
			break;
		}
		

		//格式对齐
		if (i!=0&&0==i%3)
		{
			cout << endl;
		}
	}
	
	cout << endl;
	delete treeArray;
	return;
}

//测试通过 周秦春
//作者：周秦春
//日期：2016/12/2/21:01
//压缩文件
void ziped()
{
	char fileName[200];
	cout << "请输入压缩的文件名！" << endl;
	cin.getline(fileName, 199);
	cin.clear();

	//统计字符
	int num = 0;
	CharWithWight *chars = new CharWithWight[256];
	if (!Stat(chars,num, fileName))
	{
		return;//字符统计失败
	}
	
	//Huffman数组
	HuffTree<char>	**treeArray = new HuffTree<char>*[num];
	for (int i = 0; i < num; i++)
	{
		treeArray[i] = new HuffTree<char>(chars[i].ch, chars[i].weight);
	}
	HuffTree<char> *codeTree = buildTree<char>(treeArray, num);

	//Huffman编码
	Code<char> code(num);
	huffmanCode<char>(codeTree, num, code);

	
	//开始压缩，写入文件
	codeHelp(code, fileName);

	//释放空间
	delete chars;
	delete treeArray;
}

//测试通过 周秦春
//作者：周秦春
//日期：2016/12/2/21:01
//传入编码方式和要压缩的文件，开始压缩
void codeHelp(Code<char> code, char * fileName)
{
	int zipCount = 0;

	string outFile(fileName);
	int posOfPoint = 0;
	posOfPoint = outFile.find_last_of('.', outFile.size());
	outFile.replace(posOfPoint, outFile.size() - posOfPoint, ".bin");

	//打开二进制文件
	ofstream sWrite(outFile, ios::binary);
	if (!sWrite)
	{
		cout << "写入文件失败！" << endl;
		return;
	}

	////配置文件
	string configFile(fileName);
	configFile.replace(posOfPoint, outFile.size() - posOfPoint, ".cfg");
	//打开配置文件
	ofstream fconfig(configFile, ios::out);
	if (!fconfig)
	{
		cout << "写入配置文件失败！" << endl;
		return;
	}
	//将编码存入数组，并写入配置文件，用于解压
	string *codes = new string[256];
	for (int i = 0; i < code.length; i++)
	{
		//字符
		char tempCh = code.vals[i];
		
		string tempS(code.outcome[i]);//字符对应编码
		
		codes[tempCh] = tempS;

		//开始写入配置文件,用于解压
		fconfig.put(tempCh);
		fconfig.write(tempS.data(),tempS.size());
		fconfig.put('\n');
	}


	//开始进行文件写入
	g_count = 0;//初始化压缩用的字符数

	BinaryBuffer buffer;//字符缓冲区
	fstream fRFile(fileName, ios::in);
	char temp = fRFile.get();//存储下一个要处理的字符
	string ccode;//要处理的字符的编码
	while (EOF != temp)
	{
		zipCount++;//计算字符数
		//cout << temp << ' ';
		ccode = codes[temp];
		//cout << ccode << endl;
		temp = fRFile.get();
		if (EOF == temp)
		{
			buffer.endif = true;
		}

		writeInBinary(buffer,ccode, sWrite);
	}
	//总共有多少字符,写入配置文件
	fconfig.put('\t');
	fconfig << zipCount;
	
	


	//计算压缩率。
	double zPercentage = (zipCount-g_count)/ (zipCount + 0.0);
	int percentage = (int)(zPercentage * 100000);
	if ((percentage%10)>=5)
	{
		percentage = percentage / 10 + 1;
	}
	else
	{
		percentage = percentage / 10;
	}

	

	cout << "压缩前的字节:" << zipCount << endl;
	cout << "压缩后的字节:" << g_count << endl;
	cout << "压缩率为" << percentage / 100.0 << "%" << endl;

	//释放资源
	sWrite.close();
	fRFile.close();
	fconfig.close();
	delete []codes;
	return;
}

//测试通过 周秦春
//作者：周秦春
//2016/12/2/16:44	
//将二进制字符串按照二进制写入文件
void writeInBinary(BinaryBuffer &buffer,string code,ofstream &pFile)
{
	for (size_t i = 0; i < code.size(); i++)
	{
		buffer.ch =  buffer.ch << 1;
		if (code[i]=='1')
		{
			buffer.ch += 1;
		}
		buffer.bit++;

		//达到一个字节写入内存
		if (8==buffer.bit)
		{
			pFile.put(buffer.ch);
			buffer.ch = 0;
			buffer.bit = 0;
			buffer.endif = false;

			g_count++;//计算压缩后的字节数
		}

	}

	//只剩最后一个编码，强制写入
	if (buffer.endif)
	{
		buffer.ch = buffer.ch << (8 - buffer.bit);
		pFile.put(buffer.ch);
		buffer.ch = 0;
		buffer.bit = 0;
		buffer.endif = false;

		
		g_count++;//计算压缩后的字节数
	}


	return;
}

//解压文件，将压缩好的文件解压
void unZip()
{
	string *codes = new string[256];

	////文件准备
	char fileName[200];
	cout << "请输入压缩的文件名！" << endl;
	cin.getline(fileName, 199);
	cin.clear();

	//打开解压文件
	ofstream fwritefile(fileName, ios::out);
	if (!fwritefile)
	{
		cout << "写入文件失败！" << endl;
		return;
	}

	//打开二进制文件
	string inFile(fileName);
	int posOfPoint = 0;
	posOfPoint = inFile.find_last_of('.', inFile.size());
	inFile.replace(posOfPoint, inFile.size() - posOfPoint, ".bin");

	ifstream sRrite(inFile, ios::binary);
	if (!sRrite)
	{
		cout << "读取文件失败！" << endl;
		return;
	}

	////配置文件
	string configFile(fileName);
	configFile.replace(posOfPoint, inFile.size() - posOfPoint, ".cfg");
	//打开配置文件
	fstream fconfig(configFile, ios::in);
	if (!fconfig)
	{
		cout << "读取配置文件失败！" << endl;
		return;
	}

	char tempCh = 0;
	char tempChs[81];
	int num = 0;//统计共有多少种字符
	long number = 0;//原来总共有多少字符
	CharWithCode * charcodes = new CharWithCode[256];
	while (EOF!=(tempCh=fconfig.get())&&!(fconfig.eof()))
	{
		if ('\t'==tempCh)
		{
			fconfig >> number;
			//cout << number << endl;
			continue;
		}

		fconfig.getline(tempChs, 80);
		//字符编码结构数组
		charcodes[num].ch = tempCh;
		charcodes[num].code = tempChs;
		//cout << tempChs << '\t';
		num++;
	}
	
	//将二进制文件转换为一个string字符串
	string code;
	readToCode(code, sRrite);
	
	//解压
	unzipHelp(code, charcodes, fwritefile, num,number);


	//资源释放
	sRrite.close();
	fconfig.close();
	fwritefile.close();
	delete[] charcodes;
	delete[] codes;
	return;
}

//测试通过 周秦春
//作者：周秦春
//日期：2016/12/2/23:13
//从二进制文件中读入内容并转换为二进制字符串
void readToCode(string &code, ifstream &rfile)
{
	int tempInt;//临时整数
	while (EOF!=(tempInt = rfile.get()))
	{
		//cout << tempInt << endl;
		bitset<8> bits(tempInt);

		code += bits.to_string();
	}
}

//借助二进制字符串和字符与二进制对照表进行解压
void unzipHelp(string &code, CharWithCode *charCodes,ofstream &wfile,int num,long number)
{

	unsigned long local = 0;//字符串中的位置
	int size; //编码二进制字符的长度
	CharWithCode temp;//编码二进制字符
	while (number>0)
	{
		for (int i = 0; i < num; i++)
		{
			temp = charCodes[i];
			size = temp.code.size();
			
			if (code.compare(local,size,temp.code)==0)
			{
				wfile.put(temp.ch);
				break;
			}
		}
		local += size;
		number--;//number计算源文件的字符数
	}
	
	if (0 == number)
	{
		cout << '\t' << "解压成功..." << endl;
	}
	return;
}