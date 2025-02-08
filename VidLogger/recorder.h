#pragma once
#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wmcodecdsp.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")

namespace recorder {
	class Recorder {
		private:
			IMFSinkWriter* writer;
			DWORD index;
			UINT frame;
			INT width;
			INT height;

			void mfInit() {
				MFStartup(MF_VERSION);
			}

			void mfKill() {
				MFShutdown();
			}

			HRESULT init(const std::wstring& output) {
				HRESULT result = S_OK;
				IMFMediaType* mediaOut = NULL;
				IMFMediaType* mediaIn = NULL;

				result = MFCreateSinkWriterFromURL(output.c_str(), NULL, NULL, &writer);
				if (FAILED(result))
					return result;

				result = MFCreateMediaType(&mediaOut);
				if (FAILED(result))
					return result;

				result = mediaOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
				result = mediaOut->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
				result = mediaOut->SetUINT32(MF_MT_AVG_BITRATE, 4000000);
				result = mediaOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
				result = MFSetAttributeSize(mediaOut, MF_MT_FRAME_SIZE, width, height);
				result = MFSetAttributeRatio(mediaOut, MF_MT_FRAME_RATE, 30, 1);
				result = MFSetAttributeRatio(mediaOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);

				result = writer->AddStream(mediaOut, &index);
				if (FAILED(result))
					return result;

				result = MFCreateMediaType(&mediaIn);
				if (FAILED(result))
					return result;

				result = mediaIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
				result = mediaIn->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
				result = mediaIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
				result = MFSetAttributeSize(mediaIn, MF_MT_FRAME_SIZE, width, height);
				result = MFSetAttributeRatio(mediaIn, MF_MT_FRAME_RATE, 30, 1);
				result = MFSetAttributeRatio(mediaIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);

				result = writer->SetInputMediaType(index, mediaIn, NULL);
				if (FAILED(result)) return result;

				result = writer->BeginWriting();

				if (mediaOut) mediaOut->Release();
				if (mediaIn) mediaIn->Release();

				return result;
			}

			HRESULT write(byte* data, unsigned long size) {
				IMFSample* sample = NULL;
				IMFMediaBuffer* buffer = NULL;
				BYTE* dst = NULL;
				LONGLONG start = frame * 333333;

				HRESULT result = MFCreateMemoryBuffer(size, &buffer);
				if (FAILED(result))
					return result;

				result = buffer->Lock(&dst, NULL, NULL);
				if (SUCCEEDED(result)) {
					memcpy(dst, data, size);
					result = buffer->Unlock();
					buffer->SetCurrentLength(size);
				}

				result = MFCreateSample(&sample);
				if (FAILED(result))
					return result;

				result = sample->AddBuffer(buffer);
				if (FAILED(result))
					return result;

				result = sample->SetSampleTime(start);
				if (FAILED(result))
					return result;

				result = sample->SetSampleDuration(333333);
				if (FAILED(result))
					return result;

				result = writer->WriteSample(index, sample);

				frame++;

				if (sample) sample->Release();
				if (buffer) buffer->Release();

				return result;
			}

		public:
			Recorder() : writer(NULL), index(0), frame(0) {
				width = GetSystemMetrics(SM_CXSCREEN);
				height = GetSystemMetrics(SM_CYSCREEN);
				mfInit();
			}

			~Recorder() {
				if (writer) {
					writer->Flush(index);
					writer->Release();
				}
				mfKill();
			}

			HRESULT CaptureFrame() {
				HDC screen = GetDC(NULL);
				HDC memDevice = CreateCompatibleDC(screen);
				HBITMAP bitmap = CreateCompatibleBitmap(screen, width, height);
				HGDIOBJ object = SelectObject(memDevice, bitmap);

				BitBlt(memDevice, 0, 0, width, height, screen, 0, 0, SRCCOPY);

				BITMAPINFOHEADER header;
				ZeroMemory(&header, sizeof(BITMAPINFOHEADER));
				header.biSize = sizeof(BITMAPINFOHEADER);
				header.biWidth = width;
				header.biHeight = -height;
				header.biPlanes = 1;
				header.biBitCount = 32;
				header.biCompression = BI_RGB;

				BYTE* data = new BYTE[width * height * 4];
				GetDIBits(memDevice, bitmap, 0, height, data, (BITMAPINFO*)&header, DIB_RGB_COLORS);

				HRESULT result = write(data, width * height * 4);

				delete[] data;
				SelectObject(memDevice, object);
				DeleteObject(bitmap);
				DeleteDC(memDevice);
				ReleaseDC(NULL, screen);

				return result;
			}

			HRESULT StartRecording(const std::wstring& outputPath) {
				return init(outputPath);
			}
	};

	bool startRecording(int duration, const std::wstring output) {
		try {
			Recorder recorder;
			HRESULT result = recorder.StartRecording(output);
			if (FAILED(result))
				return false;

			auto startTime = std::chrono::high_resolution_clock::now();
			while (std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::high_resolution_clock::now() - startTime).count() < duration) {

				result = recorder.CaptureFrame();
				if (FAILED(result))
					return false;

				std::this_thread::sleep_for(std::chrono::milliseconds(33));
			}

			return true;
		}
		catch (...) { return false; }
	}
}