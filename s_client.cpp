#include <minwindef.h>
#include <mmdeviceapi.h>
#include <immintrin.h>
#include <audioclient.h>
#include <mmeapi.h>
#include <synchapi.h>
#include <winscard.h>

#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC 10000
#define BUFFER_TIME_MULTIPLIER 2 // NOTE: Uyku süresini belirler / Ne Kadar büyükse o kadar kısa uyunur



void sendPacket(BYTE *data, UINT32 packetSize);
bool setupAudioClient();

REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
REFERENCE_TIME hnsSleepDuration;
WAVEFORMATEX *waveform = NULL;
IAudioCaptureClient *capture_client = NULL;
IAudioClient *client = NULL;
IMMDevice *mic = NULL;
UINT32 bufferSize;

int main(int argc, char *argv[]){
	CoInitialize(NULL);
	setupAudioClient();



	return 0;
}


bool setupAudioClient(){
	
	IMMDeviceEnumerator *enumer = NULL;

	// TODO: Add err handling

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void **) &enumer );
	enumer->GetDefaultAudioEndpoint(eCapture, eConsole, &mic);


	mic->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &client );


	client->GetMixFormat(&waveform);

	// NOTE: Bitwise could be tried as a Init StreamFlag
	client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, waveform, NULL);


	client->GetService(__uuidof(IAudioCaptureClient), (void **) &capture_client );


	client->GetBufferSize(&bufferSize);


	hnsSleepDuration = (double)REFTIMES_PER_SEC * bufferSize / waveform->nSamplesPerSec / REFTIMES_PER_MILLISEC / BUFFER_TIME_MULTIPLIER;


	return true;
}


void startTransmission(){
	
	client->Start();
	
	bool done = false;
	UINT32 packetSize;
	BYTE *data = NULL;
	UINT32 framesToRead;
	DWORD flags;


	while (done == false) {
	
		Sleep(hnsSleepDuration);

		capture_client->GetNextPacketSize(&packetSize);
	
		while ( packetSize > 0 ) {
			if ( capture_client->GetBuffer(&data, &framesToRead, &flags, NULL, NULL) == S_OK ){
				sendPacket(data, packetSize);
			}

			capture_client->ReleaseBuffer(framesToRead);
			capture_client->GetNextPacketSize(&packetSize);

		}



	}

}


void sendPacket(BYTE *data, UINT32 packetSize){
	;
}
