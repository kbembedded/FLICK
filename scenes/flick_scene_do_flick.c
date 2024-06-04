#include <furi_hal_region.h>
#include <storage/storage.h>

#include "flick_scene.h"
#include "../flick_data.h"

//#include <pb_common.h>
#include <pb_encode.h>
#include <flick.pb.h>

/* TODO: Add some customization options? Power, enabling specific bands rather than all, region name? */
FuriHalRegion region = {
        .country_code = "LOL",
        .bands_count = 1,
        .bands = {
                { 0, 1000000000, 53, 50 },
                {},
        },
};

static bool encode_band(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
	FuriHalRegion *reg = NULL;
	FuriHalRegionBand *bands = NULL;
	int i;

	reg = *arg;
	bands = reg->bands;

	for (i = reg->bands_count; i; i--) {
		if (!pb_encode_tag_for_field(stream, field)) {
			FURI_LOG_E("FLICK", "Encode tag failed\n");
			return false;
		}

		if (!pb_encode_submessage(stream, PB_Region_Band_fields, bands)) {
			FURI_LOG_E("FLICK", "Encode submessage failed\n");
                        return false;
                }

                bands++;
        }

        return true;
}

void flick_scene_do_flick_on_enter(void *context)
{
	furi_assert(context);
	struct flick_app *flick = context;
	pb_byte_t *buf = NULL;
	PB_Region pb_region = {0};
	pb_ostream_t ostream = {0};
	Storage *storage = NULL;
	File *file = NULL;
	size_t size;

	size = strlen(region.country_code);

	pb_region.country_code = malloc(size+1);
	pb_region.country_code->size = size;
	memcpy(pb_region.country_code->bytes, region.country_code, size+1);

	pb_region.bands.funcs.encode = encode_band;
	pb_region.bands.arg = &region;

	furi_check(
	pb_get_encoded_size(&size, PB_Region_fields, &pb_region));

	buf = malloc(size);

	ostream = pb_ostream_from_buffer(buf, size);

	furi_check(
	pb_encode(&ostream, PB_Region_fields, &pb_region));

	/* TODO: Back up existing file to our FLICK app data folder */
	/* Write out the file */
	storage = furi_record_open(RECORD_STORAGE);
	file = storage_file_alloc(storage);

	furi_check(
	storage_file_open(file, "/int/.region_data", FSAM_WRITE, FSOM_CREATE_ALWAYS));

	do {
		size -= storage_file_write(file, buf, size);
	} while(size);

	storage_file_free(file);
	furi_record_close(RECORD_STORAGE);

	/* Clean up our memory */
	free(buf);
	//pb_release(PB_Region_fields, &pb_region);
	scene_manager_search_and_switch_to_previous_scene(flick->scene_manager,
							  FlickSceneMainMenu);

}

void flick_scene_do_flick_on_exit(void *context)
{
	UNUSED(context);
}

bool flick_scene_do_flick_on_event(void* context, SceneManagerEvent event)
{
	UNUSED(context);
	UNUSED(event);

	return false;
}
