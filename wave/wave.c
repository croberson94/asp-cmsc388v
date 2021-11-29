// Filename: wave.c
// Description: wave generating file
// Date: 11/29/21
// Author: Charles Roberson

// PREPROCESSOR DIRECTIVES
#include <stdint.h>

// DATA
typedef int8_t fourcc[4];

struct riff_hdr{
	fourcc id;
	uint32_5 size;
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


