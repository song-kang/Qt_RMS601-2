/**
 *
 * 文 件 名 : EscPrint.cpp
 * 创建日期 : 2012-10-17 13:12
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : ESC(EPSON Standard Code for Printer)命令集打印基类
 * 修改记录 : 
 *            $Log: $
 *
 **/

#include "SEscPrint.h"

#define ESC 0x1B 
#define FF 0x0C  
static unsigned char LQPrintData[][10]={  
	{ESC,'@'},{2},/*打印机初始化*/  
	{ESC,'*'},{2},/*选择图像模式 m n1 n2*/ 
	{ESC,'3'},{2},/*设定n/180英寸行间距*/ 
	{ESC,'2'},{2},/*设定1/6英寸行间距*/ 
		};  

SEscPrint::SEscPrint()
{

}

SEscPrint::~SEscPrint()
{

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打开打印机
// 作    者:  邵凯田
// 创建时间:  2012-10-17 13:23
// 参数说明:  @sAttr为打印机参数
// 返 回 值:  true打开成功，false打开失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::Open(SString sAttr)
{
	BYTE buf[]={ESC,'@'};//打印机初始化
	if(Write(buf,sizeof(buf)) != sizeof(buf))
	{
		LOGERROR("打印机初始化指令发送失败!");
		Close();
		return false;
	}
	SwitchTextMode();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  选择文本打印模式
// 作    者:  邵凯田
// 创建时间:  2012-10-17 15:34
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::SwitchTextMode()
{
	BYTE buf[]={ESC,'3',0x20};//间距0x20
	if(Write(buf,sizeof(buf)) != sizeof(buf))//行距
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  选择图形打印模式
// 作    者:  邵凯田
// 创建时间:  2012-10-17 15:35
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::SwitchGraphicMode()
{
	BYTE buf[]={ESC,'3',0x18};//间距0x18
	if(Write(buf,sizeof(buf)) != sizeof(buf))//行距
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印文本
// 作    者:  邵凯田
// 创建时间:  2012-10-17 15:41
// 参数说明:  @str为文本内容，\n为换行，换行时不需要\r符
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintText(SString str)
{
	if(Write((BYTE*)str.data(),str.length()) != str.length())//行距
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印指定的BMP文件，该文件必须为1位颜色的黑白图片
// 作    者:  邵凯田
// 创建时间:  2012-10-17 18:13
// 参数说明:  @sBmpFile为文件名全路径
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintBmpFile(SString sBmpFile)
{
	SFile file(sBmpFile);
	if(!file.open(IO_ReadOnly))
	{
		LOGWARN("打开BMP文件(%s)失败!",sBmpFile.data());
		return false;
	}
	stuBitmapFileHeader fileHeader;
	stuBitmapInfo info;
	int filesize = file.size();
	file.seekBegin();
	int ret,w,h,datasize;
	ret = file.readBlock((BYTE*)&fileHeader,sizeof(fileHeader));
	SKT_SWAP_WORD(fileHeader.bfType);
	SKT_SWAP_WORD(fileHeader.bfSize);
	SKT_SWAP_WORD(fileHeader.bfOffBits);
	if(ret != sizeof(fileHeader) || fileHeader.bfType != 0x4d42 || fileHeader.bfSize != filesize )//BM
	{
		LOGWARN("BMP文件(%s)格式无效!",sBmpFile.data());
		file.close();
		return false;
	}
	ret = file.readBlock((BYTE*)&info,sizeof(info));
	if(ret != sizeof(info))
	{
		LOGWARN("BMP文件(%s)格式无效!",sBmpFile.data());
		file.close();
		return false;
	}
	SKT_SWAP_WORD(info.bmiHeader.biBitCount);
	SKT_SWAP_DWORD(info.bmiHeader.biWidth);
	SKT_SWAP_DWORD(info.bmiHeader.biHeight);
	SKT_SWAP_DWORD(info.bmiHeader.biSize);
	if(info.bmiHeader.biBitCount != 1)
	{
		LOGWARN("BMP文件(%s)不是单色图片,不能打印!",sBmpFile.data());
		file.close();
		return false;
	}
	w = info.bmiHeader.biWidth;
	h = info.bmiHeader.biHeight;
	datasize = w*h/8;
	if((w*h)%8 != 0)
		datasize ++;
	if(datasize == 0)
	{
		LOGWARN("BMP文件(%s)大小为0!",sBmpFile.data());
		file.close();
		return false;
	}
	BYTE *pBuffer = new BYTE[datasize];
	file.seekTo(fileHeader.bfOffBits);//跳到数据区开始位置
	ret = file.readBlock(pBuffer,datasize);
	if(ret != datasize)
	{
		delete[] pBuffer;
		LOGWARN("BMP文件(%s)格式无效!",sBmpFile.data());
		file.close();
		return false;
	}
	if(!PrintMonochromeBmpMem(w,h,pBuffer,datasize))
	{
		delete[] pBuffer;
		LOGWARN("BMP文件(%s)打印失败!",sBmpFile.data());
		file.close();
		return false;
	}
	
	delete[] pBuffer;
	file.close();
	LOGDEBUG("BMP文件(%s)打印完毕!",sBmpFile.data());
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印指针大小及缓冲区的单色图片
// 作    者:  邵凯田
// 创建时间:  2012-10-17 18:32
// 参数说明:  @w为图片宽度
//            @h为图片高度
//            @pBuffer为图片内容缓冲区指针
//            @iBytes为图片内容缓冲区字节数
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintMonochromeBmpMem(int w,int h,BYTE *pBuffer,int iBytes)
{
	/*
	紧跟在彩色表之后的是图像数据字节阵列。图像的每一扫描行由表示图像像素的连续的字节组成，
	每一行的字节数取决于图像的颜色数目和用像素表示的图像宽度。扫描行是由底向上存储的，
	这就是说，阵列中的第一个字节表示位图左下角的像素，而最后一个字节表示位图右上角的像素。
	*/
	//一次打印一行，每行的每一列均由一个24位(24针打印机)的3字节组成，每行即w*3个字节
	int rowsize = 6+w*3;
	BYTE *pRowData = new BYTE[rowsize];
	pRowData[0] = 0x1b;
	pRowData[1] = 0x2a;
	pRowData[2] = 0x21;
	pRowData[3] = (BYTE)(w&0xff);
	pRowData[4] = (BYTE)(w>>8);
	pRowData[rowsize-1] = 0x0A;//换行符
	int row,rows = h/24;
	int col;
	int p,ps;
	int w_d8=w/8;
	if(h%24 != 0)
		rows++;
	BYTE *pPixel;
	SwitchGraphicMode();
	PrintLF();
	for(row=0;row<rows;row++)//24点组成的行
	{
		memset(pRowData+5,0,rowsize-6);//清空行缓存
		ps = 24;//一行由24个像素组成
		if(row == rows-1)
		{
			//最后一行可以不足24个像素
			ps = (h-row*24);
		}
		BYTE *pRow = &pBuffer[(w*h-w*row*24-w)/8];//24点行的最后一个像素行对应的起始地址,对应当前行第一个像素行
		BYTE *pMatrix=new BYTE[24*w];
		memset(pMatrix,0,w*24);
		for(p=0;p<ps;p++)//一次打印行最多由24点像素组成
		{
			for(col=0;col<w;col++)//每一列
			{
				if(col == 15)
					p=p;
				pPixel = pRowData+5+col*3+(p>>3);//启始位置
			
				//LOGDEBUG("p=%d,col=%d,pixel=%d, v=%d",p,col,(pPixel-pRowData)-5,((pRow[col>>3] >> (7-col&7)) & 1));
				if((((pRow[col>>3] >> (7-col&7)) & 1) << (7-col&7)) == 0)
				{
					pMatrix[p*w+col] = 1;
					*pPixel |= 1 << (7-(p&7));//(((pRow[col>>3] >> (7-col&7)) & 1) << (7-col&7));
				}
			}
			pRow -= w_d8;//到下一行
		}
		//printf("\n");
		/*
		for(p=0;p<24;p++)
		{
			printf("%02d:    ",p);
			for(col=0;col<w;col++)
			{
				if(pMatrix[p*w+col] != 0)
					printf("█");
				else
					printf("□");
			}
			printf("\n");
		}
		*/
		//printf("\n");
			
	//	LOGDEBUG("WRITE(len:%d):%s",rowsize,SString::HexToStr(pRowData,rowsize).data());

//		PrintText("打印：");
		Write(pRowData,rowsize);
	}

	delete[] pRowData;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印一个图形行
// 作    者:  邵凯田
// 创建时间:  2012-10-18 20:44
// 参数说明:  @w为列数
//            @pBuffer为图形缓冲区指针，每列24行(点)组成，共3个字节表示一个点
//            @iBytes为缓冲区字节数，必须为w的3倍
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintGraphicLine(int w,BYTE *pBuffer,int iBytes)
{
	BYTE buf[]={0x1b,0x2a,0x21,0,0};
	buf[3] = (BYTE)(w&0xff);
	buf[4] = (BYTE)(w>>8);
	if(iBytes != w*3)
	{
		LOGWARN("缓冲区大小与列数不匹配!");
		return false;
	}
	if(Write(buf,sizeof(buf)) != sizeof(buf))
	{
		LOGWARN("写打印数据失败!");
		return false;
	}
	Write(pBuffer,iBytes);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印换行符
// 作    者:  邵凯田
// 创建时间:  2012-10-17 16:37
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintLF()
{
	Write((BYTE*)"\n",1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印回车符
// 作    者:  邵凯田
// 创建时间:  2012-10-17 16:37
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintCR()
{
	Write((BYTE*)"\r",1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印并走纸到下页首,将打印缓冲区中的数据全部打印出来并返回标准模式。
// 作    者:  邵凯田
// 创建时间:  2012-10-17 16:43
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::MoveToNextPage()
{
	BYTE buf[]={0x0c};
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印并走纸到下页首,将打印缓冲区中的数据全部打印出来并返回标准模式。
// 作    者:  邵凯田
// 创建时间:  2012-10-17 16:43
// 参数说明:  @points为垂直点数，一个垂直点距为0.33mm
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::MovePaperPoints(BYTE points)
{
	BYTE buf[]={ESC,'J',0};
	buf[2] = (BYTE)points;
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	return true;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  打印并进纸lines 行,打印打印缓冲区中的数据并进纸lines字符行
// 作    者:  邵凯田
// 创建时间:  2012-10-17 16:43
// 参数说明:  @lines为进纸的行数
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::MovePaperLines(BYTE lines)
{
	BYTE buf[]={ESC,'d',0};
	buf[2] = (BYTE)lines;
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  检查是否缺纸
// 作    者:  邵凯田
// 创建时间:  2012-10-24 9:34
// 参数说明:  void
// 返 回 值:  true表示不缺纸，false表示缺纸
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::CheckPaper()
{
	BYTE buf[]={16,4,1};
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	Read(buf,1);
	return true;

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  发出蜂鸣声
// 作    者:  邵凯田
// 创建时间:  2012-10-24 10:14
// 参数说明:  @iTimes表示蜂鸣次数
//            @iSlp表示两次蜂鸣间的间隔时间，单位毫秒
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SEscPrint::Beep(int iTimes/*=1*/,int iSlp/*=0*/)
{
	BYTE buf[]={7};
	for(int i=0;i<iTimes;i++)
	{
		if(Write(buf,sizeof(buf)) != sizeof(buf))
			return;
		if(i<iTimes-1)
			SApi::UsSleep(iSlp*1000);
	}
}
