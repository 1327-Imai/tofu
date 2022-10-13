#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXmath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <math.h>
#include <random>

#include "WinApp.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")

//��pixel
const size_t textureWidth = 256;
//�cpixel
const size_t textureHight = 256;
//�z��̗v�f
const size_t imageDataCount = textureWidth * textureHight;
//�摜�C���[�W�f�[�^�z��
XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];//��ŕK����������Ȃ��Ƃ����Ȃ�

//�S�s�N�Z���̐F��������
for (size_t i = 0; i < imageDataCount; i++)
{
	imageData[i].x 	= 1.0f;   //R
	imageData[i].y	= 0.0f;	  //G
	imageData[i].z	= 0.0f;	  //B
	imageData[i].w	= 1.0f;	  //A
}



// �q�[�v�ݒ�
D3D12_HEAP_PROPERTIES textureHeapProp{};
textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//// ���\�[�X�ݒ�
//D3D12_RESOURCE_DESC textureResourceDesc{};
//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; textureResourceDesc.Width = textureWidth;  // ��
//textureResourceDesc.Height = textureWidth; // ����
//textureResourceDesc.DepthOrArraySize = 1;
//textureResourceDesc.MipLevels = 1;
//textureResourceDesc.SampleDesc.Count = 1;

D3D12_RESOURCE_DESC textureResourceDesc{};
textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
textureResourceDesc.Format = metadata.format;
textureResourceDesc.Width = metadata.width;
textureResourceDesc.Height = (UINT)metadata.height;
textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
textureResourceDesc.SampleDesc.Count = 1;

D3D12_RESOURCE_DESC textureResourceDesc2{};
textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
textureResourceDesc2.Format = metadata2.format;
textureResourceDesc2.Width = metadata2.width;
textureResourceDesc2.Height = (UINT)metadata2.height;
textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
textureResourceDesc2.SampleDesc.Count = 1;