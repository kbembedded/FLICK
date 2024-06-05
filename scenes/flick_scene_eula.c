#include <gui/scene_manager.h>
#include <gui/modules/text_box.h>

#include "flick_scene.h"
#include "../flick_data.h"

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

static char *eula = "End User License Agreement (EULA) for Frequency Liberation and Interchangeable Country Kit for Flipper Zero (FLICK)\n\nWelcome to the grand odyssey that is the Frequency Liberation and Interchangeable Country Kit for Flipper Zero (hereinafter referred to as \"FLICK\"). Before we embark on this whimsical journey, we must lay down some groundwork with this End User License Agreement (EULA). So, grab a cup of your favorite beverage and settle in for a rollercoaster of legal jargon, absurdity, and a sprinkle of nonsensical gibberish.\n\n1. Acceptance of Terms\nBy downloading, installing, or using FLICK, you, the esteemed user, agree to be bound by the terms of this EULA. If you do not agree to these terms, please refrain from interacting with this software. Seriously, just stop right here. Otherwise, prepare to dive into a world of bewilderment and mayhem.\n\n2. Disclaimer of Responsibility\nThe author of FLICK, who shall remain a mysterious entity cloaked in shadows, is not, under any circumstances, responsible for the use, misuse, or miraculous events that occur as a result of using this software. This includes, but is not limited to, the spontaneous combustion of your Flipper hardware, the sudden onset of sentience in your electronic devices, or any damages that may arise from your attempts to communicate with extraterrestrial beings via FLICK. If your Flipper explodes, melts, or turns into a sentient being that demands its own Netflix account, that's on you.\n\n3. Grant of License\nFLICK is provided to you, the user, as a whimsical gift from the digital deities. You are hereby granted a non-exclusive, non-transferable, and absolutely absurd license to use FLICK on your Flipper Zero device. This license is effective until the end of time or until you decide to delete the software, whichever comes first. And let's be real, who knows when the end of time actually is?\n\n4. Usage Restrictions\nYou agree not to use FLICK for any purpose that could be deemed sensible, rational, or otherwise grounded in reality. Specifically, you shall not:\n   a. Use FLICK to hack into government databases, unless you have a really good reason and a note from your mom.\n   b. Employ FLICK to control your neighbor's toaster, unless they specifically ask you to do so in writing. And even then, maybe think twice.\n   c. Attempt to use FLICK to summon ancient deities or mythical creatures, as this could result in catastrophic consequences, including but not limited to, being eaten alive or turned into a newt.\n\n5. Warranties\nFLICK is provided \"as is\" with no warranties of any kind, either express or implied. This includes, but is not limited to, warranties of merchantability, fitness for a particular purpose, or the likelihood that using FLICK will make you more attractive to potential romantic partners. Any resemblance of FLICK to actual software, living or dead, is purely coincidental. If FLICK turns your Flipper into a useless brick or a sentient overlord, don't come crying to us.\n\n6. Indemnification\nBy using FLICK, you agree to indemnify, defend, and hold harmless the author from any claims, damages, or other liabilities arising from your use or misuse of the software. This includes, but is not limited to, legal fees, the cost of replacing your Flipper device, and the price of therapy sessions needed to cope with the existential crisis that may arise from interacting with FLICK. If you end up in a legal battle with a government agency or an alien race, that’s your problem.\n\n7. Password to Continue\nAh, the moment you've been waiting for! To continue to FLICK your Flipper after reading this EULA, you must enter the sacred password. The password, dear user, is \""ATCGTAG TAGCGATCA CTAGCTAG GCATCGAT"\" (without quotes). Type it with reverence and proceed with caution.\n\n8. Governing Law\nThis EULA shall be governed by the laws of the Land of Make-Believe, without regard to its conflict of law principles. Any disputes arising under or in connection with this EULA shall be resolved through a duel at dawn, using rubber chickens as the weapon of choice. If rubber chickens are unavailable, sock puppets will be acceptable.\n\n9. Severability\nIf any provision of this EULA is found to be unenforceable or invalid, the remaining provisions shall remain in full force and effect. This EULA is like a hydra—cut off one clause, and two more shall take its place. Don't try to understand it; just accept it and move on.\n\n10. Termination\nThis EULA is effective until terminated by either party. You may terminate it at any time by uninstalling and destroying all copies of FLICK in your possession. The author may terminate this EULA immediately, without notice, if you breach any term or if the author just feels like it. If the author wakes up on the wrong side of the bed, that might be enough reason.\n\n11. Entire Agreement\nThis EULA constitutes the entire agreement between you and the author regarding FLICK and supersedes all prior understandings, agreements, or communications, whether written or oral, concerning the subject matter. If you thought there was something else, you were wrong.\n\n12. Miscellaneous Absurdities\nIn the event of a zombie apocalypse, FLICK may be used to send distress signals, provided you have the appropriate firmware and a sense of impending doom. Additionally, FLICK is not certified for use in time travel, parallel universe exploration, or as a flotation device. If you use FLICK to attempt to communicate with underwater civilizations, you're on your own.\n\n13. Final Instructions\nCongratulations on making it this far! To complete your journey through this EULA, please press the Back button on your device to continue. Yes, you read that correctly. Press the Back button. Why? Because life is full of surprises, and this EULA is no exception.\n\nThank you for embarking on this adventure with FLICK. May your frequencies be ever liberated and your Flipper Zero ever functional, despite the odds. If you encounter any issues, just remember: we warned you.\n";

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
