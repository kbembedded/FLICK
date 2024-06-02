#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "flick.pb.h"

#include "pb_decode.h"
#include "pb_encode.h"
#include "pb_common.h"

typedef struct {
	uint32_t start;
	uint32_t end;
	int8_t power_limit;
	uint8_t duty_cycle;
} FuriHalRegionBand;

typedef struct {
	char country_code[4];
	uint16_t bands_count;
	FuriHalRegionBand bands[];
} FuriHalRegion;

FuriHalRegion region = {
	"US",
	3,
	{
		{ 304100000, 321950000, 12, 50 },
		{ 433050000, 434790000, 12, 50 },
		{ 915000000, 928000000, 12, 50 },
		{},
	},
};


static bool subghz_on_system_start_istream_encode_band(
pb_ostream_t* stream,
const pb_field_t* field,
void * const *arg)
{

	FuriHalRegion* reg = NULL;
	FuriHalRegionBand* bands = NULL;
	int i;

	reg = *arg;
	bands = reg->bands;

	for (i = reg->bands_count; i; i--) {
		if (!pb_encode_tag_for_field(stream, field)) {
			printf("tag failed\n");
			return false;
		}

		if (!pb_encode_submessage(stream, PB_Region_Band_fields, bands)) {
			printf("submessage failed\n");
			return false;
		}
		bands++;
	}

	return true;
}

int main(void)
{
	pb_byte_t *buf = NULL;
	size_t encoded_size = 0;
	PB_Region pb_region = {0};
	pb_ostream_t ostream;
	int i;

	pb_region.country_code = malloc(3); // XXX: Hardcoded at the moment
	pb_region.country_code->size = 2;
	memcpy(pb_region.country_code->bytes, region.country_code, 3);

	pb_region.bands.funcs.encode = subghz_on_system_start_istream_encode_band;
	pb_region.bands.arg = &region;

	if (!pb_get_encoded_size(&encoded_size, PB_Region_fields, &pb_region)) {
		printf("Shits fucked\n");
		return 1;
	}
	buf = malloc(256);

	ostream = pb_ostream_from_buffer(buf, 256);

	if(!pb_encode(&ostream, PB_Region_fields, &pb_region)) {
		printf("Invalid region data");
		return 1;
	}

	for (i = 0; i < encoded_size; i++) putchar(buf[i]);

	/* XXX: Region should now be decoded */
	pb_release(PB_Region_fields, &pb_region);


	return 0;
}
