/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cw_vm_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthierce <jthierce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 18:04:40 by jthierce          #+#    #+#             */
/*   Updated: 2020/06/18 16:36:52 by amalsago         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cw_vm.h"

static void		cw_vm_header(void)
{
	int			fd;
	char		*line;

	if ((fd = open("header.txt", O_RDONLY)) != -1)
		while (ft_readtonl(fd, &line, 116) > 0)
		{
			ft_printf("{bold}{orangered}%s{}", line);
			ft_strdel(&line);
		}
	close(fd);
}

static void		cw_vm_init_struct_vm(t_cw_vm *vm)
{
	ft_bzero(vm, sizeof(vm));
	vm->dump = -1;
	ft_bzero(vm->arena, sizeof(unsigned char) * (CW_MEM_SIZE + 1));
	ft_bzero(vm->players, sizeof(t_cw_player) * CW_MAX_PLAYERS);
	ft_memset(vm->data.assigned_nbr, -1, sizeof(int) * CW_MAX_PLAYERS);
}

static void		free_memory(t_cw_vm *vm)
{
	int			i;

	i = -1;
	while (++i < vm->data.nbr_players)
	{
		ft_strdel(&vm->data.filename[i]);
		if (vm->players[i].champion != NULL)
			cw_champion_destroy(&vm->players[i].champion);
	}
}

int				main(int argc, char **argv)
{
	t_cw_vm		vm;
	int			ret;

	cw_vm_header();
	if (argc < 2)
		return (cw_vm_usage());
	cw_vm_init_struct_vm(&vm);
	if ((ret = cw_vm_parsing(argc - 1, argv + 1, &vm)) == CW_SUCCESS)
		if ((ret = cw_vm_read_player(&vm)) == CW_SUCCESS)
			ret = cw_vm_game(&vm);
	free_memory(&vm);
	return (ret);
}
