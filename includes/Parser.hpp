/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:57:18 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/20 17:52:33 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

class Parser {

public:
	Parser(...);
	~Parser();

private:
	std::regex	_aRegex;
	std::regex	_bRegex;
	std::regex	_cRegex;
	std::regex	_optionsRegex;

};

#endif
