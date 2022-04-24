#include "QRCodeGenerator.h"

#if PLATFORM_WINDOWS || PLATFORM_MAC
#include "qrcodegen.hpp"

#include <string>
#include <fstream>

using namespace std;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
#endif

#define OUT_FILE_PIXEL_PRESCALER  8

#define PIXEL_COLOR_R 0
#define PIXEL_COLOR_G 0
#define PIXEL_COLOR_B 0
#define PIXEL_COLOR_A 255

//resolve behind's question£º error C4103: alignment changed after including header, may be due to missing #pragma pack(pop)
//#pragma warning(disable:4103)
#pragma pack(push, 2) //2 byte align, not will be have question

UTexture2D* UQRCodeGenerator::GenerateQRCode(FString code)
{
	UTexture2D* MyTexture = NULL;

#if PLATFORM_WINDOWS || PLATFORM_MAC
	std::string MyStdString(TCHAR_TO_UTF8(*code));
	const char* szSourceSring = MyStdString.c_str();    //QRCODE_TEXT
	unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char* pRGBData, * pDestData;

	// Compute QRCode
	QrCode pQRC = QrCode::encodeText(szSourceSring, QrCode::Ecc::MEDIUM);;


	//pQRC = QRcode_encodeString(szSourceSring, 2, QR_ECLEVEL_H, QR_MODE_8, 1);
	if (true)
	{
		unWidth = pQRC.getSize();
		unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 4; //Number of pixels per dimension (8), 3 bytes per pixel
		/*if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;*/
		unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

		// Allocate pixels buffer
		pRGBData = (unsigned char*)malloc(unDataBytes);
		if (!pRGBData)
		{
			printf("Out of memory");
			exit(-1);
		}

		// Preset to white
		memset(pRGBData, 0xff, unDataBytes);

		// Convert QrCode bits to bmp pixels
		//pSourceData = pQRC->data;
		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
			for (x = 0; x < unWidth; x++)
			{
				//if (*pSourceData & 1)
				if (pQRC.getModule(x, y))
				{
					for (l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
					{
						for (n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
						{
							*(pDestData + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_B;
							*(pDestData + 1 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_G;
							*(pDestData + 2 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_R;
							*(pDestData + 3 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_A;
						}
					}
				}
				pDestData += 4 * OUT_FILE_PIXEL_PRESCALER;
				//pSourceData++;
			}
		}


		MyTexture = UTexture2D::CreateTransient(unWidth * OUT_FILE_PIXEL_PRESCALER, unWidth * OUT_FILE_PIXEL_PRESCALER);
		//MyTexture->SRGB = true;

#if ENGINE_MAJOR_VERSION < 5
		FTexture2DMipMap& Mip = MyTexture->PlatformData->Mips[0];
#else
		FTexture2DMipMap& Mip = MyTexture->GetPlatformData()->Mips[0];
#endif
		void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);

		//FMemory::Memcpy(Data, RawFileData.GetData(), RawFileData.Num());
		FMemory::Memcpy(Data, pRGBData, unDataBytes);
		Mip.BulkData.Unlock();
		MyTexture->UpdateResource();

		// Free data
		free(pRGBData);
		//QRcode_free(pQRC);
	}
#endif

	return MyTexture;
}

UTexture2D* UQRCodeGenerator::GenerateQRCodeWithParams(FString code, int QRCodeVersion, int PixelNum, FColor Backcolor, FColor QRPixelColor)
{
	UTexture2D* MyTexture = NULL;

#if PLATFORM_WINDOWS || PLATFORM_MAC
	std::string MyStdString(TCHAR_TO_UTF8(*code));
	const char* szSourceSring = MyStdString.c_str();    //QRCODE_TEXT
	unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char* pRGBData, * pDestData;

	QrCode pQRC = QrCode::encodeText(szSourceSring, QrCode::Ecc::MEDIUM);

	if (true)
	{
		unWidth = pQRC.getSize();
		unWidthAdjusted = unWidth * PixelNum * 4;
		/*if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;*/
		unDataBytes = unWidthAdjusted * unWidth * PixelNum;

		// Allocate pixels buffer
		pRGBData = (unsigned char*)malloc(unDataBytes);
		if (!pRGBData)
		{
			printf("Out of memory");
			exit(-1);
		}

		// Preset to white
		memset(pRGBData, 0xff, unDataBytes);

		// Convert QrCode bits to bmp pixels
		//pSourceData = pQRC->data;
		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * PixelNum;
			for (x = 0; x < unWidth; x++)
			{
				if (pQRC.getModule(x, y))
				{
					for (l = 0; l < (unsigned int)PixelNum; l++)
					{
						for (n = 0; n < (unsigned int)PixelNum; n++)
						{
							/**(pDestData + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_B;
							*(pDestData + 1 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_G;
							*(pDestData + 2 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_R;
							*(pDestData + 3 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_A;*/

							*(pDestData + n * 4 + unWidthAdjusted * l) = QRPixelColor.B;
							*(pDestData + 1 + n * 4 + unWidthAdjusted * l) = QRPixelColor.G;
							*(pDestData + 2 + n * 4 + unWidthAdjusted * l) = QRPixelColor.R;
							*(pDestData + 3 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_A;
						}
					}
				}
				else
				{
					for (l = 0; l < (unsigned int)PixelNum; l++)
					{
						for (n = 0; n < (unsigned int)PixelNum; n++)
						{
							/**(pDestData + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_B;
							*(pDestData + 1 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_G;
							*(pDestData + 2 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_R;
							*(pDestData + 3 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_A;*/

							*(pDestData + n * 4 + unWidthAdjusted * l) = Backcolor.B;
							*(pDestData + 1 + n * 4 + unWidthAdjusted * l) = Backcolor.G;
							*(pDestData + 2 + n * 4 + unWidthAdjusted * l) = Backcolor.R;
							*(pDestData + 3 + n * 4 + unWidthAdjusted * l) = PIXEL_COLOR_A;
						}
					}
				}
				pDestData += 4 * PixelNum;
				//pSourceData++;
			}
		}


		MyTexture = UTexture2D::CreateTransient(unWidth * PixelNum, unWidth * PixelNum);
		//MyTexture->SRGB = true;

#if ENGINE_MAJOR_VERSION < 5
		FTexture2DMipMap& Mip = MyTexture->PlatformData->Mips[0];
#else
		FTexture2DMipMap& Mip = MyTexture->GetPlatformData()->Mips[0];
#endif
		void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);

		//FMemory::Memcpy(Data, RawFileData.GetData(), RawFileData.Num());
		FMemory::Memcpy(Data, pRGBData, unDataBytes);
		Mip.BulkData.Unlock();
		MyTexture->UpdateResource();

		// Free data
		free(pRGBData);
		//QRcode_free(pQRC);
	}
#endif

	return MyTexture;
}



// uncomment the following line to resolve
#pragma pack(pop)