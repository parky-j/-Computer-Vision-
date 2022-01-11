#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
void InverseImage(BYTE* Img, BYTE *Out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255)
		{
			Out[i] = 255;
		}
		else if (Img[i] + Val < 0)
		{
			Out[i] = 0;
		}
		else 	Out[i] =Img[i] + Val;
	}
}
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] * Val > 255.0)
		{
			Out[i] = 255;
		}
		else 	Out[i] = (BYTE)(Img[i] * Val);
	}
}

void ObtainHistogram(BYTE* Img, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
}

void ObtainAHistogram(int* Histo, int* AHisto)
{
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			AHisto[i] += Histo[j];
		}
	}
}

void HistogramStretching(BYTE * Img, BYTE * Out, int * Histo, int W, int H)
{
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0);
	}
}
void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H)
{
	int ImgSize = W * H;
	int Nt = W * H, Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++) {
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = NormSum[Img[i]];
	}
}

void Binarization(BYTE * Img, BYTE * Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

int GozalezBinThresh(BYTE* Image, int* Histo, int Epsilon, int ImgSize)
{
	int low, high;
	int var = 0;
	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			high = i;
			break;
		}
	}

	int T = (low + high) / 2;
	do {
		int sum = 0, cnt = 0;
		double m1 = 0, m2 = 0;

		for (int i = 0; i < T; i++) {
			sum += Histo[i] * i;
			cnt += Histo[i];
		}
		m1 = (double)sum / cnt;

		sum = 0; cnt = 0;

		for (int i = T; i < 256; i++) {
			sum += Histo[i] * i;
			cnt += Histo[i];
		}
		m2 = (double)sum / cnt;

		int nT = (int)((m1 + m2) / 2);
		if (T > nT) var = T - nT;
		else if (T <= nT) var = nT - T;
		T = nT;
	} while (var >= Epsilon);
	return T;
}

void AverageConv(BYTE* Img, BYTE* Out, int W, int H) // �ڽ���Ȱȭ
{
	double Kernel[3][3] = {0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111	};
	double SumProduct = 0.0;
	for (int i = 1; i < H-1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W-1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i+m)*W + (j+n)] * Kernel[m+1][n+1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) // ����þ���Ȱȭ
{
	double Kernel[3][3] = {0.0625, 0.125, 0.0625,
										0.125, 0.25, 0.125,
										0.0625, 0.125, 0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����ũ X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����ũ X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 1.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����ũ X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-2.0, 0.0, 2.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����ũ X
{
	double Kernel[3][3] = { -1.0, -2.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 2.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����ũ X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 8.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) // Prewitt ����ũ X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 9.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++) { // X��ǥ (��)
			for (int m = -1; m <= 1; m++) { // Kernel ��
				for (int n = -1; n <= 1; n++) { // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			//Out[i * W + j] = abs((long)SumProduct) / 8;
			if (SumProduct > 255.0) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, 
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE * fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W*H, fp);
	fclose(fp);
}

void swap(BYTE* a, BYTE* b) 
{
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}

BYTE Median(BYTE* arr, int size)
{
	// �������� ����
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // �񱳴�� index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S/2];
}

BYTE MaxPooling(BYTE* arr, int size)
{
	// �������� ����
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // �񱳴�� index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S-1];
}

BYTE MinPooling(BYTE* arr, int size)
{
	// �������� ����
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // �񱳴�� index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[0];
}

int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}


// GlassFire �˰����� �̿��� �󺧸� �Լ�
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// �������� ����� �޸� �Ҵ�
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// �󺧸��� �ȼ��� �����ϱ� ���� �޸� �Ҵ�

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // �޸� �ʱ�ȭ

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// �̹� �湮�� ���̰ų� �ȼ����� 255�� �ƴ϶�� ó�� ����
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//���� �ȼ��� �����踦 ����� ó�� ����
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // ���� �󺧷� ��ũ
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor;

	// ���� ������ ���� ������ ã�Ƴ��� ���� 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage �迭 Ŭ����~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	// coloring�� ����� �󺧸� ����� (Out_Area�� �����) ������ ���� ū �͸� CutImage�� ����
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // ���� ū �͸� ����
		//if (BlobArea[coloring[k]] > 500) CutImage[k] = 0;
		//CutImage[k] = (unsigned char)(coloring[k] * grayGap);
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// �󺧸� �� ���� ���� ������ ���ؼ��� �̾Ƴ��� �ڵ� ����

void BinaryImageEdgeDetection(BYTE *Bin, BYTE *Out, int W, int H) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Bin[i * W + j] == 0) {
				if (!(Bin[(i - 1) * W + j] == 0 && Bin[(i + 1) * W + j] == 0
					&& Bin[i * W + j - 1] == 0 && Bin[i * W + j + 1] == 0))
					Out[i * W + j] = 255;
			}
		}
	}
}

void FindRectOutlinePoints(BYTE* Img, int W, int H, int* arr) {
	for (int i = H - 1; i >= 0; i--) {
		for (int j = 0; j < W; j++) {
			if (Img[i * W + j] == 0) {
				arr[0] = i;
				break;
			}
		}
		if (arr[0] != NULL) break;
	}	
	for (int i = 0; i < H; i++) {
		for (int j = W - 1; j >= 0; j--) {
			if (Img[i * W + j] == 0) {
				arr[1] = i;
				break;
			}
		}
		if (arr[1] != NULL) break;
	}
	for (int j = 0; j < W; j++) {
		for (int i = H - 1; i >= 0; i--) {
			if (Img[i * W + j] == 0) {
				arr[2] = j;
				break;
			}
		}
		if (arr[2] != NULL) break;
	}
	for (int j = W-1; j >= 0; j--) {
		for (int i = 0; i < H; i++) {
			if (Img[i * W + j] == 0) {
				arr[3] = j;
				break;
			}
		}
		if (arr[3] != NULL) break;
	}
}

void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y) {
	for (int i = LU_X; i <= RD_X; i++) 
		Img[LU_Y * W + i] = 255;
	
	for (int i = LU_Y; i >= RD_Y; i--)
		Img[i * W + LU_X] = 255;
	
	for (int i = LU_X; i <= RD_X; i++)
		Img[RD_Y * W + i] = 255;

	for (int i = LU_Y; i >= RD_Y; i--)
		Img[i * W + RD_X] = 255;
}

int BinaryImageFindCenX(BYTE *Img, int W, int H) {
	int cnt = 0, sumx = 0;
	int cenx = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Img[i * W + j] == 0) {
				cnt++;
				sumx += j;
			}
		}
	}

	cenx = sumx / cnt;
	return cenx;
}

int BinaryImageFindCenY(BYTE* Img, int W, int H) {
	int cnt = 0, sumy = 0;
	int ceny = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Img[i * W + j] == 0) {
				cnt++;
				sumy += i;
			}
		}
	}

	ceny = sumy / cnt;
	return ceny;
}

void DrawCrossLine(BYTE* Img, int W, int H, int Cx, int Cy) {
	for (int i = Cx; i >= 0; i--) 
		Img[Cy * W + i] = 255;
	for (int i = Cx; i < W; i++)
		Img[Cy * W + i] = 255;
	for (int i = Cy; i >= 0; i--)
		Img[i * W + Cx] = 255;
	for (int i = Cy; i < H; i++)
		Img[i * W + Cx] = 255;
}

int main()
{
	BITMAPFILEHEADER hf; // 14����Ʈ
	BITMAPINFOHEADER hInfo; // 40����Ʈ
	RGBQUAD hRGB[256]; // 1024����Ʈ
	FILE* fp;
	fp = fopen("pupil2.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1,  fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);
	BYTE * Temp = (BYTE*)malloc(ImgSize); // �ӽù迭
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int H = hInfo.biHeight, W = hInfo.biWidth;
	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };

	/*ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainAHistogram(Histo, AHisto);
	HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);*/
	/*int Thres = GozalezBinThresh();
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);

	GaussAvrConv(Image, Output, hInfo.biWidth, hInfo.biHeight);*/
	
	/*Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) 	Output[i] = Temp[i];
	}
	Binarization(Output, Output, hInfo.biWidth, hInfo.biHeight, 40);*/
	/*GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);*/
	
	//HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight);
	//InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//BrightnessAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 70);
	//ContrastAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 0.5);

	/*	
	// Median filtering 
	int Length = 5;  // ����ũ�� �� ���� ����
	int Margin = Length / 2;  // ����ũ�� ���� ���� ����� ���� �����ϱ� ���� ����ũ�� �� ���� ������ ������ �������� ����
	int WSize = Length * Length; // ����ũ�� �� ������
	BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * WSize); // ���� ���󿡼� ���� ����ũ�� ��ġ�� ������ ȭ�Ұ��� ������ �ӽ� �迭 ����
	int W = hInfo.biWidth, H = hInfo.biHeight; // W�� H ������ �����Ͽ� ���� ���� ������ ���α���, ���α��̷� �ʱ�ȭ
	int i, j, m, n; // Median filtering�� �����ϱ� ���� �ݺ��� ���ο��� �ʿ��� ���� �̸� ����

	for (i = Margin; i < H - Margin; i++) { // ����ũ ��ü�� ���� ���� ������. ���� ���󿡼� ���� ��ŭ�� �������� �ΰ� �����δ�. i���� ���� ������� ����� ��ġ�� y��ǥ�� �����
		for (j = Margin; j < W - Margin; j++) { // ����ũ ��ü�� ���� ���� ������. ���������� ���� ���󿡼� ���� ��ŭ�� �������� �ΰ� �����δ�. j���� ���� ������� ����� ��ġ�� x��ǥ�� �����
			for (m = -Margin; m <= Margin; m++) { // ����ũ ���ο����� ���� ���� ������. m�� ���� -Margin���� Margin���� ���ϸ� ����ũ�� �߾��� y��ǥ�� �󸶳� ������ ��ġ������ ��Ÿ��
				for (n = -Margin; n <= Margin; n++) { // ����ũ ���ο����� ���� ���� ������. n�� ���� -Margin���� Margin���� ���ϸ� ����ũ�� �߾��� x��ǥ�� �󸶳� ������ ��ġ������ ��Ÿ��
					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n]; // temp 1���� �迭�� ���� ���󿡼� ���� ����ũ�� �����ϴ� ������ ȭ�Ұ��� ����
				}
			}
			Output[i * W + j] = Median(temp, WSize); // ���󿡼� ���� ����ũ�� ��ġ�� ��ġ�� �߾�(ȭ�Ұ��� ���� ������ ��ġ)�� temp�� ����� ���� �� �߾Ӱ��� �ʱ�ȭ
		}
	}
	free(temp); // �޸𸮴��� ���� ���� free�Լ��� �� �̻� ������ �ʴ� temp�� Ǯ����
	/* Median filtering */


	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
		
	Binarization(Image, Temp, W, H, 50);
	InverseImage(Temp, Temp, W, H);
	m_BlobColoring(Temp, H, W);
	for (int i = 0; i < ImgSize; i++) Output[i] = Image[i];
	BinaryImageEdgeDetection(Temp, Output, W, H);

	int arr[4] = {NULL};
	FindRectOutlinePoints(Temp, W, H, arr);
	int cenx = BinaryImageFindCenX(Temp, W, H);
	int ceny = BinaryImageFindCenY(Temp, W, H);

	DrawRectOutline(Output, W, H, arr[2], arr[0], arr[3], arr[1]);
	DrawCrossLine(Output, W, H, cenx, ceny);

	SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "pupil2_output.bmp");


	free(Image);
	free(Output);
	free(Temp);
	return 0;
}