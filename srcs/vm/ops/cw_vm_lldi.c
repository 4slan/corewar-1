/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cw_vm_lldi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthierce <jthierce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 17:14:53 by amalsago          #+#    #+#             */
/*   Updated: 2020/06/13 21:10:07 by amalsago         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cw_vm_game.h"
#include "cw_inst.h"

int					cw_vm_op_lldi_dir(t_cw_game *game, t_cw_vm *vm, int pos)
{
	int				i;
	int 			pow;
	int				total;

	i = -1;
	total = 0;
	pow = ft_pow(256, CW_DIR_SIZE_LLDI - 1);
	while (++i < CW_DIR_SIZE_LLDI)
	{
		total += pow * vm->arena[(game->procs->pos + pos + i) % CW_MEM_SIZE];
		pow /= 256;
	}
	return (total);
}

int16_t				cw_vm_op_lldi_ind(t_cw_game *game, t_cw_vm *vm, int pos)
{
	int				i;
	int16_t			arg;
	int				total;
	unsigned int	mult;
	int				idx_address;

	i = -1;
	arg = 0;
	total = 0;
	mult = 256;
	while (++i < 2)
	{
		arg += mult * vm->arena[(game->procs->pos + pos + i) % CW_MEM_SIZE];
		mult /= 256;
	}
	idx_address = (game->procs->pos + (arg % CW_IDX_MOD)) % CW_MEM_SIZE;
	if (idx_address < 0)
		idx_address += CW_MEM_SIZE;
	i = -1;
	arg = 0;
	mult = ft_pow(256, 3);
	while (++i < 3)
	{
		total += mult * vm->arena[(idx_address + i) % CW_MEM_SIZE];
		mult /= 256;
	}
	return (arg);
}

int					cw_vm_op_lldi_value_reg(t_cw_vm *vm, int pos)
{
	int 			i;
	int 			pow;
	int 			total;

	i = -1;
	pow = ft_pow(256, 3);
	while (++i < 4)
	{
		total = vm->arena[(pos + i) % CW_MEM_SIZE];
		pow /= 256;
	}
	return (total);
}

void				cw_vm_op_lldi_body(t_cw_inst *inst, t_cw_game *game, t_cw_vm *vm)
{
	int				i;
	int				arg[3];
	int				pos;

	i = -1;
	pos = 2;
	while (++i < 2)
	{
		if (inst->types[i] == T_REG)
		{
			if (cw_vm_is_reg(vm->arena[game->procs->pos + pos]) == false)
				ft_printf("ERROR\n");
			arg[i] = game->procs->regs[(vm->arena[game->procs->pos + pos]) - 1];
			pos++;
		}
		else if (inst->types[i] == T_DIR)
		{
			arg[i] = cw_vm_op_lldi_dir(game, vm, pos);
			pos += CW_DIR_SIZE_LLDI;
		}
		else if (inst->types[i] == T_IND)
		{
			arg[i] = cw_vm_op_lldi_ind(game, vm, pos);
			pos += 2;
		}
	}
	arg[0] = game->procs->pos + (arg[0] + arg[1]);
	if (arg[0] < 0)
		arg[0] += CW_MEM_SIZE;
	game->procs->regs[vm->arena[game->procs->pos + pos] - 1] = cw_vm_op_lldi_value_reg(vm, arg[0]);
}

void	cw_vm_op_lldi(t_cw_inst *inst, t_cw_game *game, t_cw_vm *vm)
{
	if (inst->args_count == 3 && inst->types[1] != T_IND && inst->types[2] == T_REG)
		cw_vm_op_lldi_body(inst, game, vm);
	// opc + encoding byte + (T_REG | T_DIR | T_IND) + (T_REG | T_DIR) + T_REG
	// 5 T_REG T_REG
	// 6 T_REG T_DIR
	// 6 T_DIR T_REG
	// 7 T_DIR T_DIR
	// 6 T_IND T_REG
	// 7 T_IND T_DIR
	game->procs->pos += (1 + 1 + ((inst->types[0] == T_REG) ? 1 : CW_DIR_SIZE_LLDI)) % CW_MEM_SIZE;
	game->procs->pos += ((inst->types[1] == T_REG) ? 1 : CW_DIR_SIZE_LLDI) % CW_MEM_SIZE;
	game->procs->pos += ((inst->types[2] == T_REG) ? 1 : 0) % CW_MEM_SIZE;
}
