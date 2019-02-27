/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:46:30 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/27 20:24:23 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

static void	usage(void)
{
	std::cout << std::endl;
	if (CV1_COLORS)
		std::cout << CV1_YEL;
	std::cout << "Usage: ./computorV1 [-s] ((-f <filename>) | <equation>)" << std::endl;
	std::cout << "options:\n" << "-s : display intermediary steps\n" << "-f <filename> : read input from file\n";
	std::cout << CV1_DEF;
}

int		main(int ac, char **av) {

	int		i = 1;
	bool	show_usage = true;
	Parser	parser;

	if (ac < 2) {
		usage();
		return (0);
	}
	else {
		while (i < ac) {
			try {
				if (parser.run(av[i]))
					show_usage = false;
			}
			catch (std::exception &e) {
				std::cout << e.what() << std::endl;
				if (CV1_INTERRUPT)
					return (-1);
			}
			i++;
		}
	}
	if (parser.waiting_for_file())
		parser.log_error(ERR_MISSING_FILE, "option -f requires an argument", false);
	if (show_usage)
		usage();
	return (0);
}

// tester plusieurs arguments av... etc.