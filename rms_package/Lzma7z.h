/**
 * 文 件 名 : Lzma7z.h
 * 创建日期 : 2014-4-22 11:32
 * 作    者 : 宋康
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 报文监视的压缩处理
 **/

#ifdef WIN32
	#include <Lzmalib.h>
	#pragma comment(lib,"LZMA.lib")
#endif

#include "sbase/SApi.h"

enum EEnum
{
	kEnd,

	kHeader,

	kArchiveProperties,

	kAdditionalStreamsInfo,
	kMainStreamsInfo,
	kFilesInfo,

	kPackInfo,
	kUnpackInfo,
	kSubStreamsInfo,

	kSize,
	kCRC,

	kFolder,

	kCodersUnpackSize,
	kNumUnpackStream,

	kEmptyStream,
	kEmptyFile,
	kAnti,

	kName,
	kCTime,
	kATime,
	kMTime,
	kWinAttributes,
	kComment,

	kEncodedHeader,

	kStartPos,
	kDummy
};

#define kCrcPoly		0xEDB88320
#define CRC_INIT_VAL	0xFFFFFFFF

#define CRC_UPDATE_BYTE_2(crc, b) (g_CrcTable[((crc) ^ (b)) & 0xFF] ^ ((crc) >> 8))

class CLzma7z
{
public:
	CLzma7z(void);
	~CLzma7z(void);

	bool	SetPathName(string path);
	void	SetFormat7z(bool b7z) { bFormat7z = b7z; };
	bool	LamzPcap(string ethName,const char *srcFile,char *compressFile);
	int		MakeTailHeader(string ethName,size_t srcLen,size_t destLen);
	void    WriteNumber(UINT64 value,int &pos);
	void	MakeHeader(size_t destLen);
	UINT32	CrcUpdateT4(const void *data, size_t size);
	void	genCRC32table();
	ULONG	Reflect(ULONG ref, char ch);
	UINT32	calCRC32Num(const char* fileName);

private:
	bool			bFormat7z;
	string			storePath;
	int				headerLen;
	int				tailHeaderLen;
	unsigned char	headerBuffer[64];
	unsigned char	tailHeaderBuffer[2048];

	unsigned int	crc32_table[256];
};
