#include <power/power_service/power.h>
#include <storage/storage.h>

#include "flick_scene.h"
#include "../flick_data.h"

static void popup_callback(void *context)
{
	UNUSED(context);

	power_reboot(PowerBootModeNormal);
}

void flick_scene_un_flick_on_enter(void *context)
{
	furi_assert(context);
	struct flick_app *flick = context;
	Storage *storage = NULL;
	File *int_file = NULL;
	File *data_file = NULL;
	FuriString *data_path = NULL;

	view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewLoading);

	/* Restore original region_data from sd to internal */
	storage = furi_record_open(RECORD_STORAGE);

	int_file = storage_file_alloc(storage);
	data_file = storage_file_alloc(storage);

	furi_check(
	storage_file_open(int_file, INT_PATH(".region_data"), FSAM_WRITE, FSOM_CREATE_ALWAYS));

	data_path = furi_string_alloc_set(flick->data_path);
	furi_string_cat_str(data_path, ".region_data");
	FURI_LOG_D("FLICK", "Copying region data from \'%s\'", furi_string_get_cstr(data_path));
	furi_check(
	storage_file_open(data_file, furi_string_get_cstr(data_path), FSAM_READ_WRITE, FSOM_OPEN_EXISTING));

	furi_check(
	storage_file_copy_to_file(data_file, int_file, storage_file_size(data_file)));

	storage_file_free(int_file);

	/* Remove the file in data, it must first be closed */
	storage_file_close(data_file);
	storage_simply_remove(storage, furi_string_get_cstr(data_path));

	/* Clean up */
	storage_file_free(data_file);
	furi_record_close(RECORD_STORAGE);

	/* Set up popup showing its been done */
	popup_reset(flick->popup);
	popup_set_callback(flick->popup, popup_callback);
	popup_set_context(flick->popup, NULL);
	popup_set_header(flick->popup,
			 "un-FLICK'ed!",
			 64,
			 2,
			 AlignCenter,
			 AlignTop);
	popup_set_text(flick->popup,
		       "un-FLICK complete!\nRebooting in 5 seconds\nto enable changes.",
		       64,
		       20,
		       AlignCenter,
		       AlignTop);
	//popup_set_icon?
	popup_set_timeout(flick->popup, 5000); // Calls callback aftre complete
	popup_enable_timeout(flick->popup);

	view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewPopup);
}

void flick_scene_un_flick_on_exit(void *context)
{
	UNUSED(context);
}

bool flick_scene_un_flick_on_event(void* context, SceneManagerEvent event)
{
	UNUSED(context);
	UNUSED(event);

	return false;
}
