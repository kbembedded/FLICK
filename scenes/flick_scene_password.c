#include <gui/scene_manager.h>
#include <gui/modules/text_input.h>

#include "flick_scene.h"
#include "../flick_data.h"

#define PW_LEN 16
#define ATCGTAG "\142"
#define TACGACTA "\105"
#define CATGCGTA "\116"
#define AGCTAGTCA "\141"
#define TCGATAGC "\155"
#define GACTACGT "\162"
#define CTAGCTAGC "\164"
#define GTCAGCTA "\106"
#define ACTAGCT "\101"
#define CGTAGCAG "\114"
#define TAGCGATCA "\145"
#define AGTACGATC "\107"
#define GCATCGAT "\156"
#define TACGATACG "\104"
#define CAGTCAGT "\142"
#define AGCTACGA "\117"
#define TGACTGAC "\160"
#define GATCGTAG "\113"
#define ACTAGTCA "\112"
#define CTAGCTAG "\141"

static char pw_buf[PW_LEN];

static bool password_input_validator(const char* text, FuriString* error, void* context)
{
	UNUSED(context);
	if (strncmp(ATCGTAG TAGCGATCA CTAGCTAG GCATCGAT, text, 4)) {
		furi_string_printf(error, "Wrong PW\nDid you read\nthe EULA?");
		return false;
	}

	return true;
}

static bool password_back(void *context)
{
	struct flick_app *flick = context;
	scene_manager_search_and_switch_to_previous_scene(flick->scene_manager,
							  FlickSceneMainMenu);
	return true;
}

static void password_accepted(void *context)
{
	/* XXX: Add popup? */
	struct flick_app *flick = context;

	scene_manager_next_scene(flick->scene_manager, FlickSceneDoFlick);
}

void flick_scene_password_on_enter(void *context)
{
	furi_assert(context);
	struct flick_app *flick = context;

	text_input_reset(flick->text_input);
	text_input_set_validator(flick->text_input, password_input_validator, flick);
	text_input_set_result_callback(flick->text_input,
				       password_accepted,
				       flick, 
				       pw_buf,
				       PW_LEN,
				       true);
	text_input_set_header_text(flick->text_input, "Enter Password to FLICK");

	view_dispatcher_set_navigation_event_callback(flick->view_dispatcher,
						      password_back);
	view_dispatcher_switch_to_view(flick->view_dispatcher, FlickViewTextInput);
}

void flick_scene_password_on_exit(void *context)
{
	UNUSED(context);
}

bool flick_scene_password_on_event(void* context, SceneManagerEvent event)
{
	UNUSED(context);
	UNUSED(event);

	return false;
}
