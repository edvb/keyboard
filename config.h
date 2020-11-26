#define ROWS 5
#define COLS 15
/* TODO remove need for */
#define LAYERS 2

#define ROW_PINS { 16, 17, 18, 19, 20 }
#define COL_PINS { 15, 14, 13, 12, 11, 10, 9, 5, 4, 3, 2, 1, 21, 0 }

#define CLOCK 23
#define DATA  22
#define RESET 0

#define LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c,   k0d, \
     k10,   k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, \
      k20,   k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b,    k2c,  \
       k30,   k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a,      k3b,  \
    k40,  k41,  k42,  k43,   k44, k45, k46, k47,   k48,   k49,  k4a,  k4b \
) \
{ \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08,   k09,  k0a, k0b,   k0c,   k0d }, \
    { k10, k11, k12, k13, k14, k15, k16, k17, k18,   k19,  k1a, k1b,   k1c,   k1d }, \
    { k20, k21, k22, k23, k24, k25, k26, k27, k28,   k29,  k2a, k2b,   0,     k2c }, \
    { k30, k31, k32, k33, k34, k35, k36, k37, k38,   k39,  k3a, 0,     0,     k3b }, \
    { k41, k42, k43, k40, k44, k45, k46, k47, 0,     k48,  k49, k4a,   0,     k4b }  \
}

int layout[][ROWS][COLS] = {
  [0] = LAYOUT(
           KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE,
           KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_BACKSLASH,
           KEY_ESC, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_QUOTE, KEY_RETURN,
           KEY_LEFT_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_LEFT_SHIFT,
           KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_SPACE, 'L', 'M', 'R', KEY_SPACE, KEY_LEFT_ALT, KEY_ESC, KEY_LEFT_GUI, KEY_LEFT_CTRL),
};
