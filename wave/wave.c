// Filename: wave.c
// Description: wave generating file
// Date: 11/29/21
// Author: Charles Roberson

// PREPROCESSOR DIRECTIVES
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// DATA
typedef int8_t fourcc[4];
typedef int16_t sample_t;
#define SAMPLE_MAX 32767
#define DURATION 5
#define SR 44100
#define NCHANNELS 1
#define NSAMPLES (NCHANNELS*DURATION*SR)
static FILE *fp;
static sample_t buf[NSAMPLES];


struct riff_hdr{
	fourcc id;
	uint32_t size;
	fourcc type;
};

struct fmt_ck{
	fourcc id;
	uint32_t size;
	uint16_t fmt_tag;
	uint16_t channels;
	uint32_t samples_per_sec;
	uint32_t bytes_per_sec;
	uint16_t block_align;
	uint16_t bits_per_sample;
};

struct data_hdr{
	fourcc id;
	uint32_t size;
};

struct wav_hdr{
	struct riff_hdr riff;
	struct fmt_ck fmt;
	struct data_hdr data;
};

// FUNCTIONS
int wave_init(void){
	struct wav_hdr hdr = {0};
	fp = fopen("output.wav", "wb");
	if (!fp)
		return 0;
	
	/* RIFF header */
	memcpy(&hdr.riff.id, "RIFF", 4);
	hdr.riff.size = 36 + NSAMPLES*sizeof(sample_t);
	memcpy(&hdr.riff.type, "WAVE", 4);
	
	/* FMT chunk */
	memcpy(&hdr.fmt.id, "fmt ", 4);
	hdr.fmt.size = 16;
	hdr.fmt.fmt_tag = 1; /* linear PCM */
	hdr.fmt.channels = NCHANNELS;
	hdr.fmt.samples_per_sec = SR;
	hdr.fmt.bytes_per_sec = NCHANNELS * SR * sizeof(sample_t);
	hdr.fmt.block_align = NCHANNELS * sizeof(sample_t);
	hdr.fmt.bits_per_sample = 8 * sizeof(sample_t);
	
	/* DATA header */
	memcpy(&hdr.data.id, "data", 4);
	hdr.data.size = NSAMPLES * sizeof(sample_t);
	
	if (fwrite(&hdr, sizeof(struct wav_hdr), 1, fp) != 1){
		fclose(fp);
		return 0;
	}
	return 1;
}

void wave_finish(void){
	fwrite(buf, sizeof(buf), 1, fp);
	/* 1 byte padding if buf size is odd */
	if (sizeof(buf) % 2 == 1){
		char nil = 0;
		fwrite(&nil, 1, 1, fp);
	}
	fclose(fp);
}

// MAIN
int main(void){
	size_t i;
	if (!wave_init())
		return 1;
	
	/* LPCM encoding */
	for (size_t i = 0; i < NSAMPLES; ++i)
		buf[i] = lrint(SAMPLE_MAX*sin(2 * M_PI * 440 * i / SR));
	
	wave_finish();
	return 0;
}
	
	
	
	