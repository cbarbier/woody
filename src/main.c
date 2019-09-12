/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 13:49:42 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/27 17:03:45 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>




int main(int ac, char **av)
{
	t_wdy			obj;

	ft_bzero(&obj, sizeof(t_wdy));
	printf("%lx %lx %lx %lu\n", params[0], params[1], params[2], (void*)params - (void*)putwoody);
	if (parse_arg(&obj, ac, av))
	{
		dprintf(STDERR, "Usage: %s BINARY [-c 1|2|3, -s key]\n\t-1 : XOR42\n\t-2 : ROT13\n\t-3 : RC4\n\n\t- The key must be 3 char long and alphanumeric\n", av[0]);
		return (1);
	}
	if (load_file(av[1], &obj) < 0)
		return (1);
	dispatcher(&obj);
	release_file(&obj);
	return (0);
}
