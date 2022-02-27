/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unzip.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 13:51:03 by nneronin          #+#    #+#             */
/*   Updated: 2022/02/27 09:34:09 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	unzip_resources(void)
{
	if (access(ROOT_PATH"game/resources", F_OK))
	{
		if (!access(ROOT_PATH"resources.zip", F_OK))
			system("unzip -q resources.zip -d "ROOT_PATH"game");
		else
			ft_printf("ZIP File not present.\n");
		exit (1);
	}
}
