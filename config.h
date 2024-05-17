/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const int startwithgaps = 1;     /* 1 means gaps are used by default */
static const unsigned int gappx = 0; /* default gap between windows in pixels */
static const unsigned int snap = 32; /* snap pixel */
static const int showbar = 1;        /* 0 means no bar */
static const int topbar = 1;         /* 0 means bottom bar */
static const int horizpadbar = 0;    /* horizontal padding for statusbar */
static const int vertpadbar = 2;     /* vertical padding for statusbar */
static const int vertpad = 0;        /* vertical padding of bar */
static const int sidepad = 0;        /* horizontal padding of bar */
static const char *fonts[] = {"Monocraft Nerd Font:size=10"};
static const char dmenufont[] = "Monocraft Nerd Font:bold:size=10";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[] = "#005577";
static const char col_green[] = "#03A062";
static const char col_red[] = "#e46876";
static const char col_black[] = "#000000";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray4, col_black, col_gray1},
    [SchemeSel] = {col_red, col_black, col_red},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;   /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */
static const int attachbelow =
    1; /* 1 means attach after the currently active window */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

// #define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split
// vertically */ #include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  &((Keychord){1, {{MODKEY, KEY}}, view, {.ui = 1 << TAG}}),                   \
      &((Keychord){                                                            \
          1, {{MODKEY | ControlMask, KEY}}, toggleview, {.ui = 1 << TAG}}),    \
      &((Keychord){1, {{MODKEY | ShiftMask, KEY}}, tag, {.ui = 1 << TAG}}),    \
      &((Keychord){1,                                                          \
                   {{MODKEY | ControlMask | ShiftMask, KEY}},                  \
                   toggletag,                                                  \
                   {.ui = 1 << TAG}}),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",      dmenumon, "-fn",   dmenufont, "-nb",     col_gray1,
    "-nf",       col_gray3, "-sb",    col_red, "-sf",     col_gray4, NULL};

// Spawn terminal
static const char *termcmd[] = {"alacritty", NULL};

/* Launch Mercury browser */
static const char *mercurycmd[] = {"firefox", NULL};

/* Volume control */

// Volume up
static const char *volup[] = {"amixer", "set", "Master", "5%+", NULL};
// Volume down
static const char *voldown[] = {"amixer", "set", "Master", "5%-", NULL};
// Mute
static const char *volmute[] = {"amixer", "set", "Master", "toggle", NULL};

/* Brightness control */

// Brightness up
static const char *brightdown[] = {"brightnessctl", "set", "5%-", NULL};
// Brightness down
static const char *brightup[] = {"brightnessctl", "set", "+5%", NULL};

/* Screenshot */

// Full desktop screenshot
static const char *fulldeskss[] = {"flameshot", "full", NULL};
// Active monitor screenshot
static const char *activemss[] = {"flameshot", "screen", NULL};
// Region screenshot
static const char *regionss[] = {"flameshot", "gui", NULL};

/* Info controls */

// Show Ram
static const char *raminfo[] = {"sys-not-ram.sh", NULL};
// Show Date
static const char *dateinfo[] = {"sys-not-cal.sh", NULL};
// Show Uptime
static const char *upinfo[] = {"sys-not-up.sh", NULL};

/* Keychord notifications */

// Super + a
static const char *superanoti[] = {"supera.sh", NULL};
static const char *supersnoti[] = {"supers.sh", NULL};

/* Include media function keys */
#include <X11/XF86keysym.h>

#include "movestack.c"

/* Keybindings */
static Keychord *keychords[] = {
    /* Keys        function        argument */

    /* Spawn terminal */
    &((Keychord){1, {{MODKEY | ShiftMask, XK_Return}}, spawn, {.v = termcmd}}),
    &((Keychord){2, {{MODKEY, XK_e}, {MODKEY, XK_e}}, spawn, {.v = termcmd}}),

    &((Keychord){1, {{MODKEY, XK_p}}, spawn, {.v = dmenucmd}}),

    /* Toggle status bar */
    &((Keychord){1, {{MODKEY, XK_b}}, togglebar, {0}}),

    /* Window focus */
    &((Keychord){1, {{MODKEY, XK_j}}, focusstack, {.i = +1}}),
    &((Keychord){1, {{MODKEY, XK_k}}, focusstack, {.i = -1}}),

    /* Increase/Decrease the number of master windows */
    &((Keychord){1, {{MODKEY, XK_i}}, incnmaster, {.i = +1}}),
    &((Keychord){1, {{MODKEY, XK_d}}, incnmaster, {.i = -1}}),

    /* Increase/Decrease the master window size */
    &((Keychord){1, {{MODKEY, XK_h}}, setmfact, {.f = -0.05}}),
    &((Keychord){1, {{MODKEY, XK_l}}, setmfact, {.f = +0.05}}),

    /* Change master window */
    &((Keychord){1, {{MODKEY, XK_Return}}, zoom, {0}}),

    /* Switch to previous tag */
    &((Keychord){1, {{MODKEY, XK_Tab}}, view, {0}}),

    /* Move current window up */
    &((Keychord){1, {{MODKEY | ControlMask, XK_j}}, movestack, {.i = +1}}),

    /* Move current window down */
    &((Keychord){1, {{MODKEY | ControlMask, XK_k}}, movestack, {.i = -1}}),

    /* Kill current window */
    &((Keychord){1, {{MODKEY | ShiftMask, XK_x}}, killclient, {0}}),

    /* Change layout */

    /* Tiled view */
    &((Keychord){1, {{MODKEY, XK_t}}, setlayout, {.v = &layouts[0]}}),
    /* Floating view */
    &((Keychord){1, {{MODKEY, XK_f}}, setlayout, {.v = &layouts[1]}}),
    /* Fullscreen view */
    &((Keychord){1, {{MODKEY, XK_m}}, setlayout, {.v = &layouts[2]}}),

    &((Keychord){1, {{MODKEY, XK_space}}, setlayout, {0}}),
    &((Keychord){1, {{MODKEY | ShiftMask, XK_space}}, togglefloating, {0}}),

    /* Tags control */
    &((Keychord){1, {{MODKEY, XK_0}}, view, {.ui = ~0}}),
    &((Keychord){1, {{MODKEY | ShiftMask, XK_0}}, tag, {.ui = ~0}}),

    //
    &((Keychord){1, {{MODKEY | ShiftMask, XK_j}}, focusmon, {.i = -1}}),
    &((Keychord){1, {{MODKEY | ShiftMask, XK_k}}, focusmon, {.i = +1}}),
    &((Keychord){1, {{MODKEY | ShiftMask, XK_comma}}, tagmon, {.i = -1}}),
    &((Keychord){1, {{MODKEY | ShiftMask, XK_period}}, tagmon, {.i = +1}}),

    /* Fullgaps */
    // Decrease gaps
    &((Keychord){1, {{MODKEY | ControlMask, XK_minus}}, setgaps, {.i = -1}}),
    // Increase gaps
    &((Keychord){1, {{MODKEY | ControlMask, XK_equal}}, setgaps, {.i = +1}}),
    // Toggle gaps
    &((Keychord){1,
                 {{MODKEY | ControlMask | ShiftMask, XK_equal}},
                 setgaps,
                 {.i = GAP_TOGGLE}}),
    // Set gaps to the default size
    &((Keychord){1,
                 {{MODKEY | ControlMask | ShiftMask, XK_minus}},
                 setgaps,
                 {.i = GAP_RESET}}),

    /* Volume controls */
    // Volume up (without media keys)
    &((Keychord){1, {{MODKEY, XK_equal}}, spawn, {.v = volup}}),
    // Volume down (without media keys)
    &((Keychord){1, {{MODKEY, XK_minus}}, spawn, {.v = voldown}}),
    // Volume up (media keys)
    &((Keychord){1, {{0, XF86XK_AudioRaiseVolume}}, spawn, {.v = volup}}),
    // Volume down (media keys)
    &((Keychord){1, {{0, XF86XK_AudioLowerVolume}}, spawn, {.v = voldown}}),
    // Mute (media keys)
    &((Keychord){1, {{0, XF86XK_AudioMute}}, spawn, {.v = volmute}}),

    /* Brightness controls */
    // Brightness up (media keys)
    &((Keychord){1, {{0, XF86XK_MonBrightnessUp}}, spawn, {.v = brightup}}),
    // Brightness down (media keys)
    &((Keychord){1, {{0, XF86XK_MonBrightnessDown}}, spawn, {.v = brightdown}}),

    /* Quit DWM */
    &((Keychord){1, {{MODKEY | ShiftMask, XK_q}}, quit, {0}}),

    /* Restart DWM */
    &((Keychord){1, {{MODKEY | ControlMask | ShiftMask, XK_q}}, quit, {1}}),

    /* Tags */
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
            TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
                TAGKEYS(XK_9, 8)

        /* Custom Keychords */

        /* Apps -> super + a */
        &
        ((Keychord){2, {{MODKEY, XK_a}, {0, XK_h}}, spawn, {.v = superanoti}}),

    // "b" Mercury browser
    &((Keychord){2, {{MODKEY, XK_a}, {0, XK_b}}, spawn, {.v = mercurycmd}}),

    /* Screen -> super + s */
    &((Keychord){2, {{MODKEY, XK_s}, {0, XK_h}}, spawn, {.v = supersnoti}}),

    // "f" Full desktop screenshot
    &((Keychord){2, {{MODKEY, XK_s}, {0, XK_f}}, spawn, {.v = fulldeskss}}),

    // "m" Active monitor screenshot
    &((Keychord){2, {{MODKEY, XK_s}, {0, XK_m}}, spawn, {.v = activemss}}),

    // "r" Selected region screenshot
    &((Keychord){2, {{MODKEY, XK_s}, {0, XK_r}}, spawn, {.v = regionss}}),

    /* Show System Info */
    &((Keychord){2, {{MODKEY, XK_u}, {0, XK_d}}, spawn, {.v = dateinfo}}),
    &((Keychord){2, {{MODKEY, XK_u}, {0, XK_r}}, spawn, {.v = raminfo}}),
    &((Keychord){2, {{MODKEY, XK_u}, {0, XK_u}}, spawn, {.v = upinfo}}),

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
