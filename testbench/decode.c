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

/*
typedef struct _PB_Region_Band {
	uint32_t start;
	uint32_t end;
	int8_t power_limit;
	uint8_t duty_cycle;
} PB_Region_Band;

typedef struct _PB_Region {
	pb_bytes_array_t *country_code;
	pb_callback_t bands;
} PB_Region;
*/
static bool subghz_on_system_start_istream_read(pb_istream_t* istream,
						pb_byte_t* buf, size_t count)
{
	int file = *(int *)istream->state;
	size_t ret = read(file, buf, count);
	return (count == ret);
}

static bool subghz_on_system_start_istream_decode_band(
pb_istream_t* stream,
const pb_field_t* field,
void** arg)
{
	(void)field;
	FuriHalRegion* region = *arg;

	PB_Region_Band band = {0};
	if(!pb_decode(stream, PB_Region_Band_fields, &band)) {
		printf("PB Region band decode error: %s", PB_GET_ERROR(stream));
		return false;
	}

	region->bands_count += 1;
	region = realloc( //-V701
	region,
	sizeof(FuriHalRegion) + sizeof(FuriHalRegionBand) * region->bands_count);
	size_t pos = region->bands_count - 1;
	region->bands[pos].start = band.start;
	region->bands[pos].end = band.end;
	region->bands[pos].power_limit = band.power_limit;
	region->bands[pos].duty_cycle = band.duty_cycle;
	*arg = region;

	printf(
		"Add allowed band: start %uHz, stop %uHz, power_limit %ddBm, duty_cycle %u%%\n",
		band.start,
		band.end,
		band.power_limit,
		band.duty_cycle);
	return true;
}

int main(void)
{
	int file = 0;
	off_t size = 0;
	PB_Region pb_region = {0};
	pb_istream_t istream;

	file = open("region_data", O_RDONLY);
	if (file < 0) {
		fprintf(stderr, "Unable to open region_data file\n");
		return 1;
	}
		
	size = lseek(file, 0, SEEK_END);
	lseek(file, 0, SEEK_SET);

	pb_region.bands.funcs.decode = subghz_on_system_start_istream_decode_band;
	istream.callback = subghz_on_system_start_istream_read;
	istream.state = &file;
	istream.errmsg = NULL;
	istream.bytes_left = size;

	pb_region.bands.arg = calloc(1, sizeof(FuriHalRegion));
	if(!pb_decode(&istream, PB_Region_fields, &pb_region)) {
		printf("Invalid region data");
		FuriHalRegion* region = pb_region.bands.arg;
		memcpy(region->country_code, pb_region.country_code->bytes,
			pb_region.country_code->size < 4 ? pb_region.country_code->size : 3);

		free(pb_region.bands.arg);
		return 1;
	}


	FuriHalRegion* region = pb_region.bands.arg;
	memcpy(region->country_code, pb_region.country_code->bytes,
		pb_region.country_code->size < 4 ? pb_region.country_code->size : 3);

	/* XXX: Region should now be decoded */
	pb_release(PB_Region_fields, &pb_region);
	close(file);


	return 0;
}
