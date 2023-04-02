#pragma once
#include<stdio.h>
#include <windows.h>

#define DEBUG

// PE文件信息结构体
typedef struct PE {
	LPVOID pBuffer;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeader;
	PIMAGE_FILE_HEADER pFileHeader;
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader;
	PIMAGE_SECTION_HEADER pSectionHeader;
	PIMAGE_DATA_DIRECTORY pDataDirectory;
	size_t pNumOptional;
}*pPE;

// 需注入的代码信息结构体
typedef struct infoData {
	LPVOID  addData;
	size_t sizeData;
	size_t insertNumSection;
	DWORD pMyFunction;
}*INDATA;

typedef struct addNode
{
	LPSTR lpszName;
	size_t size;
}*NODE;
LPVOID ReadPEFile(LPSTR lpszFile);
// 将PE文件读取到内存缓冲区
/*
	参数1: 要读取的PE文件路径
	返回值: 成功返回指向PE文件信息的structPE指针,失败返回NULL
*/

LPVOID CpyPEToImageBuffer(pPE FileBuffer);
// 将读取到内存中的PE文件重新分配内存空间存储拉伸成执行准备状态
/*
	参数: 内存中缓冲区中文件状态下的PE结构体(*pPE)指针
	返回值:成功返回指向已经拉升后的PE文件结构,失败返回NULL
*/

DWORD WriteImageBufferToFileBuffer(pPE ImageBuffer, LPSTR lpszFile);
// 将在内存中拉伸后的PE文件压缩并放回存储磁盘
/*
	参数1: 指向pPE结构体的指针
	参数2: PE压缩后要存入磁盘的路径
	返回值: 成功返回1,失败返回NULL
*/

DWORD WriteImageBufferCode(IN pPE ImageBuffer, IN  INDATA addData);
// 往内存缓冲区中已经展开并初始化的的pPE结构中的节插入代码 
/*
	参数1: 需要插入代码的pPE结构
	参数2: 需要插入的代码数据信息(数据地址,数据大小,第几节，注入代码要跳转的地址)
	返回值: 成功返回1,失败返回NULL
*/

DWORD  pPEAssignment(pPE pEBuffer);
// 判断当前缓冲区数据(p->pBuffer)是否是PE数据结构,并初始化pPE结构体
/*
	参数1: pPE文件结构体
	返回值:  初始化成功返回1,初始化失败返回NULL
*/

pPE pAddImageBufferNode(pPE ImageBuffer, NODE addnode);
// 将内存缓冲区已经拉伸开的pBuffer中添加新的节点并添加进节表
/*
	参数: ->pBuffer已经拉升展开并初始化的pPE结构体指针
	返回值: 添加表成功后的新缓冲区， 失败返回NULL
*/

pPE pAddNodeSectionSize(pPE ImageBuffer, size_t size);
// 扩展PE最后一个节的大小
/*
	参数1:  ->pBuffer已经拉升展开并初始化的pPE结构体指针
	参数2: 扩展的大小
	返回值: 成功返回指向新PE结构的指针, 失败返回NULL
*/

VOID pAddAllSection(pPE pImageBuffer);
// 合并所有节到一个节
/*
	参数1: 在内存中已展开的pPE结构
*/

VOID pointSection(pPE pImageBuffer);
// 打印所有数据目录
/*
	参数1: 在内存中已展开的pPE结构
*/

int Align(int x, int y);
// 根据输入的数,返回数据对齐后的大小
/*
	参数1: 要对齐的数
	参数2: 对齐的宽度
	返回值: 参数1对齐后的数
*/