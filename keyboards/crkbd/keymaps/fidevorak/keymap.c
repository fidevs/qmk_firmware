/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

// Initialize variable holding the binary
// representation of active modifiers.
uint8_t mod_state;

enum layers { _DVORAK, _LOWER, _RAISE, _ADJUST };

enum my_keycodes { RMT = SAFE_RANGE, RMS, RMIH, RMDH, RMIS, RMDS, RMIV, RMDV };
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Store the current modifier state in the variable for later reference
    mod_state = get_mods();

    switch (keycode) {
        case RMT:
            if (record->event.pressed) {
                rgb_matrix_toggle();
            }
            return false;
        case RMS:
            if (record->event.pressed) {
                rgb_matrix_step();
            }
            return false;
        case RMIH:
            if (record->event.pressed) {
                rgb_matrix_increase_hue();
            }
            return false;
        case RMDH:
            if (record->event.pressed) {
                rgb_matrix_decrease_hue();
            }
            return false;
        case RMIS:
            if (record->event.pressed) {
                rgb_matrix_increase_sat();
            }
            return false;
        case RMDS:
            if (record->event.pressed) {
                rgb_matrix_decrease_sat();
            }
            return false;
        case RMIV:
            if (record->event.pressed) {
                rgb_matrix_increase_val();
            }
            return false;
        case RMDV:
            if (record->event.pressed) {
                rgb_matrix_decrease_val();
            }
            return true;
        case KC_BSPC: {
            // Initialize a boolean variable that keeps track
            // of the delete key status: registered or not?
            static bool delkey_registered;
            if (record->event.pressed) {
                // Detect the activation of either shift keys
                if (mod_state & MOD_MASK_SHIFT) {
                    // First temporarily canceling both shifts so that
                    // shift isn't applied to the KC_DEL keycode
                    del_mods(MOD_MASK_SHIFT);
                    register_code(KC_DEL);
                    // Update the boolean variable to reflect the status of KC_DEL
                    delkey_registered = true;
                    // Reapplying modifier state so that the held shift key(s)
                    // still work even after having tapped the Backspace/Delete key.
                    set_mods(mod_state);
                    return false;
                }
            } else { // on release of KC_BSPC
                // In case KC_DEL is still being sent even after the release of KC_BSPC
                if (delkey_registered) {
                    unregister_code(KC_DEL);
                    delkey_registered = false;
                    return false;
                }
            }
            // Let QMK process the KC_BSPC keycode as usual outside of shift
            return true;
        }
        case LT(_DVORAK, KC_SCLN): {
            if (record->tap.count && record->event.pressed) {
                return true; // Return true for normal processing of tap keycode
                break;
            } else if (record->event.pressed) {
                tap_code16(KC_COLN); // Intercept hold function to send COLON
                return false;
            }
            return true; // this allows for normal processing of key release!
        }
        case LT(_DVORAK, KC_N): {
            if (record->tap.count && record->event.pressed) {
                return true; // Return true for normal processing of tap keycode
                break;
            } else if (record->event.pressed) {
                // Intercept hold function to send ñ/Ñ
                register_code(KC_ALGR);
                tap_code16(KC_N);
                unregister_code(KC_ALGR);
                return false;
            }
            return true; // this allows for normal processing of key release!
        }

        default:
            return true;
    }
}

#define LOWER LT(_LOWER, KC_SPC)
#define RAISE LT(_RAISE, KC_ENT)
#define LSFTCAP SFT_T(KC_CAPS)
#define SEMI_CLN LT(_DVORAK, KC_SCLN)
#define CUSTOM_N LT(_DVORAK, KC_N)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* DVORAK
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * | Esc  |   '  |   ,  |   .  |   P  |   Y  |                 |   F  |   G  |   C  |   R  |   L  | Bksp |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * | Tab  |   A  |   O  |   U  |   I  |   D  |                 |   D  |   H  |   T  |   N  |   S  |   /  |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |Shift |   :  |   Q  |   J  |   K  |   X  +------+   +------+   B  |   M  |   W  |   V  |   Z  |   -  |
 * +------+------+------+------+------+------+ Lower|   |Raise +------+------+------+------+------+------+
 *                             | Ctrl |Super |      |   |      |  Alt |      |
 *                             +------+------+------+   +------+------+------+   
 */
  [_DVORAK] = LAYOUT_split_3x6_3(
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
     KC_ESC, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,    KC_Y,                         KC_F,    KC_G,    KC_C,    KC_R,    KC_L, KC_BSPC,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
     KC_TAB,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I,                         KC_D,    KC_H,    KC_T,CUSTOM_N,    KC_S, KC_SLSH,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    LSFTCAP,SEMI_CLN,    KC_Q,    KC_J,    KC_K,    KC_X,                         KC_B,    KC_M,    KC_W,    KC_V,    KC_Z, KC_MINS,
//+--------+--------+--------+--------+--------+--------+--------+  +--------+--------+--------+--------+--------+--------+--------+
                                        KC_LGUI,   MO(1),  KC_SPC,     KC_ENT,   MO(2), KC_RALT
                                    //+--------+--------+--------+  +--------+--------+--------+
  ),

/* Lower
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |      |      |      |      |      |      |                 |      |   7  |   8  |   9  |   =  | Bksp |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |      |      |      |      |      |      |                 |      |   4  |   5  |   6  |   *  |   /  |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |      |      |      |      |      |      +------+   +------+      |   1  |   2  |   3  |   +  |   -  |
 * +------+------+------+------+------+------+      |   |      +------+------+------+------+------+------+
 *                             |      |      |      |   |      |      |   0  |
 *                             +------+------+------+   +------+------+------+   
 */
  [_LOWER] = LAYOUT_split_3x6_3(
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    _______, _______, _______, _______, _______, _______,                      _______,    KC_7,    KC_8,    KC_9,  KC_EQL, _______,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    _______, _______, _______, _______, _______, _______,                      _______,    KC_4,    KC_5,    KC_6, KC_ASTR, _______,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    _______, _______, _______, _______, _______, _______,                      _______,    KC_1,    KC_2,    KC_3, KC_PLUS, _______,
//+--------+--------+--------+--------+--------+--------+--------+  +--------+--------+--------+--------+--------+--------+--------+
                                        _______, _______, _______,    _______, _______,    KC_0
                                    //+--------+--------+--------+  +--------+--------+--------+
  ),

/* Raise
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |      |  F1  |  F2  |  F3  |  F4  |      |                 |      | PgUp |  Up  | PgDn |      |      |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |      |  F5  |  F6  |  F7  |  F8  |      |                 | Home | Left | Down |Right | End  |Enter |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |      |  F9  | F10  | F11  | F12  |      +------+   +------+      |      |      |      |      |      |
 * +------+------+------+------+------+------+      |   |      +------+------+------+------+------+------+
 *                             |      |      |      |   |      |      |      |
 *                             +------+------+------+   +------+------+------+   
 */
  [_RAISE] = LAYOUT_split_3x6_3(
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4, _______,                      _______, KC_PGUP,   KC_UP, KC_PGDN, _______, _______,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    _______,   KC_F5,   KC_F6,   KC_F7,   KC_F8, _______,                      KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT,  KC_END,  KC_ENT,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    _______,   KC_F9,  KC_F10,  KC_F11,  KC_F12, _______,                      _______, _______, _______, _______, _______, _______,
//+--------+--------+--------+--------+--------+--------+--------+  +--------+--------+--------+--------+--------+--------+--------+
                                        _______, _______, _______,    _______, _______, _______
                                    //+--------+--------+--------+  +--------+--------+--------+
  ),

/* Adjust (Lower + Raise)
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |Reset |      | RGB  | HUE+ | SAT+ |BRGTH+|                 |      |      |      |      |      | PWR  |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |      |      |RGBMOD| HUE- | SAT- |BRGTH-|                 |      |      |      |      | PSCR |      |
 * +------+------+------+------+------+------+                 +------+------+------+------+------+------+
 * |Debug |      |RGBTOG|      |      |      +------+   +------+      |      |      |      |      |      |
 * +------+------+------+------+------+------+      |   |      +------+------+------+------+------+------+
 *                             |      |      |      |   |      |      |      |
 *                             +------+------+------+   +------+------+------+
 */
  [_ADJUST] = LAYOUT_split_3x6_3(
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
      RESET, _______,     RMT,    RMIH,    RMIS,    RMIV,                      _______, _______, _______, _______, _______,  KC_PWR,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
    _______, _______,     RMS,    RMDH,    RMDS,    RMDV,                      _______, _______, _______, _______, KC_PSCR, _______,
//+--------+--------+--------+--------+--------+--------+                    +--------+--------+--------+--------+--------+--------+
      DEBUG, _______, RGB_TOG, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
//+--------+--------+--------+--------+--------+--------+--------+  +--------+--------+--------+--------+--------+--------+--------+
                                        _______, _______, _______,    _______, _______, _______
                                    //+--------+--------+--------+  +--------+--------+--------+
  ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _RAISE:
            rgblight_setrgb(0x00, 0x00, 0xFF);
            break;
        case _LOWER:
            rgblight_setrgb(0xFF, 0x00, 0x00);
            break;
        case _DVORAK:
            rgblight_setrgb(0x00, 0xFF, 0x00);
            break;
        case _ADJUST:
            rgblight_setrgb(0x7A, 0x00, 0xFF);
            break;
        default: //  for any other layers, or the default layer
            rgblight_setrgb(0x00, 0xFF, 0xFF);
            break;
    }
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
