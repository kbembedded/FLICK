#include <furi.h>

/* GUI handling */
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/loading.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_box.h>
#include <gui/modules/text_input.h>
#include <storage/storage.h>

/* Main application struct */
#include "flick_data.h"

/* Scene list produced by overly-complex-but-still-probably-the-best-way-to-
 * maintain macro hell.
 */
#include "scenes/flick_scene.h"

struct flick_app *flick_alloc(void)
{
	struct flick_app *flick = malloc(sizeof(struct flick_app));
	Storage *storage;

	/* View dispatcher setup
	 * This probably isn't needed since we're using scenes mostly, but, I
	 * already know this paradigm and its easy to spit out.
	 */
	flick->view_dispatcher = view_dispatcher_alloc();
	view_dispatcher_enable_queue(flick->view_dispatcher);
	view_dispatcher_set_event_callback_context(flick->view_dispatcher, flick);
	view_dispatcher_attach_to_gui(flick->view_dispatcher,
				      (Gui *)furi_record_open(RECORD_GUI),
				      ViewDispatcherTypeFullscreen);

	/* Submenu */
	flick->submenu = submenu_alloc();
	view_dispatcher_add_view(flick->view_dispatcher,
				 FlickViewMenu,
				 submenu_get_view(flick->submenu));

	/* Popup */
	flick->popup = popup_alloc();
	view_dispatcher_add_view(flick->view_dispatcher,
				 FlickViewPopup,
				 popup_get_view(flick->popup));

	/* DialogEx */
	flick->dialog_ex = dialog_ex_alloc();
	view_dispatcher_add_view(flick->view_dispatcher,
				 FlickViewDialogEx,
				 dialog_ex_get_view(flick->dialog_ex));

	/* TextBox */
	flick->text_box = text_box_alloc();
	view_dispatcher_add_view(flick->view_dispatcher,
				 FlickViewTextBox,
				 text_box_get_view(flick->text_box));

	/* TextInput */
	flick->text_input = text_input_alloc();
	view_dispatcher_add_view(flick->view_dispatcher,
				 FlickViewTextInput,
				 text_input_get_view(flick->text_input));

	/* Loading animation */
	flick->loading = loading_alloc();
	view_dispatcher_add_view(flick->view_dispatcher,
				 FlickViewLoading,
				 loading_get_view(flick->loading));

	/* Path for data folder */
	storage = furi_record_open(RECORD_STORAGE);
	flick->data_path = furi_string_alloc_set(APP_DATA_PATH());
	storage_common_resolve_path_and_ensure_app_directory(storage, flick->data_path);
	furi_record_close(RECORD_STORAGE);

	/* Scene management */
	flick->scene_manager = scene_manager_alloc(&flick_scene_handlers, flick);
	scene_manager_next_scene(flick->scene_manager, FlickSceneMainMenu);

	return flick;
}

void flick_free(struct flick_app *flick)
{
	/* Asset folder path */
	furi_string_free(flick->data_path);

	/* Loading animation */
	view_dispatcher_remove_view(flick->view_dispatcher, FlickViewLoading);
	loading_free(flick->loading);

	/* TextInput */
	view_dispatcher_remove_view(flick->view_dispatcher, FlickViewTextInput);
	text_input_free(flick->text_input);

	/* TextBox */
	view_dispatcher_remove_view(flick->view_dispatcher, FlickViewTextBox);
	text_box_free(flick->text_box);

	/* DiaglogEx */
	view_dispatcher_remove_view(flick->view_dispatcher, FlickViewDialogEx);
	dialog_ex_free(flick->dialog_ex);

	/* Popup */
	view_dispatcher_remove_view(flick->view_dispatcher, FlickViewPopup);
	popup_free(flick->popup);

	/* SubMenu */
	view_dispatcher_remove_view(flick->view_dispatcher, FlickViewMenu);
	submenu_free(flick->submenu);

	/* ViewDispatcher */
	view_dispatcher_free(flick->view_dispatcher);

	/* Path for data folder */
	furi_string_free(flick->data_path);

	/* Scene Manager */
	scene_manager_free(flick->scene_manager);

	free(flick);
}

int32_t flick_app(void* p) {
	UNUSED(p);

	struct flick_app *flick;
	flick = flick_alloc();

	view_dispatcher_run(flick->view_dispatcher);

	flick_free(flick);

	return 0;
}
