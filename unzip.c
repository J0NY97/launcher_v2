/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unzip.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 13:51:03 by nneronin          #+#    #+#             */
/*   Updated: 2022/01/29 14:09:18 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	unzip_resources(void)
{
	if (access(ROOT_PATH"game/resources", F_OK))
	{
		if (!access(ROOT_PATH"resources.zip", F_OK))
			system("unzip -q resources.zip -d "ROOT_PATH"game");
		ft_printf("ZIP File not present.\n");
		exit (1);
	}
}
