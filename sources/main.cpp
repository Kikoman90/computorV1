/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:46:30 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/20 17:50:35 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computorV1.hpp"

# define A_COL "\033[31m"
# define B_COL "\033[32m"
# define C_COL "\033[33m"

typedef struct	s_polynomial {
	double		a;
	double		b;
	double		c;
	int			degree;
}				t_polynomial;

static void	usage(void)
{
	std::cout << "Usage: computorV1 [-...] \"equation\"" << std::endl;
	//show options
}

int		main(int ac, char **av) {

	int		i = 0;	
	bool	show_usage = true;

	while (i < ac)
	{
		
		i++;
	}
	if (show_usage)
		usage();
	return (0);
}
