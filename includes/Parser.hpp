/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:57:18 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/22 19:13:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

//# include <Solver.hpp>
# include <iostream>
# include <regex>
//# include <utility>
# include <vector>
# include <map>

class Parser {

public:
	//Parser(Solver &solver);
	Parser();
	~Parser();

	bool	run(char const *input);

private:
	std::regex													_equationRegex;
	std::regex													_optionRegex;
	std::map<unsigned int, double, std::greater<unsigned int> >	_coefficients;

	void	show_map() const;
	void	new_term(std::string const &cstr, std::string const &dstr, bool right_side);
	//Solver							&_solver;

};

#endif
