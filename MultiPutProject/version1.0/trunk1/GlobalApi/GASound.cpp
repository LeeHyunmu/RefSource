
#include "stdAfx.h"
#pragma comment(lib,"Winmm.lib")
//--------------------------------------------------------------
//|
//|持失切
//|
//---------------------------------------------------------------
GASound::GASound(void)
{
}
//--------------------------------------------------------------
//|
//|社瑚切
//|
//---------------------------------------------------------------
GASound::~GASound(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void GASound::SetSoundVolume(DWORD dwVolume)
{
	WAVEFORMATEX wf;
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 1;
	wf.nSamplesPerSec = 8000 * 1000;
	wf.wBitsPerSample = 8;
	wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	wf.cbSize = 0;

	HWAVEOUT hwo;
	for (UINT id = 0; id < waveOutGetNumDevs(); id++) 
	{
		if (waveOutOpen(&hwo, id, &wf, 0, 0, CALLBACK_NULL)
			== MMSYSERR_NOERROR)
		{
			waveOutSetVolume(hwo, dwVolume);
			waveOutClose(hwo);
			break;
		}
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void GASound::SoundPlay(CString _wave)
{
	PlaySound(_wave, NULL, SND_ASYNC | SND_FILENAME);
}