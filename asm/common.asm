BUFTYPE_SINE = 0
BUFTYPE_TRIANGLE = 1
BUFTYPE_SQUARE = 2
BUFTYPE_NOISE = 3

paInt8 =  0x00000010

struc Buffer WT, notes_ptr, current_note_idx, current_sample_idx, is_sample {
	.notes_ptr dq notes_ptr
	.WT dq WT 
	.current_note_idx dq  current_note_idx
	.current_sample_idx dq current_sample_idx 
	.is_sample dq is_sample
	
}

