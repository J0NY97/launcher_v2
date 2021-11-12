#include "launcher.h"

void	launcher_init(t_launcher *launcher)
{
	launcher->win_main = ui_list_get_window_by_id(launcher->layout.windows, "win_main");
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
		if (launcher.win_main->wants_to_close)
			run = 0;
		while (SDL_PollEvent(&e))
		{
			ui_layout_event(&launcher.layout, e);
		}
		ui_layout_render(&launcher.layout);
	}
	return (0);
}
