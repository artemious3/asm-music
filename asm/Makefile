BUILDDIR = ./build

libSequencer.a: buffer.o sequencer.o
	ar rcs libSequencer.a buffer.o sequencer.o
	

buffer.o: buffer.asm config.asm  samples/*.dat 
	fasm buffer.asm


sequencer.o: sequencer.asm config.asm
	fasm sequencer.asm


