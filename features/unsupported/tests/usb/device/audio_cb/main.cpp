// Playback example with the USBAUDIO library

#include "mbed.h"
#include "USBAudio.h"

// frequency: 48 kHz
#define FREQ_SPK 16000
#define FREQ_MIC 16000

// 2channels: stereo
#define NB_CHA_SPK 2
#define NB_CHA_MIC 2

// length computed: each ms, we receive 48 * 16bits ->48 * 2 bytes. as there are two channels, the length will be 48 * 2 * 2
#define LENGTH_AUDIO_PACKET_SPK (FREQ_SPK / 500) * NB_CHA_SPK
#define LENGTH_AUDIO_PACKET_MIC (FREQ_MIC / 500) * NB_CHA_MIC

// USBAudio object
USBAudio audio(FREQ_SPK, NB_CHA_SPK, FREQ_MIC, NB_CHA_MIC, 0xab45, 0x0378);
int filled;
int ready = 2;

/*  buffer 4 packets to avoid  */
int buf[4][LENGTH_AUDIO_PACKET_SPK/sizeof(int)];
void tx_audio(void)
{
    /*  used some buffer in advance  */
    ready = (filled+2)&0x3;
    audio.writeSync((uint8_t *)buf[ready]);
}


void rx_audio(void)
{
    int size=0;
    int next = (filled + 1)&0x3; 
    size = audio.readSync((uint8_t *)buf[next]);
    if (size ) filled = next;
    if ((size) && (size!=LENGTH_AUDIO_PACKET_MIC)) printf("%d\n",size);
}

int main()
{
    filled = 0;
    memset(&buf[0][0], 0, sizeof (buf));
    audio.attachTx(tx_audio);
    audio.attachRx(rx_audio);
    /*  start the tx with a silent packet */
    audio.write((uint8_t *)buf[2]);
    while(1);
}
