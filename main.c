#include "launcher.h"

void	play_events(t_launcher *launcher, SDL_Event e)
{
	if (launcher->endless_button->state == UI_STATE_CLICK)
		launcher->active_play_button = launcher->endless_button;
	if (launcher->story_button->state == UI_STATE_CLICK)
		launcher->active_play_button = launcher->story_button;
	launcher->endless_menu->show = launcher->active_play_button == launcher->endless_button;
	launcher->story_menu->show = launcher->active_play_button == launcher->story_button;
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

void	get_files_from_dir_with_file_ending(t_list **dest_list, char *directory, char *ending)
{
	DIR				*dirp;
	struct dirent	*dp;

	dirp = opendir(directory);
	if (!dirp)
	{
		ft_printf("[%s] Couldn\'t open directory <%s>.\n",
			__FUNCTION__, directory);
		return ;
	}
	while (1)
	{
		dp = readdir(dirp);
		if (!dp)
			break ;
		if (!ft_strendswith(dp->d_name, ending))
		{
			ft_printf("Git got %s %s\n", dp->d_name, ending);
			add_to_list(dest_list, ft_strdup(dp->d_name), sizeof(char *));
		}
	}
	closedir(dirp);
}

void	init_map_buttons_from_list(t_list *map_names, t_ui_recipe *recipe, t_ui_element *parent)
{
	t_ui_element	*elem;
	t_vec2			pos;
	int				amount_x;
	int				i;
	t_list			*curr;

	i = ft_lstlen(parent->children) - 1;
	amount_x = parent->pos.w / (recipe->pos.w + 10 + recipe->pos.x);
	curr = map_names;
	while (curr)
	{
		++i;
		pos = vec2(recipe->pos.x + (i % (amount_x + 1) * (recipe->pos.w + 10)),
				recipe->pos.y + (i / (amount_x + 1) * (recipe->pos.h + 10)));
		elem = ft_memalloc(sizeof(t_ui_element));
		ui_button_new(parent->win, elem);
		ui_element_set_parent(elem, parent, UI_TYPE_ELEMENT);
		ui_element_edit(elem, recipe);
		ui_element_pos_set2(elem, pos);
		ui_label_set_text(ui_button_get_label_element(elem), curr->content);
		curr = curr->next;
	}
}

void	launcher_init(t_launcher *launcher)
{
	launcher->win_main = ui_list_get_window_by_id(launcher->layout.windows, "win_main");

	// Main Menu
	launcher->main_menu = ui_list_get_element_by_id(launcher->layout.elements, "main_menu");

	// Play Menu
	launcher->play_menu = ui_list_get_element_by_id(launcher->layout.elements, "play_menu");
	launcher->endless_menu = ui_list_get_element_by_id(launcher->layout.elements, "endless_menu");
	launcher->story_menu = ui_list_get_element_by_id(launcher->layout.elements, "story_menu");
	launcher->endless_button = ui_list_get_element_by_id(launcher->layout.elements, "endless_button");
	launcher->story_button = ui_list_get_element_by_id(launcher->layout.elements, "story_button");
	launcher->active_play_button = launcher->endless_button;
	launcher->difficulty_dropdown = ui_list_get_element_by_id(launcher->layout.elements, "difficulty_dropdown");
	ui_dropdown_activate(launcher->difficulty_dropdown, ui_list_get_element_by_id(ui_dropdown_get_menu_element(launcher->difficulty_dropdown)->children, "normal_button"));

	// Editor Menu
	launcher->editor_menu = ui_list_get_element_by_id(launcher->layout.elements, "editor_menu");

	// Settings Menu
	launcher->settings_menu = ui_list_get_element_by_id(launcher->layout.elements, "settings_menu");
	launcher->resolution_dropdown = ui_list_get_element_by_id(launcher->layout.elements, "resolution_dropdown");
	ui_dropdown_activate(launcher->resolution_dropdown, ui_list_get_element_by_id(ui_dropdown_get_menu_element(launcher->resolution_dropdown)->children, "1920x1080_button"));

	// Buttons
	launcher->play_button = ui_list_get_element_by_id(launcher->layout.elements, "play_button");
	launcher->editor_button = ui_list_get_element_by_id(launcher->layout.elements, "editor_button");
	launcher->settings_button = ui_list_get_element_by_id(launcher->layout.elements, "settings_button");

	// Temp (REMOVE)
	launcher->active_menu_button = launcher->settings_button;

	add_to_list(&launcher->menu_buttons, launcher->play_button, sizeof(t_ui_element));
	add_to_list(&launcher->menu_buttons, launcher->editor_button, sizeof(t_ui_element));
	add_to_list(&launcher->menu_buttons, launcher->settings_button, sizeof(t_ui_element));

	launcher->quit_button = ui_list_get_element_by_id(launcher->layout.elements, "quit_button");

	get_files_from_dir_with_file_ending(&launcher->endless_maps, MAP_PATH, ".dnde");
	get_files_from_dir_with_file_ending(&launcher->story_maps, MAP_PATH, ".dnds");

	t_ui_recipe	*map_button_recipe;
	map_button_recipe = ui_list_get_recipe_by_id(launcher->layout.recipes, "map_button_prefab");

	init_map_buttons_from_list(launcher->endless_maps, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "endless_map_menu"));
	init_map_buttons_from_list(launcher->story_maps, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "story_map_menu"));

	init_map_buttons_from_list(launcher->endless_maps, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "editor_map_menu"));
	init_map_buttons_from_list(launcher->story_maps, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "editor_map_menu"));
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
