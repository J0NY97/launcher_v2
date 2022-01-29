/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unzip.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 13:51:03 by nneronin          #+#    #+#             */
/*   Updated: 2022/01/29 13:54:53 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	unzip_resources(void)
{
	if (access("./game/resources", F_OK))
	{
		if (!access("./resources.zip", F_OK))
			system("unzip -q resources.zip -d ./game");
		ft_printf("ZIP File not present.\n");
		exit (1);
	}
}
