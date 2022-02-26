/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 08:44:26 by jsalmi            #+#    #+#             */
/*   Updated: 2022/02/26 13:00:14 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	set_setting(char **farr, t_settings *settings)
{
	if (!farr[0] || !farr[1])
		return ;
	if (ft_strequ(farr[0], "fov"))
		settings->fov = ft_atoi(farr[1]);
	else if (ft_strequ(farr[0], "mouse_x"))
		settings->mouse_x = ft_atoi(farr[1]);
	else if (ft_strequ(farr[0], "mouse_y"))
		settings->mouse_y = ft_atoi(farr[1]);
	else if (ft_strequ(farr[0], "texture_scale"))
		settings->texture_scale = ft_atoi(farr[1]);
	else if (ft_strequ(farr[0], "developer"))
		settings->developer = ft_atoi(farr[1]);
	else if (ft_strequ(farr[0], "width"))
		settings->width = ft_atoi(farr[1]);
	else if (ft_strequ(farr[0], "height"))
		settings->height = ft_atoi(farr[1]);
	else if (ft_strequ(farr[0], "difficulty"))
		settings->difficulty = ft_atoi(farr[1]);
}

void	get_settings_from_file(char *file, t_settings *settings)
{
	char	*content;
	char	**arr;
	char	**farr;
	int		i;

	content = get_file_content(file);
	if (!content)
		return ;
	arr = ft_strsplit(content, '\n');
	i = -1;
	while (arr[++i])
	{
		farr = ft_strsplit(arr[i], ' ');
		set_setting(farr, settings);
	}
}

void	set_settings_to_file(char *file, t_settings *settings)
{
	char	*str;	
	int		fd;

	fd = open(file, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0)
		return ;
	str = ft_sprintf(
			"fov %d\n"
			"mouse_x %d\n"
			"mouse_y %d\n"
			"texture_scale %d\n"
			"developer %d\n"
			"width %d\n"
			"height %d\n"
			"difficulty %d\n",
			settings->fov,
			settings->mouse_x,
			settings->mouse_y,
			settings->texture_scale,
			settings->developer,
			settings->width,
			settings->height,
			settings->difficulty);
	ft_putstr_fd(str, fd);
	close(fd);
}
