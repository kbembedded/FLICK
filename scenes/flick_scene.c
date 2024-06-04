#include "flick_scene.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const flick_on_enter_handlers[])(void*) = {
#include "flick_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const flick_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "flick_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const flick_on_exit_handlers[])(void* context) = {
#include "flick_scene_config.h"
};
#undef ADD_SCENE

// Initialize scene handlers configuration structure
const SceneManagerHandlers flick_scene_handlers = {
    .on_enter_handlers = flick_on_enter_handlers,
    .on_event_handlers = flick_on_event_handlers,
    .on_exit_handlers = flick_on_exit_handlers,
    .scene_num = FlickSceneNum,
};
