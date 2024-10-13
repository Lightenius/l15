#include <corecrt.h>
#include <immintrin.h>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <iostream>
#include <winscard.h>



IAudioClient *client = NULL;
IAudioRenderClient *render_client = NULL;
IMMDevice * speaker = NULL;



struct AUD_PACKET {
	UINT32 BufferSize;
};






int main(void){

	return 0;
}


void startPlayback(){
	;
}

void createAudioClient(){
	IMMDeviceEnumerator *enumer = NULL;
	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void **) &enumer);

	enumer->GetDefaultAudioEndpoint(eRender, eConsole, &speaker);

	speaker->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **) &client);

	client->GetService(IID_IAudioRenderClient, (void **) &render_client);

	// NOTE: LEFTHERE


}

void recievePacket(){
	;
}
