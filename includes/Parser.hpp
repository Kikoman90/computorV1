/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:57:18 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/27 20:22:50 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "Solver.hpp"
# include <sys/stat.h>
# include <fstream>
# include <errno.h>
# include <regex>

enum err_type {
	ERR_MISSING_LEFT_SIDE = 0,
	ERR_MISSING_RIGHT_SIDE,
	ERR_INV_TERM,
	ERR_UNKNOWN_INPUT,
	ERR_FILE,
	ERR_MISSING_FILE
};

class Parser {

public:
	Parser();
	~Parser();

	void		log_error(err_type type, std::string msg, bool throw_exception = true) const;
	// returns true if input is a valid equation
	bool		run(char const *input);
	bool		waiting_for_file() const;

private:
	Solver														_solver;
	bool														_showSteps;
	bool														_inputFromFile;
	std::ifstream												_filestream;
	std::map<unsigned int, double, std::greater<unsigned int> >	_coefficients;
	std::regex													_equationRegex;
	std::regex													_optionRegex;

	void		show_map() const;
	std::string	get_file(std::string const filename);
	void		new_term(std::string const &cstr, std::string const &dstr, bool right_side);
	bool		optionMatch(std::string const &str);
	bool		equationMatch(std::string const &str);


	class CV1Exception : public std::exception {
	
	public:
		CV1Exception(std::string const msg);
		~CV1Exception() throw();

		char const	*what() const throw();
	private:
		std::string	_msg;

	};

};

#endif
