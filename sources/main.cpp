/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:46:30 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/22 16:45:05 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/*
# define A_COL "\033[31m"
# define B_COL "\033[32m"
# define C_COL "\033[33m"
*/

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

	//Solver	solver();
	Parser	parser;
	int		i = 1;
	bool	show_usage = true;

	while (i < ac)
	{
		//std::cout << av[i] << std::endl;
		parser.run(av[i]);
		i++;
	}
	if (show_usage)
		usage();
	return (0);
}
