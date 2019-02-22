/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:58:31 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/22 19:25:51 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/*
Parser::Parser(Solver &solver) : _solver(solver) {

}
*/

Parser::Parser() {

	// equation regex match -> group 1 | group 12
	// group 1 -> (2|3|4)[whitespaces]?(5|10)
	// group 5 -> (6|7)8?
	_equationRegex =
		std::regex("(" // 1 : term match (a*X^d) with a = coefficient (constant) and d = degree of the indeterminate X
						"(?:"
							"(?:^\\s*([-+]?))|" // 2 : beginning of line (optional sign)
							"(?:\\s*([-+]))|" // 3 : otherwise, operator is not optional
							"(?:\\s*[=]\\s*([-+]?))" // 4 : monomial right after '=' sign is optional
						")"
						"\\s*" // ignore whitespaces
						"(?:"
							"(" // 5 : term with a coefficient (with or without X)
								"(?:"
									"(?:"
										"(?:0+\\B)?"
										"((?:\\d+)(?:[.]\\d*)?)" // 6 : digits with optional decimals
									")|"
									"([.]\\d+)" // 7 : decimal point followed by digits
								")"
							"(\\s*[*]?\\s*[xX](?:\\^(?:0+\\B)?(\\d+))?)?)|" // 8 : X part of the term (optional); 9 : the degree (optional)
						"([xX](?:\\^(?:0+\\B)?(\\d+))?))" // 10 : single X (factor = 1); 11 : the degree (optional)
					")|(.*)"); // 12 : any other match (=invalid)

	_optionRegex = std::regex("");

}

unsigned int	stoui(std::string const &str)
{
	unsigned long	lres;
	unsigned int	res;

	lres = std::stoul(str);
	if (lres >= UINT_MAX)
    	throw std::out_of_range("result out of unsigned integer range (UINT_MAX)");
	res = static_cast<unsigned int>(lres);
    return res;
}

void			Parser::new_term(std::string const &cstr, std::string const &dstr, bool right_side) {

	unsigned int	degree;
	double			coefficient;

	degree = stoui(dstr);
	coefficient = std::stod(cstr);
	coefficient *= (right_side) ? -1 : 1;
	if (!_coefficients.insert(std::make_pair(degree, coefficient)).second)
		_coefficients.at(degree) += coefficient;
}

void			Parser::show_map() const {
	for (auto item : _coefficients)	{
		std::cout << item.second;
		if (item.first)
			std::cout << "X^" << item.first;
		std::cout << std::endl;
	}
}



bool			Parser::run(char const *input) {

	//tester 0X^2, -0X^2

	// -f : input from file => [-f <filename>]
	// ...
	std::string				str = input;
	std::string				coef_str;
	std::string				deg_str;
	std::smatch				match;
	bool					right_side = false;

	while (!str.empty() && std::regex_search(str, match, _equationRegex))
	{
		if (match[1].matched)
		{
			if (match[2].matched)
				coef_str = match[2].str();
			else if (match[3].matched)
				coef_str = match[3].str();
			else
			{
				if (_coefficients.empty())
				{
					std::cout << "the equation is invalid my dude\n";
					//exit;
				}
				coef_str = match[4].str();
				right_side = true;
			}
			if (match[5].matched) {
				coef_str += (match[6].matched) ? match[6].str() : match[7].str();
				if (match[8].matched)
					deg_str = (match[9].matched) ? match[9].str() : "1";
				else
					deg_str = "0";
			}
			else {
				coef_str += "1";
				deg_str = (match[11].matched) ? match[11].str() : "1";
			}
			new_term(coef_str, deg_str, right_side);
		}
		else if (match[12].matched) {
			std::cout << "syntax error : '" << match.str() << "'\n";
			str = match.suffix().str();
			break ;
		}
		str = match.suffix().str();
	}
	std::cout << "final str = '" << str << "'\n";
	show_map();
	return (true);
}

Parser::~Parser() {}
