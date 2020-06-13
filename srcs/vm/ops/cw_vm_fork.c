/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cw_vm_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthierce <jthierce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 17:13:59 by amalsago          #+#    #+#             */
/*   Updated: 2020/06/13 21:09:04 by amalsago         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cw_vm_game.h"
#include "cw_inst.h"

void			cw_vm_op_fork(t_cw_inst *inst, t_cw_game *game, t_cw_vm *vm)
{
	int			i;
	int			idx_address;
	int			mult;
	int16_t		arg1;
	t_cw_proc	*new;

	if (inst->args_count == 1 && inst->types[0] == T_DIR)
	{
		i = -1;
		arg1 = 0;
		mult = ft_pow(256, CW_DIR_SIZE_FORK - 1);
		while (++i < CW_DIR_SIZE_FORK)
		{
			arg1 += mult * vm->arena[(game->procs->pos + 1 + i) % CW_MEM_SIZE];
			mult /= 256;
		}
		idx_address = game->procs->pos + (arg1 % CW_IDX_MOD);
		if (!(new = ft_memdup(game->procs, sizeof(t_cw_proc)))) //a verifier le tableau de registre
			exit(CW_ERROR_MALLOC_FAILED);
		new->pos = idx_address;
		new->next = game->head;
		game->head = new;
	}
	// opc + T_DIR
	// 3
	game->procs->pos += (1 + ((inst->types[0] == T_DIR) ? CW_DIR_SIZE_FORK : 0)) % CW_MEM_SIZE;
	/* ft_printf("0x%02x 0x%02x 0x%02x\n", vm->arena[game->procs->pos-1], vm->arena[game->procs->pos], vm->arena[game->procs->pos + 1]); */
}
