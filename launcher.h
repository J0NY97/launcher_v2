#ifndef LAUNCHER_H
# define LAUNCHER_H

#include "libft.h"
#include "libpf.h"
#include "libui.h"

typedef struct s_launcher
{
	t_ui_layout		layout;

	t_ui_window		*win_main;

	t_ui_element	*main_menu;

	t_ui_element	*play_button;
	t_ui_element	*editor_button;
	t_ui_element	*settings_button;
	t_ui_element	*quit_button;
	
	t_list			*menu_buttons; // t_ui_element *
	t_ui_element	*active_menu_button;

	t_ui_element	*play_menu;
	t_ui_element	*editor_menu;
	t_ui_element	*settings_menu;
}					t_launcher;

#endif
