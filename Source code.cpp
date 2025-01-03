#include <windows.h>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#include <math.h>
#include <time.h>
#define M_PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}


typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE unused;
	};
}_RGBQUAD, * PRGBQUAD;

DWORD WINAPI hell(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int x = SM_CXSCREEN;
		int y = SM_CYSCREEN;
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 30, rand() % 30, w, h, hdc, rand() % 30, rand() % 30, NOTSRCERASE);
		Sleep(10);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader1(LPVOID lpParam)
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;


	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int fx = (int)((i ^ 4) + (i * 4) * log((x * 111 + i) + (111 * y + x)));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}

DWORD WINAPI train(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, -30, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, w - 30, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI colors(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int x = GetSystemMetrics(0);
	int y = GetSystemMetrics(1);
	while (true)
	{
		hdc = GetDC(0);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 225, rand() % 225, rand() % 225));
		SelectObject(hdc, brush);
		PatBlt(hdc, 0, 0, rand() % x, rand() % y, PATCOPY);
		DeleteObject(brush);
		ReleaseDC(GetDesktopWindow(), hdc);
		DeleteDC(hdc);
		Sleep(100);
	}
}

DWORD WINAPI polygon(LPVOID lpParam) {
	while (1) {
		int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
		HDC hdc = GetDC(0);
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		HPEN hOldPen = SelectPen(hdc, hPen);
		HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		HBRUSH hOldBrush = SelectBrush(hdc, hBrush);
		POINT vertices[] = { {rand() % w, rand() % h}, {rand() % w, rand() % h}, {rand() % w, rand() % h} };
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
		SelectBrush(hdc, hOldBrush);
		DeleteObject(hBrush);
		SelectPen(hdc, hOldPen);
		DeleteObject(hPen);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI masher(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(HWND_DESKTOP);
		int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
		BitBlt(hdc, rand() % 5, rand() % 5, rand() % sw, rand() % sh, hdc, rand() % 5, rand() % 5, SRCCOPY);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI shader2(LPVOID lpParam)
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				FLOAT fx = x | y * i;

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCERASE);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}

	return 0x00;
}

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}

DWORD WINAPI colors2(LPVOID lpParam) {
	HDC hdc = GetDC(HWND_DESKTOP);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);

	while (true)
	{
		HDC hdc = GetDC(HWND_DESKTOP);
		int y = rand() % sh, h = sh - rand() % sh - (sh / 1 - 8);
		HBRUSH brush = CreateSolidBrush(Hue(239));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, y, sw, h, hdc, rand() % 96 - 56, y, SRCCOPY);
		PatBlt(hdc, -1, y, sw, h, PATINVERT);
		Sleep(10);
	}
}

DWORD WINAPI msg1(LPVOID lpParam) {
	while (1) {
		MessageBox(NULL, L"Just f**king deal with it, stop crying over spilled milk, you can't change the past, crying won't help fix your PC, only reinstalling Windows can!", L"Thailandium.exe - Demotivational Message", MB_ICONERROR);
	}
	return 0;
}


VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t | (t >> 9 | t >> 7)) * t & (t >> 11 | t >> 9));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t * "57"[(t >> 14) % 2] >> 2 & 128) + (t * "1234"[(t >> 10) % 4] * "57"[(t >> 14) % 2] & 95) + 96 * (1 + (1 & t >> "999"[(t >> 12) % 2])) + 70 * sin(4E3 / (t & 4095)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 11 & t >> 8 & 123 & t >> 3));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}


VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 9 ^ (t >> 7) - 1 ^ 1) % 13 * t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(9 * t & t >> 4 | 5 * t & t >> 7 | (3 * t & t >> 10) - 2);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (4 + (t >> 9) % 3) & t / 3 >> 7 & 224);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}





int main() {
	if (MessageBoxW(NULL, L"this malware is called Thailandium, it is a remake of pcigdvvibp.exe, it contains flashing lights, it could do serious harm to your eyes, & it could cause a seizure, do you want to run this program?", L"Thailandium", MB_YESNO | MB_ICONWARNING) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"are you really sure?", L"Thailandium", MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			Sleep(5000);
			HANDLE thread1 = CreateThread(0, 0, hell, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, shader1, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread3 = CreateThread(0, 0, train, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, colors, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			TerminateThread(thread3dot1, 0);
			CloseHandle(thread3);
			InvalidateRect(0, 0, 0);
			CloseHandle(thread3dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, polygon, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, masher, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			CloseHandle(thread4dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, shader2, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			CreateThread(0, 0, msg1, 0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, colors2, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, colors, 0, 0, 0);
			sound6();
			Sleep(30000);
			Sleep(100);
			ExitProcess(0);
		}
	}
}
