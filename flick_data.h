#ifndef __FLICK_DATA_H__
#define __FLICK_DATA_H__

#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_box.h>
#include <gui/modules/text_input.h>

struct flick_app {
	ViewDispatcher *view_dispatcher;
	SceneManager *scene_manager;
	DialogEx *dialog_ex;
	Submenu *submenu;
	TextBox *text_box;
	TextInput *text_input;
};

/* Views */
enum {
	FlickViewMenu,
	FlickViewDialogEx,
	FlickViewTextBox,
	FlickViewTextInput,
	FlickViewNum
};

#endif // __FLICK_DATA_H__
