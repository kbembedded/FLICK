#include "flick_scene.h"
#include "../flick_data.h"

#include "flick_scene_check_flicked.h"

static void callback(void* context, uint32_t index)
{
	struct flick_app *flick = context;

	scene_manager_next_scene(flick->scene_manager, index);
}

/* Back event handler for scene manager */
static bool flick_main_menu_back_event(void *context)
{
	furi_assert(context);
	struct flick_app *flick = context;

	return scene_manager_handle_back_event(flick->scene_manager);
}

void flick_scene_main_menu_on_enter(void *context)
{
	furi_assert(context);
	struct flick_app *flick = context;

	submenu_reset(flick->submenu);
	submenu_add_item(flick->submenu,
			 "FLICK",
			 FlickSceneCheckFlicked,
			 callback,
			 flick);
	if (flick_scene_check_flicked(flick)) {
		submenu_add_item(flick->submenu,
				 "un-FLICK",
				 FlickSceneUnFlick,
				 callback,
				 flick);
	}

	view_dispatcher_set_navigation_event_callback(flick->view_dispatcher,
						      flick_main_menu_back_event);

	view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewMenu);
}

void flick_scene_main_menu_on_exit(void *context)
{
	UNUSED(context);
}

bool flick_scene_main_menu_on_event(void* context, SceneManagerEvent event)
{
	UNUSED(context);
	UNUSED(event);

	return false;
}
