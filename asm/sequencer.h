
#ifndef SEQUENCER_H_WRDYD91H
#define SEQUENCER_H_WRDYD91H



#ifdef __cplusplus
extern "C" {
#endif

#include <cstdint>
#include <stdint.h>
#include "portaudio.h"

extern void PutNote(uint64_t note, int8_t* buf);
extern void ClearBuffer(uint8_t* buf);

extern void SequencerPlay(); // check if rbx is restored
extern void SequencerStop();
extern void SequencerInit();

struct __attribute__((__packed__)) Buffer{
	uint8_t* note_ptr;
	int8_t* WT;
	double current_node_idx;
	uint64_t current_sample_idx;
	uint64_t is_sample;
};


extern signed char Notes;

extern struct Buffer Buffers[];


extern int8_t SineTblWT[], SawtoothTblWT[], SquareTblWT[],
       	      KickTblWT[], SnareTblWT[];

extern PaStream* Stream_ptr;
extern uint64_t Current_sector;

const int BUFFER_SIZE_SECTORS = 512; 




#ifdef __cplusplus
}
#endif

#endif /* end of include guard: SEQUENCER_H_WRDYD91H */

