/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 18:58:04 by nneronin          #+#    #+#             */
/*   Updated: 2022/02/27 11:17:16 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	editor_menu_init(t_launcher *launcher)
{
	launcher->editor_menu
		= ui_layout_get_element(&launcher->layout, "editor_menu");
}

void	editor_events(t_launcher *launcher)
{
	t_ui_element	*clicked_map;

	clicked_map = ui_list_get_clicked_element(launcher->editor_map_buttons);
	if (clicked_map)
		start_editor(ui_button_get_text(clicked_map));
}

void	start_editor(char *map)
{
	char	**args;

	args = ft_memalloc(sizeof(char *) * 4);
	args[0] = ft_sprintf("%s%s", EDITOR_PATH, "doom_editor");
	args[1] = ft_sprintf("%s%s", MAP_PATH, map);
	args[2] = ft_sprintf("-launcher");
	args[4] = NULL;
	execv(args[0], args);
	ft_arraydel(args);
}
