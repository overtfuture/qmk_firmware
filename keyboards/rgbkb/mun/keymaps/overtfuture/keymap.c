#include QMK_KEYBOARD_H
#include "common_oled.h"

enum keymap_layers {
    _QWERTY,
    _FN
};

enum keymap_keycodes {
    // Disables touch processing
    TCH_TOG = SAFE_RANGE,
    MENU_BTN,
    MENU_UP,
    MENU_DN
};

// Default Layers
#define QWERTY   DF(_QWERTY)

// Momentary Layers
#define FN       MO(_FN)

#define FN_CAPS  LT(_FN, KC_CAPS)
#define FN_ESC   LT(_FN, KC_ESC)

/* This keyboard is enabled with an RGB Menu Control system.
This functionality is enabled, but still requires a little configuration based on your exact setup.
The RGB Menu will appear on the Right Half's OLED and can be controlled by the MENU keycodes:
MENU_BTN - Triggers a button action for the menu
MENU_UP - Triggers an increase action for the menu
MENU_DN - Triggers a decrease action for the menu

To finish configuration for your board, you will want to change the default keycodes for an encoder on the right half.
Encoder press keycode should be set to MENU_BTN, Clockwise should be MENU_UP, and Counter Clockwise should be MENU_DN.
Depending on where you add an encoder to the right half will determin in the default keymap where you should put those keycodes.
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______,       _______,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    _______,       _______,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    _______,       _______,   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______,       _______,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        FN,      KC_LCTL, KC_LALT, KC_LGUI, KC_SPC,  KC_SPC,  FN,            FN,        KC_SPC,  KC_SPC,  KC_RGUI, KC_RALT, KC_RCTL, FN,

        KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, KC_MPRV,                                             RGB_HUI, RGB_HUD, RGB_RMOD,RGB_TOG, RGB_MOD
    ),

    [_FN] = LAYOUT(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,       _______,  KC_F6,   KC_F7,   KC_F8,  KC_F9,   KC_F10,  KC_DEL,
        _______, KC_MINS, KC_EQL , KC_LPRN, KC_RPRN, _______, _______,       _______, _______, _______, _______, _______, _______, _______,
        _______, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, KC_GRV , _______,       _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_QUOT, _______,
        _______, _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, _______,
        QK_BOOT, _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, QK_BOOT,

        KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, KC_MPRV,                                             RGB_HUI, RGB_HUD, RGB_RMOD,RGB_TOG, RGB_MOD
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode)
    {
        case MENU_BTN:
            if (record->event.pressed) {
                rgb_menu_selection();
            }
            return false;
        case MENU_UP:
            if (record->event.pressed) {
                rgb_menu_action(true);
            }
            return false;
        case MENU_DN:
            if (record->event.pressed) {
                rgb_menu_action(false);
            }
            return false;
        case TCH_TOG:
            if (record->event.pressed) {
                touch_encoder_toggle();
            }
            return false;  // Skip all further processing of this key
        default:
            return true;
    }
}

static void render_layer(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer"), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("QWRTY"), false);
            break;
        case _FN:
            oled_write_ln_P(PSTR("FN   "), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
}

static void render_leds(void)
{
    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUMLK")     : PSTR("     "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAPLK")    : PSTR("     "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCRLK")  : PSTR("     "), false);
}

static void render_touch(void)
{
    // Host Touch LED Status
    oled_write_P(touch_encoder_is_on() ? PSTR("TOUCH")  : PSTR("     "), false);
    oled_write_P(touch_encoder_is_calibrating() ? PSTR("CLBRT")  : PSTR("     "), false);
}

bool oled_task_user(void) {
    if (is_keyboard_left()) {
        render_icon();
        oled_write_P(PSTR("     "), false);
        render_layer();
        oled_write_P(PSTR("     "), false);
        render_leds();
        oled_write_P(PSTR("     "), false);
        render_touch();
    }
    else {
        render_icon();
        oled_write_P(PSTR("     "), false);
        render_rgb_menu();
    }
    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =       { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [1] =       { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) }
};
#endif