/**
 * 文 件 名 : Lzma7z.cpp
 * 创建日期 : 2014-4-22 11:32
 * 作    者 : 宋康
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 报文监视的压缩处理
 **/

#include "Lzma7z.h"

CLzma7z::CLzma7z(void)
{
	genCRC32table();
}

CLzma7z::~CLzma7z(void)
{
}

bool CLzma7z::SetPathName(string path)
{
	storePath = path;

	return true;
}

bool CLzma7z::LamzPcap(string ethName,const char *srcFile,char *compressFile)
{
	FILE *pFile = 0;
	pFile = fopen(srcFile,"r");

	fseek(pFile,0,SEEK_END);
	size_t srcLen = ftell(pFile);
	rewind(pFile);
	size_t destLen = srcLen*2;
	unsigned char *psrcRead = new unsigned char[srcLen]; //原始文件数据
	unsigned char *pLzma = new unsigned char[destLen]; //存放压缩数据

	fread(psrcRead,sizeof(char),srcLen,pFile);
	unsigned char prop[5];
	size_t sizeProp = 5;

	if (SZ_OK != LzmaCompress(pLzma,&destLen,psrcRead,srcLen,prop,&sizeProp,5,(1<<24),3,0,2,32,2))
	{
		delete psrcRead;
		delete pLzma;
		fclose(pFile);
		return false;
	}
	fclose(pFile);

	int len = MakeTailHeader(ethName,srcLen,destLen);
	MakeHeader(destLen);

	FILE *pCompressFile = 0;
	pCompressFile = fopen(compressFile,"wb");
	if (pCompressFile == NULL)
	{
		delete psrcRead;
		delete pLzma;
		fclose(pCompressFile);
		return false;
	}
	if (bFormat7z)
		fwrite(headerBuffer,sizeof(char),headerLen,pCompressFile);
	fwrite(pLzma,sizeof(char),destLen,pCompressFile);
	if (bFormat7z)
		fwrite(tailHeaderBuffer,sizeof(char),tailHeaderLen,pCompressFile);

	fclose(pCompressFile);

	delete psrcRead;
	delete pLzma;

	::remove(srcFile);

	return true;
}

void CLzma7z::WriteNumber(UINT64 value,int &pos)
{
	unsigned char firstByte = 0;
	unsigned char mask = 0x80;
	int i;
	for (i = 0; i < 8; i++)
	{
		if (value < ((UINT64(1) << ( 7  * (i + 1)))))
		{
			firstByte |= unsigned char(value >> (8 * i));
			break;
		}
		firstByte |= mask;
		mask >>= 1;
	}
	tailHeaderBuffer[pos++] = firstByte;
	for (;i > 0; i--)
	{
		tailHeaderBuffer[pos++] = (unsigned char)value;
		value >>= 8;
	}
}

int CLzma7z::MakeTailHeader(string ethName,size_t srcLen,size_t destLen)
{
	unsigned char unkown[11] = {0x01,0x23,0x03,0x01,0x01,0x05,0x5d,0x00,0x00,0x01,0x00};
	unsigned char unkown1[4] = {0x20,0x00,0x00,0x00};
	int	pos = 0;

	tailHeaderBuffer[pos++] = kHeader;
	tailHeaderBuffer[pos++] = kMainStreamsInfo;

	tailHeaderBuffer[pos++] = kPackInfo;
	tailHeaderBuffer[pos++] = 0;
	tailHeaderBuffer[pos++] = 1;
	tailHeaderBuffer[pos++] = kSize;
	WriteNumber(destLen,pos);
	tailHeaderBuffer[pos++] = kEnd;

	tailHeaderBuffer[pos++] = kUnpackInfo;
	tailHeaderBuffer[pos++] = kFolder;
	tailHeaderBuffer[pos++] = 1;
	tailHeaderBuffer[pos++] = 0;
	memcpy(&tailHeaderBuffer[pos],unkown,11);
	pos += 11;
	tailHeaderBuffer[pos++] = kCodersUnpackSize;
	WriteNumber(srcLen,pos);
	tailHeaderBuffer[pos++] = kEnd;

	tailHeaderBuffer[pos++] = kSubStreamsInfo;
	tailHeaderBuffer[pos++] = kCRC;
	tailHeaderBuffer[pos++] = 1;
	string  path = storePath + ethName + ".pcap";
	UINT32 v = calCRC32Num(path.c_str());
	memcpy(&tailHeaderBuffer[pos],&v,sizeof(UINT32));	//CRC+
	pos += sizeof(UINT32);
	tailHeaderBuffer[pos++] = kEnd;
	tailHeaderBuffer[pos++] = kEnd;

	tailHeaderBuffer[pos++] = kFilesInfo;
	tailHeaderBuffer[pos++] = 1;
	tailHeaderBuffer[pos++] = kName;
	string file = ethName + ".pcap";
	int len = file.length();
	len = len * 2 + 3;
	tailHeaderBuffer[pos++] = len;
	tailHeaderBuffer[pos++] = 0;
	for (size_t t = 0; t <= file.length(); t++)
	{
		wchar_t c = file[t];
		tailHeaderBuffer[pos++] = (unsigned char)c;
		tailHeaderBuffer[pos++] = (unsigned char)(c >> 8);
	}
	tailHeaderBuffer[pos++] = kMTime;
	tailHeaderBuffer[pos++] = 10;
	tailHeaderBuffer[pos++] = 1;
	tailHeaderBuffer[pos++] = 0;
	SYSTEMTIME	st;
	FILETIME	ft;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	memcpy(&tailHeaderBuffer[pos],&ft.dwLowDateTime,4);
	pos += 4;
	memcpy(&tailHeaderBuffer[pos],&ft.dwHighDateTime,4);
	pos += 4;
	tailHeaderBuffer[pos++] = kWinAttributes;
	tailHeaderBuffer[pos++] = 6;
	tailHeaderBuffer[pos++] = 1;
	tailHeaderBuffer[pos++] = 0;
	memcpy(&tailHeaderBuffer[pos],unkown1,4);
	pos += 4;
	tailHeaderBuffer[pos++] = kEnd;
	tailHeaderBuffer[pos++] = kEnd;

	tailHeaderLen = pos;

	return tailHeaderLen;
}

void CLzma7z::MakeHeader(size_t destLen)
{
	unsigned char kSignature[6] = {'7', 'z', 0xBC, 0xAF, 0x27, 0x1C};
	int pos = 0;

	memcpy(&headerBuffer[pos],kSignature,6);
	pos += 6;
	headerBuffer[pos++] = 0;
	headerBuffer[pos++] = 3;
	memset(&headerBuffer[pos],0,4);	//empty crc
	pos += 4;
	UINT64 tailHeaderPostion = destLen;
	memcpy(&headerBuffer[pos],&tailHeaderPostion,8);
	pos += 8;
	UINT64 tailHeaderLen64 = tailHeaderLen;
	memcpy(&headerBuffer[pos],&tailHeaderLen64,8);
	pos += 8;
	UINT32 v = CrcUpdateT4(tailHeaderBuffer,tailHeaderLen);
	memcpy(&headerBuffer[pos],&v,sizeof(UINT32));	//CRC+
	pos += sizeof(UINT32);

	v = CrcUpdateT4(&headerBuffer[12],20);
	memcpy(&headerBuffer[8],&v,sizeof(UINT32));	//CRC+

	headerLen = pos;
}

UINT32 CLzma7z::CrcUpdateT4(const void *data, size_t size)
{
	UInt32 g_CrcTable[256 * 8];
	UINT32 v = CRC_INIT_VAL;

	UInt32 i;
	for (i = 0; i < 256; i++)
	{
		UInt32 r = i;
		unsigned j;
		for (j = 0; j < 8; j++)
			r = (r >> 1) ^ (kCrcPoly & ~((r & 1) - 1));
		g_CrcTable[i] = r;
	}
	for (; i < 256 * 8; i++)
	{
		UInt32 r = g_CrcTable[i - 256];
		g_CrcTable[i] = g_CrcTable[r & 0xFF] ^ (r >> 8);
	}

	const Byte *p = (const Byte *)data;
	for (; size > 0 && ((unsigned)(ptrdiff_t)p & 3) != 0; size--, p++)
		v = CRC_UPDATE_BYTE_2(v, *p);
	for (; size >= 4; size -= 4, p += 4)
	{
		v ^= *(const UInt32 *)p;
		v =
			g_CrcTable[0x300 + (v & 0xFF)] ^
			g_CrcTable[0x200 + ((v >> 8) & 0xFF)] ^
			g_CrcTable[0x100 + ((v >> 16) & 0xFF)] ^
			g_CrcTable[0x000 + ((v >> 24))];
	}
	for (; size > 0; size--, p++)
		v = CRC_UPDATE_BYTE_2(v, *p);
	v = v ^ CRC_INIT_VAL;
	return v;
}

void CLzma7z::genCRC32table()
{
	ULONG ulPolynomial = 0x04c11db7;

	for(int i = 0; i <= 0xFF; i++)
	{
		crc32_table[i]=Reflect(i, 8) << 24;
		for (int j = 0; j < 8; j++)
			crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
		crc32_table[i] = Reflect(crc32_table[i], 32);
	}
}

ULONG CLzma7z::Reflect(ULONG ref, char ch)
{
	ULONG value(0);

	for(int i = 1; i < (ch + 1); i++)
	{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}

UINT32 CLzma7z::calCRC32Num(const char* fileName)
{
    FILE *pFile = fopen(fileName,"r");

	fseek(pFile,0,SEEK_END);
	size_t srcLen = ftell(pFile);
	rewind(pFile);
	unsigned char *psrcRead = new unsigned char[srcLen]; //原始文件数据

	fread(psrcRead,sizeof(char),srcLen,pFile);
	fclose(pFile);

    ULONG  crc(0xffffffff);
	int len;
	unsigned char* buffer;

	len = srcLen;
	// Save the text in the buffer.
	buffer = psrcRead;
	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(len--)
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];
	// Exclusive OR the result with the beginning value.
	return crc^0xffffffff;
}
