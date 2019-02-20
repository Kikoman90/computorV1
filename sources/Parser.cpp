/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:58:31 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/20 20:17:24 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser() {}

bool	Parser::run(char const *input) {
	
	static std::regex const	equationRegex = "\\s*(?:\\b...=";
	static std::regex const	optionRegex = "\\s*(?:-)(?:(a)|(b)|(f)|(usage))";
	// -f : input from file => [-f <filename>]
	// ...

	// "4*X^2 + -8*X^1 + 3X^0 = -32X^2 + 12"

	std::string				str;
	std::smatch				match;
	size_t					offset = 0;

	str = input;
	// trouver le signe egal.
	while (std::regex_search(str, match, equationRegex))
	{
		
		str = m.suffix();	
	}
}

Parser::~Parser() {}
