// SPDX-License-Identifier: MIT
// Keyboard bindings.
// Copyright (C) 2023 Artem Senichev <artemsen@gmail.com>

#include "keybind.h"

#include "application.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Default key bindings
struct keybind_default {
    xkb_keysym_t key;     ///< Keyboard key
    uint8_t mods;         ///< Key modifiers
    struct action action; ///< Action
};

// clang-format off

/** Default key bindings for viewer mode. */
static const struct keybind_default default_viewer[] = {
    { .key = XKB_KEY_F1,          .action = { action_help, NULL } },
    { .key = XKB_KEY_Home,        .action = { action_first_file, NULL } },
    { .key = XKB_KEY_End,         .action = { action_last_file, NULL } },
    { .key = XKB_KEY_space,       .action = { action_next_file, NULL } },
    { .key = XKB_KEY_SunPageDown, .action = { action_next_file, NULL } },
    { .key = XKB_KEY_SunPageUp,   .action = { action_prev_file, NULL } },
    { .key = XKB_KEY_c,           .action = { action_skip_file, NULL } },
    { .key = XKB_KEY_d,           .action = { action_next_dir, NULL } },
    { .key = XKB_KEY_d, .mods = KEYMOD_SHIFT, .action = { action_prev_dir, NULL } },
    { .key = XKB_KEY_o,                       .action = { action_next_frame, NULL } },
    { .key = XKB_KEY_o, .mods = KEYMOD_SHIFT, .action = { action_prev_frame, NULL } },
    { .key = XKB_KEY_s,                       .action = { action_animation, NULL } },
    { .key = XKB_KEY_s, .mods = KEYMOD_SHIFT, .action = { action_slideshow, NULL } },
    { .key = XKB_KEY_f,           .action = { action_fullscreen, NULL } },
    { .key = XKB_KEY_Left,        .action = { action_step_left, NULL } },
    { .key = XKB_KEY_Right,       .action = { action_step_right, NULL } },
    { .key = XKB_KEY_Up,          .action = { action_step_up, NULL } },
    { .key = XKB_KEY_Down,        .action = { action_step_down, NULL } },
    { .key = XKB_KEY_equal,       .action = { action_zoom, "+10" } },
    { .key = XKB_KEY_plus,        .action = { action_zoom, "+10" } },
    { .key = XKB_KEY_minus,       .action = { action_zoom, "-10" } },
    { .key = XKB_KEY_w,           .action = { action_zoom, "width" } },
    { .key = XKB_KEY_w, .mods = KEYMOD_SHIFT, .action = { action_zoom, "height" } },
    { .key = XKB_KEY_z,                       .action = { action_zoom, "fit" } },
    { .key = XKB_KEY_z, .mods = KEYMOD_SHIFT, .action = { action_zoom, "fill" } },
    { .key = XKB_KEY_0,                       .action = { action_zoom, "real" } },
    { .key = XKB_KEY_BackSpace,               .action = { action_zoom, "optimal" } },
    { .key = XKB_KEY_bracketleft,             .action = { action_rotate_left, NULL } },
    { .key = XKB_KEY_bracketright,            .action = { action_rotate_right, NULL } },
    { .key = XKB_KEY_m,                       .action = { action_flip_vertical, NULL } },
    { .key = XKB_KEY_m, .mods = KEYMOD_SHIFT, .action = { action_flip_horizontal, NULL } },
    { .key = XKB_KEY_a,           .action = { action_antialiasing, NULL } },
    { .key = XKB_KEY_r,           .action = { action_reload, NULL } },
    { .key = XKB_KEY_i,           .action = { action_info, NULL } },
    { .key = XKB_KEY_Return,      .action = { action_mode, NULL } },
    { .key = XKB_KEY_Escape,      .action = { action_exit, NULL } },
    { .key = XKB_KEY_q,           .action = { action_exit, NULL } },
    { .key = VKEY_SCROLL_LEFT,    .action = { action_step_right, "5" } },
    { .key = VKEY_SCROLL_RIGHT,   .action = { action_step_left,  "5" } },
    { .key = VKEY_SCROLL_UP,      .action = { action_step_up,    "5" } },
    { .key = VKEY_SCROLL_DOWN,    .action = { action_step_down,  "5" } },
    { .key = VKEY_SCROLL_UP,   .mods = KEYMOD_CTRL,  .action = { action_zoom, "+10" } },
    { .key = VKEY_SCROLL_DOWN, .mods = KEYMOD_CTRL,  .action = { action_zoom, "-10" } },
    { .key = VKEY_SCROLL_UP,   .mods = KEYMOD_SHIFT, .action = { action_prev_file, NULL } },
    { .key = VKEY_SCROLL_DOWN, .mods = KEYMOD_SHIFT, .action = { action_next_file, NULL } },
    { .key = VKEY_SCROLL_UP,   .mods = KEYMOD_ALT,   .action = { action_prev_frame, NULL } },
    { .key = VKEY_SCROLL_DOWN, .mods = KEYMOD_ALT,   .action = { action_next_frame, NULL } },
    { .key = XKB_KEY_Delete, .mods = KEYMOD_SHIFT, .action = { action_none, NULL } },
};

/** Default key bindings for gallery mode. */
static const struct keybind_default default_gallery[] = {
    { .key = XKB_KEY_F1,          .action = { action_help, NULL } },
    { .key = XKB_KEY_Home,        .action = { action_first_file, NULL } },
    { .key = XKB_KEY_End,         .action = { action_last_file, NULL } },
    { .key = XKB_KEY_f,           .action = { action_fullscreen, NULL } },
    { .key = XKB_KEY_Left,        .action = { action_step_left, NULL } },
    { .key = XKB_KEY_Right,       .action = { action_step_right, NULL } },
    { .key = XKB_KEY_Up,          .action = { action_step_up, NULL } },
    { .key = XKB_KEY_Down,        .action = { action_step_down, NULL } },
    { .key = XKB_KEY_Prior,       .action = { action_page_up, NULL } },
    { .key = XKB_KEY_Next,        .action = { action_page_down, NULL } },
    { .key = XKB_KEY_c,           .action = { action_skip_file, NULL } },
    { .key = XKB_KEY_a,           .action = { action_antialiasing, NULL } },
    { .key = XKB_KEY_r,           .action = { action_reload, NULL } },
    { .key = XKB_KEY_i,           .action = { action_info, NULL } },
    { .key = XKB_KEY_Return,      .action = { action_mode, NULL } },
    { .key = XKB_KEY_Escape,      .action = { action_exit, NULL } },
    { .key = XKB_KEY_q,           .action = { action_exit, NULL } },
    { .key = VKEY_SCROLL_LEFT,    .action = { action_step_right, NULL } },
    { .key = VKEY_SCROLL_RIGHT,   .action = { action_step_left,  NULL } },
    { .key = VKEY_SCROLL_UP,      .action = { action_step_up,    NULL } },
    { .key = VKEY_SCROLL_DOWN,    .action = { action_step_down,  NULL } },
    { .key = XKB_KEY_Delete, .mods = KEYMOD_SHIFT, .action = { action_none, NULL } },
};
// clang-format on

// Names of virtual keys
struct virtual_keys {
    xkb_keysym_t key;
    const char* name;
};
static const struct virtual_keys virtual_keys[] = {
    { VKEY_SCROLL_UP,    "ScrollUp"    },
    { VKEY_SCROLL_DOWN,  "ScrollDown"  },
    { VKEY_SCROLL_LEFT,  "ScrollLeft"  },
    { VKEY_SCROLL_RIGHT, "ScrollRight" },
    { VKEY_MOUSE_LEFT,   "MouseLeft"   },
    { VKEY_MOUSE_RIGHT,  "MouseRight"  },
    { VKEY_MOUSE_MIDDLE, "MouseMiddle" },
    { VKEY_MOUSE_SIDE,   "MouseSide"   },
    { VKEY_MOUSE_EXTRA,  "MouseExtra"  },
};

/** Head of global key binding list. */
static struct keybind* kb_viewer;
static struct keybind* kb_gallery;

/**
 * Convert text name to key code with modifiers.
 * @param name key text name
 * @param key output keyboard key
 * @param mods output key modifiers (ctrl/alt/shift)
 * @return false if name is invalid
 */
static bool parse_keymod(const char* name, xkb_keysym_t* key, uint8_t* mods)
{
    struct str_slice slices[4]; // mod[alt+ctrl+shift]+key
    const size_t snum = str_split(name, '+', slices, ARRAY_SIZE(slices));
    if (snum == 0) {
        return false;
    }

    // get modifiers
    *mods = 0;
    for (size_t i = 0; i < snum - 1; ++i) {
        const char* mod_names[] = { "Ctrl", "Alt", "Shift" };
        const ssize_t index =
            str_index(mod_names, slices[i].value, slices[i].len);
        if (index < 0) {
            return false;
        }
        *mods |= 1 << index;
    }

    // get key
    *key = xkb_keysym_from_name(slices[snum - 1].value,
                                XKB_KEYSYM_CASE_INSENSITIVE);
    // check for virtual keys
    if (*key == XKB_KEY_NoSymbol) {
        for (size_t i = 0; i < ARRAY_SIZE(virtual_keys); ++i) {
            if (strcmp(slices[snum - 1].value, virtual_keys[i].name) == 0) {
                *key = virtual_keys[i].key;
                break;
            }
        }
    }
    // check for international symbols
    if (*key == XKB_KEY_NoSymbol) {
        wchar_t* wide = str_to_wide(slices[snum - 1].value, NULL);
        *key = xkb_utf32_to_keysym(wide[0]);
        free(wide);
    }

    return (*key != XKB_KEY_NoSymbol);
}

/**
 * Allocate new key binding.
 * @param key keyboard key
 * @param mods key modifiers (ctrl/alt/shift)
 * @param actions sequence of actions
 */
static struct keybind* create_binding(xkb_keysym_t key, uint8_t mods,
                                      struct action_seq* actions)
{
    struct keybind* kb = calloc(1, sizeof(struct keybind));
    if (!kb) {
        return NULL;
    }

    kb->key = key;
    kb->mods = mods;
    kb->actions = *actions;

    // construct help description
    if (kb->actions.sequence[0].type != action_none) {
        size_t max_len = 30;
        char* key_name = keybind_name(kb->key, kb->mods);
        if (key_name) {
            const struct action* action =
                &kb->actions.sequence[0]; // first only
            str_append(key_name, 0, &kb->help);
            str_append(": ", 0, &kb->help);
            str_append(action_typename(action), 0, &kb->help);
            if (action->params) {
                str_append(" ", 0, &kb->help);
                str_append(action->params, 0, &kb->help);
            }
            if (kb->actions.num > 1) {
                str_append("; ...", 0, &kb->help);
            }
            if (strlen(kb->help) > max_len) {
                const char* ellipsis = "...";
                const size_t ellipsis_len = strlen(ellipsis);
                memcpy(&kb->help[max_len - ellipsis_len], ellipsis,
                       ellipsis_len + 1);
            }
            free(key_name);
        }
    }

    return kb;
}

/**
 * Free key binding.
 * @param kb key binding
 */
static void free_binding(struct keybind* kb)
{
    if (kb) {
        action_free(&kb->actions);
        free(kb->help);
        free(kb);
    }
}

/**
 * Put key binding to the global scheme.
 * @param kb head of binding list
 * @param key keyboard key
 * @param mods key modifiers (ctrl/alt/shift)
 * @param actions sequence of actions
 */
static void set_binding(struct keybind** head, xkb_keysym_t key, uint8_t mods,
                        struct action_seq* actions)
{
    struct keybind* kb;

    // remove existing binding
    list_for_each(*head, struct keybind, it) {
        if (it->key == key && it->mods == mods) {
            *head = list_remove(&it->list);
            free_binding(it);
            break;
        }
    }

    // create new binding
    kb = create_binding(key, mods, actions);
    if (kb) {
        *head = list_add(*head, kb);
    } else {
        action_free(actions);
    }
}

/**
 * Create default binding.
 * @param head head of binding list
 * @param kb default key binding description
 */
static void set_default(struct keybind** head, const struct keybind_default* kb)
{
    struct action_seq actions;

    if (kb->action.type == action_none && kb->key == XKB_KEY_Delete &&
        kb->mods == KEYMOD_SHIFT) {
        // special action: Shift+Del
        actions.num = 2;
        actions.sequence = malloc(actions.num * sizeof(struct action));
        if (actions.sequence) {
            actions.sequence[0].type = action_exec;
            actions.sequence[0].params = str_dup("rm \"%\"", NULL);
            actions.sequence[1].type = action_skip_file;
            actions.sequence[1].params = NULL;
        }
    } else {
        actions.num = 1;
        actions.sequence = malloc(actions.num * sizeof(struct action));
        if (actions.sequence) {
            actions.sequence[0].type = kb->action.type;
            if (kb->action.params) {
                actions.sequence[0].params = str_dup(kb->action.params, NULL);
            } else {
                actions.sequence[0].params = NULL;
            }
        }
    }

    if (actions.sequence) {
        set_binding(head, kb->key, kb->mods, &actions);
    }
}

/**
 * Load binding from config parameters.
 * @param kb head of binding list
 * @param cfg config instance
 * @param section name of the section
 * @param value actions
 */
static void load_binding(struct keybind** head, struct config* cfg,
                         const char* section)
{
    list_for_each(cfg, struct config, cs) {
        if (strcmp(section, cs->name) == 0) {
            list_for_each(cs->params, struct config_keyval, kv) {
                struct action_seq actions = { 0 };
                xkb_keysym_t keysym;
                uint8_t mods;

                kv->used = true;

                // parse keyboard shortcut
                if (!parse_keymod(kv->key, &keysym, &mods)) {
                    config_error_key(section, kv->key);
                    continue;
                }
                // parse actions
                if (!action_create(kv->value, &actions)) {
                    config_error_val(section, kv->value);
                    continue;
                }

                set_binding(head, keysym, mods, &actions);
            }
            break;
        }
    }
}

void keybind_init(struct config* cfg)
{
    // create default bindings
    for (size_t i = 0; i < ARRAY_SIZE(default_viewer); ++i) {
        set_default(&kb_viewer, &default_viewer[i]);
    }
    for (size_t i = 0; i < ARRAY_SIZE(default_gallery); ++i) {
        set_default(&kb_gallery, &default_gallery[i]);
    }

    // load bindings from config
    load_binding(&kb_viewer, cfg, "keys.viewer");
    load_binding(&kb_gallery, cfg, "keys.gallery");
}

void keybind_destroy(void)
{
    list_for_each(kb_viewer, struct keybind, it) {
        free_binding(it);
    }
    kb_viewer = NULL;
    list_for_each(kb_gallery, struct keybind, it) {
        free_binding(it);
    }
    kb_gallery = NULL;
}

struct keybind* keybind_get(void)
{
    return app_is_viewer() ? kb_viewer : kb_gallery;
}

struct keybind* keybind_find(xkb_keysym_t key, uint8_t mods)
{
    // we always use lowercase + Shift modifier
    key = xkb_keysym_to_lower(key);

    list_for_each(keybind_get(), struct keybind, it) {
        if (it->key == key && it->mods == mods) {
            return it;
        }
    }

    return NULL;
}

char* keybind_name(xkb_keysym_t key, uint8_t mods)
{
    char key_name[32];
    char* name = NULL;

    // skip modifiers
    switch (key) {
        case XKB_KEY_Super_L:
        case XKB_KEY_Super_R:
        case XKB_KEY_Shift_L:
        case XKB_KEY_Shift_R:
        case XKB_KEY_Control_L:
        case XKB_KEY_Control_R:
        case XKB_KEY_Meta_L:
        case XKB_KEY_Meta_R:
        case XKB_KEY_Alt_L:
        case XKB_KEY_Alt_R:
            return NULL;
    }

    // modifiers
    if (mods & KEYMOD_CTRL) {
        str_append("Ctrl+", 0, &name);
    }
    if (mods & KEYMOD_ALT) {
        str_append("Alt+", 0, &name);
    }
    if (mods & KEYMOD_SHIFT) {
        str_append("Shift+", 0, &name);
    }

    // key name
    if (xkb_keysym_get_name(key, key_name, sizeof(key_name)) > 0) {
        str_append(key_name, 0, &name);
    } else {
        // search in virtual keys
        bool found = false;
        for (size_t i = 0; !found && i < ARRAY_SIZE(virtual_keys); ++i) {
            found = (virtual_keys[i].key == key);
            if (found) {
                str_append(virtual_keys[i].name, 0, &name);
            }
        }
        if (!found) {
            str_append("<UNKNOWN>", 0, &name);
        }
    }

    return name;
}

uint8_t keybind_mods(struct xkb_state* state)
{
    uint8_t mods = 0;

    if (xkb_state_mod_name_is_active(state, XKB_MOD_NAME_CTRL,
                                     XKB_STATE_MODS_EFFECTIVE) > 0) {
        mods |= KEYMOD_CTRL;
    }
    if (xkb_state_mod_name_is_active(state, XKB_MOD_NAME_ALT,
                                     XKB_STATE_MODS_EFFECTIVE) > 0) {
        mods |= KEYMOD_ALT;
    }
    if (xkb_state_mod_name_is_active(state, XKB_MOD_NAME_SHIFT,
                                     XKB_STATE_MODS_EFFECTIVE) > 0) {
        mods |= KEYMOD_SHIFT;
    }

    return mods;
}
