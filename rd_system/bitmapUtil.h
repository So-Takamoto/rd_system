#ifndef __BITMAPUTIL_H__
#define __BITMAPUTIL_H__

#include <fstream>
#include <cstdio>
#include <string>

void saveBMP(std::string filename, int width, int height, void* pixels){
	BITMAPFILEHEADER bmpFile;
	bmpFile.bfType = 'MB';
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+3*width*height;
	bmpFile.bfReserved1 = 0;
	bmpFile.bfReserved2 = 0;
	bmpFile.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	BITMAPINFOHEADER bmpInfo;
	bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.biWidth = width;
	bmpInfo.biHeight = height;
	bmpInfo.biPlanes = 1;
	bmpInfo.biBitCount = 24;
	bmpInfo.biSizeImage = 0;
	bmpInfo.biCompression = BI_RGB;
	bmpInfo.biXPelsPerMeter = 0;
	bmpInfo.biYPelsPerMeter = 0;
	bmpInfo.biClrUsed = 0;
	bmpInfo.biClrImportant = 0;
	char* buf = new char[50];
	std::ofstream fs(filename, std::ios::binary|std::ios::trunc);
	memcpy(buf, &bmpFile, sizeof(BITMAPFILEHEADER));
	fs.write(buf, sizeof(BITMAPFILEHEADER));
	memcpy(buf, &bmpInfo, sizeof(BITMAPINFOHEADER));
	fs.write(buf, sizeof(BITMAPINFOHEADER));
	fs.write(reinterpret_cast<char*>(pixels), width*height*3);
	delete[] buf;
}

#endif