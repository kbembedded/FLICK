# Include the nanopb provided Makefile rules
include ../lib/nanopb/extra/nanopb.mk

# Compiler flags to enable all warnings & debug info
CFLAGS = -Wall -Werror -g -O0 -DPB_ENABLE_MALLOC
CFLAGS += "-I$(NANOPB_DIR)"

# C source code files that are required
CSRC = flick.pb.c                # The compiled protocol definition
CSRC += $(NANOPB_DIR)/pb_encode.c  # The nanopb encoder
CSRC += $(NANOPB_DIR)/pb_decode.c  # The nanopb decoder
CSRC += $(NANOPB_DIR)/pb_common.c  # The nanopb common parts

.PHONY: all
all: encode decode

.PHONY: clean
clean:
	rm -f decode encode *.pb.*

# Build rule for the main program
decode: decode.c $(CSRC)
	$(CC) $(CFLAGS) -odecode decode.c $(CSRC)

# Build rule for the main program
encode: encode.c $(CSRC)
	$(CC) $(CFLAGS) -oencode encode.c $(CSRC)

# Build rule for the protocol
flipper.pb.c: flick.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. flick.proto


