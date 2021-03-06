/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const char *fonts[] = {
	"tewi-medium:size=10"
};
static const char dmenufont[]       = "-*-tewi-medium-*-*--*-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#000000";
static const char normbgcolor[]     = "#000000";
static const char normfgcolor[]     = "#CCCCCC";
static const char selbordercolor[]  = "#FFFFFF";
static const char selbgcolor[]      = "#000000";
static const char selfgcolor[]      = "#FFFFFF";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

#include "maximize.c"

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title       tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",        NULL,       NULL,       0,            1,           0,         0,        -1 },
	{ "Chromium",    NULL,       NULL,       1 << 8,       0,           0,         0,        -1 },
	{ "st-", 		 NULL,       NULL,       0,            0,           1,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,            {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0";

static const char *dmenucmd[] = { "menu", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, "-l 10", NULL };
static const char *termcmd[]  = { "st", "-e", "tmux", NULL };
static const char *raisevolumecmd[] = {"volume", "+"};
static const char *lowervolumecmd[] = {"volume", "-"};

static Key keys[] = {
	/* modifier                     key        function            argument */
	{ MODKEY,                       XK_p,      spawn,              {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,              {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,          {0} },
	{ MODKEY,                       XK_j,      focusstack,         {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,         {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,         {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,           {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,           {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,               {0} },
	{ MODKEY,                       XK_Tab,    view,               {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,         {0} },
	{ MODKEY,                       XK_t,      setlayout,          {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,          {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,          {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,          {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating,     {0} },
	{ MODKEY,                       XK_0,      view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,                {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,           {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,             {.i = +1 } },
	{ MODKEY|ControlMask,           XK_j, 	   moveresize,         {.v  =  "0x 80y 0w 0h"}},
	{ MODKEY|ControlMask,           XK_k,  	   moveresize,         {.v  =  "0x -80y 0w 0h"}},
	{ MODKEY|ControlMask,           XK_l,  	   moveresize,         {.v  =  "80x 0y 0w 0h"}},
	{ MODKEY|ControlMask,           XK_h,  	   moveresize,         {.v  =  "-80x 0y 0w 0h"}},
	{ MODKEY|ControlMask|ShiftMask, XK_j,  	   moveresize,         {.v  =  "0x 0y 0w 80h"}},
	{ MODKEY|ControlMask|ShiftMask, XK_k,  	   moveresize,         {.v  =  "0x 0y 0w -80h"}},
	{ MODKEY|ControlMask|ShiftMask, XK_l,  	   moveresize,         {.v  =  "0x 0y 80w 0h"}},
	{ MODKEY|ControlMask|ShiftMask, XK_h,  	   moveresize,         {.v  =  "0x 0y -80w 0h"}},
	{ MODKEY|ControlMask, 			XK_m,      toggleverticalmax,   NULL },
	{ MODKEY|ControlMask|ShiftMask,	XK_m,      togglehorizontalmax, NULL },
	{ 0, XF86XK_AudioRaiseVolume          , spawn,          {.v = raisevolumecmd } },
	{ 0, XF86XK_AudioLowerVolume          , spawn,          {.v = lowervolumecmd } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_0,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,               Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,               Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,               Button2,        zoom,           {0} },
	{ ClkStatusText,        0,               Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,          Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,          Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,			 Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,               Button1,        view,           {0} },
	{ ClkTagBar,            0,               Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,          Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,          Button3,        toggletag,      {0} },
};

