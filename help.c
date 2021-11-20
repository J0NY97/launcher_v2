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

void	get_files_from_dir_with_file_ending(
		t_list **dest_list, char *directory, char *ending)
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
