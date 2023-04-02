#pragma once
#include<stdio.h>
#include <windows.h>

#define DEBUG

// PE�ļ���Ϣ�ṹ��
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

// ��ע��Ĵ�����Ϣ�ṹ��
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
// ��PE�ļ���ȡ���ڴ滺����
/*
	����1: Ҫ��ȡ��PE�ļ�·��
	����ֵ: �ɹ�����ָ��PE�ļ���Ϣ��structPEָ��,ʧ�ܷ���NULL
*/

LPVOID CpyPEToImageBuffer(pPE FileBuffer);
// ����ȡ���ڴ��е�PE�ļ����·����ڴ�ռ�洢�����ִ��׼��״̬
/*
	����: �ڴ��л��������ļ�״̬�µ�PE�ṹ��(*pPE)ָ��
	����ֵ:�ɹ�����ָ���Ѿ��������PE�ļ��ṹ,ʧ�ܷ���NULL
*/

DWORD WriteImageBufferToFileBuffer(pPE ImageBuffer, LPSTR lpszFile);
// �����ڴ���������PE�ļ�ѹ�����Żش洢����
/*
	����1: ָ��pPE�ṹ���ָ��
	����2: PEѹ����Ҫ������̵�·��
	����ֵ: �ɹ�����1,ʧ�ܷ���NULL
*/

DWORD WriteImageBufferCode(IN pPE ImageBuffer, IN  INDATA addData);
// ���ڴ滺�������Ѿ�չ������ʼ���ĵ�pPE�ṹ�еĽڲ������ 
/*
	����1: ��Ҫ��������pPE�ṹ
	����2: ��Ҫ����Ĵ���������Ϣ(���ݵ�ַ,���ݴ�С,�ڼ��ڣ�ע�����Ҫ��ת�ĵ�ַ)
	����ֵ: �ɹ�����1,ʧ�ܷ���NULL
*/

DWORD  pPEAssignment(pPE pEBuffer);
// �жϵ�ǰ����������(p->pBuffer)�Ƿ���PE���ݽṹ,����ʼ��pPE�ṹ��
/*
	����1: pPE�ļ��ṹ��
	����ֵ:  ��ʼ���ɹ�����1,��ʼ��ʧ�ܷ���NULL
*/

pPE pAddImageBufferNode(pPE ImageBuffer, NODE addnode);
// ���ڴ滺�����Ѿ����쿪��pBuffer������µĽڵ㲢��ӽ��ڱ�
/*
	����: ->pBuffer�Ѿ�����չ������ʼ����pPE�ṹ��ָ��
	����ֵ: ��ӱ�ɹ�����»������� ʧ�ܷ���NULL
*/

pPE pAddNodeSectionSize(pPE ImageBuffer, size_t size);
// ��չPE���һ���ڵĴ�С
/*
	����1:  ->pBuffer�Ѿ�����չ������ʼ����pPE�ṹ��ָ��
	����2: ��չ�Ĵ�С
	����ֵ: �ɹ�����ָ����PE�ṹ��ָ��, ʧ�ܷ���NULL
*/

VOID pAddAllSection(pPE pImageBuffer);
// �ϲ����нڵ�һ����
/*
	����1: ���ڴ�����չ����pPE�ṹ
*/

VOID pointSection(pPE pImageBuffer);
// ��ӡ��������Ŀ¼
/*
	����1: ���ڴ�����չ����pPE�ṹ
*/

int Align(int x, int y);
// �����������,�������ݶ����Ĵ�С
/*
	����1: Ҫ�������
	����2: ����Ŀ��
	����ֵ: ����1��������
*/