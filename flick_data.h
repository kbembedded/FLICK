#ifndef __FLICK_DATA_H__
#define __FLICK_DATA_H__

#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/loading.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_box.h>
#include <gui/modules/text_input.h>

struct flick_app {
	ViewDispatcher *view_dispatcher;
	SceneManager *scene_manager;
	DialogEx *dialog_ex;
	Popup *popup;
	Submenu *submenu;
	TextBox *text_box;
	TextInput *text_input;
	Loading *loading;
	FuriString *data_path;
};

/* Views */
enum {
	FlickViewMenu,
	FlickViewDialogEx,
	FlickViewPopup,
	FlickViewTextBox,
	FlickViewTextInput,
	FlickViewLoading,
	FlickViewNum
};

#endif // __FLICK_DATA_H__
