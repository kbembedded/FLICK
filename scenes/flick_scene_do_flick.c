#include <furi_hal_region.h>
#include <power/power_service/power.h>
#include <storage/storage.h>

#include "flick_scene.h"
#include "../flick_data.h"

#include <pb_decode.h>
#include <pb_encode.h>
#include <flick.pb.h>

/* TODO: Add some customization options? Power, enabling specific bands rather than all, region name? */
FuriHalRegion region = {
        .country_code = "LOL",
        .bands_count = 3,
        .bands = {
                { 300000000, 348000000, 53, 50 },
                { 387000000, 464000000, 53, 50 },
                { 779000000, 928000000, 53, 50 },
                {},
        },
};

static void popup_callback(void *context)
{
	UNUSED(context);

	power_reboot(furi_record_open(RECORD_POWER), PowerBootModeNormal);
}

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
	File *int_file = NULL;
	File *data_file = NULL;
	FuriString *data_path = NULL;
	size_t size;

	view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewLoading);

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

	/* Copy existing .region_data int_file to our APP data location as a backup */
	storage = furi_record_open(RECORD_STORAGE);
	int_file = storage_file_alloc(storage);

	/* XXX: Note that we don't really do any handling if the .region_data file
	 * did not already exist. However, we can skip copying to backup location
	 * just to prevent restoring a junk file later.
	 */

	if (storage_file_exists(storage, EXT_PATH(".int/.region_data"))) {
		data_path = furi_string_alloc_set(flick->data_path);
		furi_string_cat_str(data_path, ".region_data");
		FURI_LOG_D("FLICK", "Copying region data to \'%s\'", furi_string_get_cstr(data_path));
		data_file = storage_file_alloc(storage);
		furi_check(
		storage_file_open(data_file, furi_string_get_cstr(data_path), FSAM_WRITE, FSOM_CREATE_ALWAYS));

		storage_file_open(int_file, INT_PATH(".region_data"), FSAM_READ, FSOM_OPEN_EXISTING);
		furi_check(
		storage_file_copy_to_file(int_file, data_file, storage_file_size(int_file)));

		storage_file_close(int_file);
		storage_file_free(data_file);
	}

	/* Now, write new region data to internal storage finally! */
	furi_check(
	storage_file_open(int_file, INT_PATH(".region_data"), FSAM_WRITE, FSOM_CREATE_ALWAYS));

	do {
		size -= storage_file_write(int_file, buf, size);
	} while(size);

	storage_file_free(int_file);
	furi_record_close(RECORD_STORAGE);

	/* Clean up our memory */
	free(buf);
	pb_release(PB_Region_fields, &pb_region);


	/* Set up popup showing its been done */
	popup_reset(flick->popup);
	popup_set_callback(flick->popup, popup_callback);
	popup_set_context(flick->popup, NULL);
	popup_set_header(flick->popup,
			 "FLICK'ed!",
			 64,
			 2,
			 AlignCenter,
			 AlignTop);
	popup_set_text(flick->popup,
		       "FLICK complete!\nRebooting in 5 seconds\nto enable changes.",
		       64,
		       20,
		       AlignCenter,
		       AlignTop);
	//popup_set_icon?
	popup_set_timeout(flick->popup, 5000); // Calls callback aftre complete
	popup_enable_timeout(flick->popup);

	view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewPopup);
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
