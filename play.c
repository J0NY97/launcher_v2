/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 18:58:41 by nneronin          #+#    #+#             */
/*   Updated: 2022/02/27 11:16:55 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	play_menu_init(t_launcher *launcher)
{
	launcher->play_menu
		= ui_layout_get_element(&launcher->layout, "play_menu");
	launcher->endless_menu
		= ui_layout_get_element(&launcher->layout, "endless_menu");
	launcher->story_menu
		= ui_layout_get_element(&launcher->layout, "story_menu");
	launcher->endless_button
		= ui_layout_get_element(&launcher->layout, "endless_button");
	launcher->story_button
		= ui_layout_get_element(&launcher->layout, "story_button");
	launcher->active_play_button = launcher->endless_button;
	launcher->difficulty_dropdown
		= ui_layout_get_element(&launcher->layout, "difficulty_dropdown");
	ui_dropdown_activate(launcher->difficulty_dropdown,
		ui_list_get_element_by_id(
			ui_dropdown_get_menu_element(launcher->difficulty_dropdown)
			->children, "normal_button"));
}

void	play_events(t_launcher *launcher)
{
	t_ui_element	*click_map;
	char			*diff_text;

	if (ui_dropdown_exit(launcher->difficulty_dropdown))
	{
		diff_text = ui_button_get_text(
				ui_dropdown_active(launcher->difficulty_dropdown));
		if (ft_strequ(diff_text, "Normal"))
			launcher->settings.difficulty = 2;
		else if (ft_strequ(diff_text, "Hard"))
			launcher->settings.difficulty = 3;
		else if (ft_strequ(diff_text, "Easy"))
			launcher->settings.difficulty = 1;
		else
			launcher->settings.difficulty = 2;
	}
	click_map = NULL;
	if (launcher->endless_menu->show)
		click_map = ui_list_get_clicked_element(
				launcher->endless_map_buttons);
	else if (launcher->story_menu->show)
		click_map = ui_list_get_clicked_element(
				launcher->story_map_buttons);
	if (click_map)
		start_game(launcher, launcher->settings, ui_button_get_text(click_map));
}

void	start_game(t_launcher *launcher, t_settings settings, char *map)
{
	char	**args;

	set_settings_to_file(LAUNCHER_PATH"settings.cfg", &settings);
	args = ft_memalloc(sizeof(char *) * 10);
	args[0] = ft_sprintf("%s", GAME_PATH"doom");
	args[1] = ft_sprintf("%s%s", MAP_PATH, map);
	args[2] = ft_sprintf("-size=%dx%d", settings.width, settings.height);
	args[3] = ft_sprintf("-res=%.2f",
			ceil((float)settings.texture_scale / 10) / 10);
	args[4] = ft_sprintf("-mouse=%.3fx%.3f",
			(float)settings.mouse_x / 1000,
			(float)settings.mouse_y / 1000);
	args[5] = ft_sprintf("-diff=%d", settings.difficulty);
	args[6] = ft_sprintf("-fov=%d", settings.fov);
	args[7] = ft_strdup("-launcher");
	if (settings.developer)
		args[8] = ft_strdup("-debug");
	else
		args[8] = NULL;
	args[9] = NULL;
	launcher_free(launcher);
	execv(args[0], args);
	ft_arraydel(args);
}
