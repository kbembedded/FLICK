#include <gui/modules/dialog_ex.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <storage/storage.h>

#include "flick_scene.h"
#include "../flick_data.h"

bool flick_scene_check_flicked(struct flick_app *flick)
{
	bool ret;

	Storage *storage = NULL;
	FuriString *path = NULL;

	/* Check if a backup of region data exists */
	storage = furi_record_open(RECORD_STORAGE);
	path = furi_string_alloc_set(flick->data_path);
	furi_string_cat_str(path, ".region_data");
	FURI_LOG_D("FLICK", "Checking for existing backup at \'%s\'", furi_string_get_cstr(path));

	ret = storage_file_exists(storage, furi_string_get_cstr(path));
	furi_record_close(RECORD_STORAGE);
	furi_string_free(path);
	
	return ret;
}



static void dialog_result(DialogExResult result, void* context)
{
	struct flick_app *flick = context;

	if (result == DialogExResultLeft) {
		scene_manager_search_and_switch_to_previous_scene(flick->scene_manager,
								  FlickSceneMainMenu);
	}
	if (result == DialogExResultRight) {
		scene_manager_next_scene(flick->scene_manager, FlickSceneEULA);
	}							
}

void flick_scene_check_flicked_on_enter(void *context)
{
	furi_assert(context);
	struct flick_app *flick = context;
	if (flick_scene_check_flicked(flick)) {
		dialog_ex_reset(flick->dialog_ex);
		dialog_ex_set_result_callback(flick->dialog_ex, dialog_result);
		dialog_ex_set_context(flick->dialog_ex, flick);
		dialog_ex_set_header(flick->dialog_ex,
				     "Warning!",
				     64,
				     0,
				     AlignCenter,
				     AlignTop);
		dialog_ex_set_text(flick->dialog_ex,
				   "Continuing will result in the\nexisting backup region_data\nbeing clobbered, are you sure?",
				   64,
				   12,
				   AlignCenter,
				   AlignTop);
		dialog_ex_set_left_button_text(flick->dialog_ex, "Back");
		dialog_ex_set_right_button_text(flick->dialog_ex, "Continue");

		view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewDialogEx);
	} else {
		scene_manager_next_scene(flick->scene_manager, FlickSceneEULA);
	}
}

void flick_scene_check_flicked_on_exit(void *context)
{
	UNUSED(context);
}

bool flick_scene_check_flicked_on_event(void* context, SceneManagerEvent event)
{
	UNUSED(context);
	UNUSED(event);

	return false;
}
