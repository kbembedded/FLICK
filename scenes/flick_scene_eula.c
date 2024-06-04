#include <gui/scene_manager.h>
#include <gui/modules/text_box.h>

#include "flick_scene.h"
#include "../flick_data.h"

static char *eula = "Well, well, well. What have we here? Some long text to view? I have no idea how to handle button presses here";

static bool eula_handle_back(void *context)
{
	struct flick_app *flick = context;

	scene_manager_next_scene(flick->scene_manager, FlickScenePassword);

	return true;
}

void flick_scene_eula_on_enter(void *context)
{
	furi_assert(context);
	struct flick_app *flick = context;

	text_box_reset(flick->text_box);
	text_box_set_font(flick->text_box, TextBoxFontText);
	text_box_set_text(flick->text_box, eula);

	view_dispatcher_set_navigation_event_callback(flick->view_dispatcher,
						      eula_handle_back);
	view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewTextBox);
}

void flick_scene_eula_on_exit(void *context)
{
	UNUSED(context);
}

bool flick_scene_eula_on_event(void* context, SceneManagerEvent event)
{
	UNUSED(event);
	UNUSED(context);

	return false;
}
