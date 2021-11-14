#include "launcher.h"

t_ui_element	*ui_list_get_clicked_element(t_list *list)
{
	t_ui_element	*button;

	while (list)
	{
		button = list->content;
		if (button->state == UI_STATE_CLICK)
			return (button);
		list = list->next;
	}
	return (NULL);
}

void	start_game(t_settings settings, char *map)
{
	ft_printf("we want to start the game with map <%s>\n", map);	
}

void	play_events(t_launcher *launcher, SDL_Event e)
{
	if (launcher->endless_button->state == UI_STATE_CLICK)
		launcher->active_play_button = launcher->endless_button;
	if (launcher->story_button->state == UI_STATE_CLICK)
		launcher->active_play_button = launcher->story_button;
	launcher->endless_menu->show = launcher->active_play_button == launcher->endless_button;
	launcher->story_menu->show = launcher->active_play_button == launcher->story_button;

	t_ui_element	*clicked_map;

	if (launcher->endless_menu->show)
		clicked_map = ui_list_get_clicked_element(launcher->endless_map_buttons);
	else if (launcher->story_menu->show)
		clicked_map = ui_list_get_clicked_element(launcher->story_map_buttons);
	else
		clicked_map = NULL;
	if (clicked_map)
		start_game(launcher->settings, ui_button_get_text(clicked_map));
}

void	start_editor(t_settings settings, char *map)
{
	ft_printf("we want to start the editor with map <%s>\n", map);	
}

void	editor_events(t_launcher *launcher, SDL_Event e)
{
	t_ui_element	*clicked_map;

	clicked_map = ui_list_get_clicked_element(launcher->editor_map_buttons);
	if (clicked_map)
		start_editor(launcher->settings, ui_button_get_text(clicked_map));
}

void	settings_events(t_launcher *launcher, SDL_Event e)
{
	if (ui_slider_updated(launcher->fov_slider))
		launcher->settings.fov = ui_slider_value_get(launcher->fov_slider);
	if (ui_slider_updated(launcher->mouse_x_slider))
		launcher->settings.mouse_x = ui_slider_value_get(launcher->mouse_x_slider);
	if (ui_slider_updated(launcher->mouse_y_slider))
		launcher->settings.mouse_y = ui_slider_value_get(launcher->mouse_y_slider);
	if (ui_slider_updated(launcher->texture_scale_slider))
		launcher->settings.texture_scale = ui_slider_value_get(launcher->texture_scale_slider);
	launcher->settings.developer = launcher->developer_checkbox->is_toggle;
	if (ui_dropdown_exit(launcher->resolution_dropdown))
	{
		char	**res;

		res = ft_strsplit(ui_button_get_text(ui_dropdown_active(launcher->resolution_dropdown)), 'x');
		launcher->settings.width = ft_atoi(res[0]);
		launcher->settings.height = ft_atoi(res[1]);
		ft_arraydel(res);
	}
}

void	settings_init(t_settings *settings)
{
	settings->fov = 75;
	settings->mouse_x = 5;
	settings->mouse_y = 5;
	settings->texture_scale = 80;
	settings->developer = 0;
	settings->width = 1920;
	settings->height = 1080;
}

void	settings_elem_default(t_launcher *launcher)
{
	ui_slider_value_set(launcher->fov_slider, launcher->settings.fov);
	ui_slider_value_set(launcher->mouse_x_slider, launcher->settings.mouse_x);
	ui_slider_value_set(launcher->mouse_y_slider, launcher->settings.mouse_y);
	ui_slider_value_set(launcher->texture_scale_slider, launcher->settings.texture_scale);
	launcher->developer_checkbox->is_toggle = launcher->settings.developer;
	char	*res;
	res = ft_sprintf("%dx%d", launcher->settings.width, launcher->settings.height);
	ui_dropdown_activate(launcher->resolution_dropdown, ui_list_get_button_with_text(ui_dropdown_get_menu_element(launcher->resolution_dropdown)->children, res));
	ft_strdel(&res);
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
			add_to_list(dest_list, ft_strdup(dp->d_name), sizeof(char *));
	}
	closedir(dirp);
}

void	init_map_buttons_from_list(t_list *map_names, t_ui_recipe *recipe, t_ui_element *parent)
{
	t_ui_element	*elem;
	float			amount_x;
	int				i;
	int				button_gap;
	t_list			*curr;

	i = ft_lstlen(parent->children) - 1;
	button_gap = 10;
	amount_x = parent->pos.w / (recipe->pos.w + button_gap + recipe->pos.x);
	while ((int)amount_x * (recipe->pos.w + button_gap) < parent->pos.w + button_gap)
		button_gap++;
	curr = map_names;
	while (curr)
	{
		++i;
		elem = ft_memalloc(sizeof(t_ui_element));
		ui_button_new(parent->win, elem);
		ui_element_set_parent(elem, parent, UI_TYPE_ELEMENT);
		ui_element_edit(elem, recipe);
		ui_element_pos_set2(elem, vec2(recipe->pos.x + (i % (int)(amount_x) * (recipe->pos.w + button_gap)),
				recipe->pos.y + (i / (int)(amount_x) * (recipe->pos.h + button_gap))));
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
	launcher->fov_slider = ui_list_get_element_by_id(launcher->layout.elements, "fov_slider");
	launcher->mouse_x_slider = ui_list_get_element_by_id(launcher->layout.elements, "mouse_x_slider");
	launcher->mouse_y_slider = ui_list_get_element_by_id(launcher->layout.elements, "mouse_y_slider");
	launcher->texture_scale_slider = ui_list_get_element_by_id(launcher->layout.elements, "texture_scale_slider");
	launcher->developer_checkbox = ui_list_get_element_by_id(launcher->layout.elements, "developer_checkbox");
	launcher->resolution_dropdown = ui_list_get_element_by_id(launcher->layout.elements, "resolution_dropdown");
	ui_dropdown_activate(launcher->resolution_dropdown, ui_list_get_element_by_id(ui_dropdown_get_menu_element(launcher->resolution_dropdown)->children, "1920x1080_button"));

	// Buttons
	launcher->play_button = ui_list_get_element_by_id(launcher->layout.elements, "play_button");
	launcher->editor_button = ui_list_get_element_by_id(launcher->layout.elements, "editor_button");
	launcher->settings_button = ui_list_get_element_by_id(launcher->layout.elements, "settings_button");

	// Temp (REMOVE)
	//launcher->active_menu_button = launcher->settings_button;

	add_to_list(&launcher->menu_buttons, launcher->play_button, sizeof(t_ui_element));
	add_to_list(&launcher->menu_buttons, launcher->editor_button, sizeof(t_ui_element));
	add_to_list(&launcher->menu_buttons, launcher->settings_button, sizeof(t_ui_element));

	launcher->quit_button = ui_list_get_element_by_id(launcher->layout.elements, "quit_button");

	get_files_from_dir_with_file_ending(&launcher->endless_map_names, MAP_PATH, ".dnde");
	get_files_from_dir_with_file_ending(&launcher->story_map_names, MAP_PATH, ".dnds");

	t_ui_recipe	*map_button_recipe;
	map_button_recipe = ui_list_get_recipe_by_id(launcher->layout.recipes, "map_button_prefab");

	init_map_buttons_from_list(launcher->endless_map_names, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "endless_map_menu"));
	launcher->endless_map_buttons = ui_list_get_element_by_id(launcher->layout.elements, "endless_map_menu")->children;
	init_map_buttons_from_list(launcher->story_map_names, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "story_map_menu"));
	launcher->story_map_buttons = ui_list_get_element_by_id(launcher->layout.elements, "story_map_menu")->children;

	init_map_buttons_from_list(launcher->endless_map_names, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "editor_map_menu"));
	init_map_buttons_from_list(launcher->story_map_names, map_button_recipe, ui_list_get_element_by_id(launcher->layout.elements, "editor_map_menu"));
	launcher->editor_map_buttons = ui_list_get_element_by_id(launcher->layout.elements, "editor_map_menu")->children;

	settings_init(&launcher->settings);
	settings_elem_default(launcher);
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
