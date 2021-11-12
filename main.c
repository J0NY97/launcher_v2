#include "launcher.h"

void	play_events(t_launcher *launcher, SDL_Event e)
{
	
}

void	editor_events(t_launcher *launcher, SDL_Event e)
{

}

void	settings_events(t_launcher *launcher, SDL_Event e)
{

}

void	user_events(t_launcher *launcher, SDL_Event e)
{
	ui_list_radio_event(launcher->menu_buttons, &launcher->active_menu_button);
	launcher->play_menu->show = launcher->active_menu_button == launcher->play_button;
	launcher->editor_menu->show = launcher->active_menu_button == launcher->editor_button;
	launcher->settings_menu->show = launcher->active_menu_button == launcher->settings_button;

	if (launcher->play_menu->show)
		play_events(launcher, e);
	if (launcher->editor_menu->show)
		editor_events(launcher, e);
	if (launcher->settings_menu->show)
		settings_events(launcher, e);
}

void	launcher_init(t_launcher *launcher)
{
	launcher->win_main = ui_list_get_window_by_id(launcher->layout.windows, "win_main");

	// Main Menu
	launcher->main_menu = ui_list_get_element_by_id(launcher->layout.elements, "main_menu");

	// Play Menu
	launcher->play_menu = ui_list_get_element_by_id(launcher->layout.elements, "play_menu");

	// Editor Menu
	launcher->editor_menu = ui_list_get_element_by_id(launcher->layout.elements, "editor_menu");

	// Settings Menu
	launcher->settings_menu = ui_list_get_element_by_id(launcher->layout.elements, "settings_menu");

	// Buttons
	launcher->play_button = ui_list_get_element_by_id(launcher->layout.elements, "play_button");
	launcher->editor_button = ui_list_get_element_by_id(launcher->layout.elements, "editor_button");
	launcher->settings_button = ui_list_get_element_by_id(launcher->layout.elements, "settings_button");

	add_to_list(&launcher->menu_buttons, launcher->play_button, sizeof(t_ui_element));
	add_to_list(&launcher->menu_buttons, launcher->editor_button, sizeof(t_ui_element));
	add_to_list(&launcher->menu_buttons, launcher->settings_button, sizeof(t_ui_element));

//	ui_label_font_set(ui_button_get_label_element(launcher->play_button), "fonts/AmazDoom.ttf");

	launcher->quit_button = ui_list_get_element_by_id(launcher->layout.elements, "quit_button");
}

int	main(void)
{
	t_launcher	launcher;
	int			run;
	SDL_Event	e;

	ui_sdl_init();
	memset(&launcher, 0, sizeof(t_launcher));
	ui_layout_load(&launcher.layout, "launcher.ui");
	launcher_init(&launcher);
	run = 1;
	while (run)
	{
		if (launcher.win_main->wants_to_close
			|| launcher.quit_button->state == UI_STATE_CLICK)
			run = 0;
		while (SDL_PollEvent(&e))
		{
			ui_layout_event(&launcher.layout, e);
			user_events(&launcher, e);
			if (e.key.keysym.scancode == SDL_SCANCODE_P)
				ui_element_print(launcher.play_button);
		}
		// TODO: FIX: first element doesnt want to show anything if we dont fill the win texture with full color;
		ui_texture_fill(launcher.win_main->renderer, launcher.win_main->texture, 0xff00ff00);
		ui_layout_render(&launcher.layout);
	}
	return (0);
}
