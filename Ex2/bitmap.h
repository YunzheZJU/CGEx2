#pragma once
typedef struct tagBITMAPFILEHEADER {

	unsigned char bfType[2];

	unsigned char bfSize[4];

	unsigned char bfReserved1[2];

	unsigned char bfReserved2[2];

	unsigned char bfOffBits[4];

} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {

	unsigned char biSize[4];

	unsigned char biWidth[4];

	unsigned char biHeight[4];

	unsigned char biPlanes[2];

	unsigned char biBitCount[2];

	unsigned char biCompression[4];

	unsigned char biSizeImage[4];

	unsigned char biXPelsPerMeter[4];

	unsigned char biYPelsPerMeter[4];

	unsigned char biClrUsed[4];

	unsigned char biClrImportant[4];

} BITMAPINFOHEADER;

BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;

void LongToByte(unsigned long value, unsigned char * outResult) {
	int i = 0;
	for (i = 0; i<4; i++) {
		outResult[i] = (unsigned char)(value % 16);
		value = value / 16;
		outResult[i] += (unsigned char)(value % 16) * 16;
		value = value / 16;
	}
	outResult[4] = '\0';
}

void screenshot(void) {
	GLint viewPort[4] = { 0 };
	glGetIntegerv(GL_VIEWPORT, viewPort);
	GLbyte * buffer = (GLbyte *)malloc(viewPort[2] * viewPort[3] * sizeof(GLbyte) * 3);
	glReadPixels(viewPort[0], viewPort[1], viewPort[2], viewPort[3],
		GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
	long fileSize = viewPort[2] * viewPort[3] * 3 + 54;
	//int i=0;  

	fileHeader.bfType[0] = 0x42;
	fileHeader.bfType[1] = 0x4d;
	LongToByte(fileSize, fileHeader.bfSize);
	LongToByte(54, fileHeader.bfOffBits);

	LongToByte(sizeof(infoHeader), infoHeader.biSize);
	LongToByte(viewPort[2], infoHeader.biWidth);
	LongToByte(viewPort[3], infoHeader.biHeight);

	infoHeader.biPlanes[0] = 0x01;
	infoHeader.biPlanes[1] = 0x00;
	infoHeader.biBitCount[0] = 0x18;
	infoHeader.biBitCount[1] = 0x00;
	LongToByte(0, infoHeader.biCompression);

	LongToByte((viewPort[2] * viewPort[3]), infoHeader.biSizeImage);

	char filename[15] = "screenshot.bmp";
	FILE * fp = fopen(filename, "w+");
	//cout << "1" << endl;
	fwrite(&fileHeader, sizeof(fileHeader), 1, fp);
	//cout << "2" << endl;
	fwrite(&infoHeader, sizeof(infoHeader), 1, fp);
	//cout << "3" << endl;
	fwrite(buffer, 1, (viewPort[2] * viewPort[3] * 3), fp);
	//cout << "4" << endl;
	fclose(fp);
	//cout << "5" << endl;

	free(buffer);
}