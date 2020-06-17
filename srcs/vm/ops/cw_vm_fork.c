/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cw_vm_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthierce <jthierce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 17:13:59 by amalsago          #+#    #+#             */
/*   Updated: 2020/06/17 01:45:35 by jthierce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cw_vm.h"
#include "cw_inst.h"

/*
** This operation creates an identical copy of the process and places it
** to the position (Argument1 % IDX_MOD) + position of parent cursor
*/

void			cw_vm_op_fork(t_cw_vm *vm)
{
	int			idx_address;
	int			new_pos;
	int			arg_pos;
	int16_t		arg_val;
	t_cw_proc	*new;

	arg_pos = (vm->game.procs->pos + 1) % CW_MEM_SIZE;
	arg_val = cw_vm_bigendian16_read(vm->arena, arg_pos);
	idx_address = (vm->game.procs->pos + (arg_val % CW_IDX_MOD)) % CW_MEM_SIZE;
	if (idx_address < 0)
		idx_address += CW_MEM_SIZE;
	if ((new = ft_memdup(vm->game.procs, sizeof(t_cw_proc))) != NULL)
	{
		new->pos = idx_address;
		new->next = vm->game.head;
		vm->game.head = new;
		new_pos = (vm->game.procs->pos + 1 + CW_DIR_SIZE_FORK) % CW_MEM_SIZE;
		vm->game.procs->pos = new_pos;
	}
	else
		ft_printf("{red}New processus can't be created" \
				"for the player %d\n{}", vm->game.procs->id);
}
