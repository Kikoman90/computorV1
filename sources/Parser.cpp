/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:58:31 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/27 20:24:58 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

void			Parser::show_map() const {
	for (auto item : _coefficients)	{
		std::cout << item.second;
		if (item.first)
			std::cout << "X^" << item.first;
		std::cout << std::endl;
	}
}

Parser::Parser() : _solver(_coefficients), _showSteps(false), _inputFromFile(false) {

	_equationRegex = std::regex(
		"((?:(?:^\\s*([-+]?))|(?:\\s*([-+]))|(?:\\s*[=]\\s*([-+]?)))"
		"\\s*(?:((?:(?:(?:0+\\B)?((?:\\d+)(?:[.]\\d*)?))|([.]\\d+))"
		"(\\s*[*]?\\s*[xX](?:\\s*\\^\\s*(?:0+\\B)?(\\d+))?)?)|(([xX]"
		"(?:\\s*\\^\\s*(?:0+\\B)?(\\d+))?)(\\s*[*]\\s*(?:(?:(?:0+\\B)?"
		"((?:\\d+)(?:[.]\\d*)?))|([.]\\d+)))?)))|(.+)");

	_optionRegex = std::regex("\\s*[-](?:(s)|(f))");

}

void			Parser::log_error(err_type type, std::string msg, bool throw_exception) const {
	
	std::stringstream			log_msg;
	static std::string const	error[] = {
		"missing left side of equation",
		"missing right side of equation",
		"invalid term",
		"unknow or invalid input",
		"file error",
		"missing argument"
	};

	log_msg << '\n' << CV1_ERR << "ERROR:" << CV1_DEF << ' ';
	if (type >= 0 && type <= 5)
		log_msg << error[type];
	if (!msg.empty())
		log_msg << " -> " << msg;
	if (throw_exception)
		throw Parser::CV1Exception::CV1Exception(log_msg.str());
	else
		std::cout << log_msg.str() << std::endl;

}

std::string		Parser::get_file(std::string const filename) {

	struct stat			s;
	std::stringstream	strs;

	_inputFromFile = false;
	if (stat(filename.c_str(), &s) == 0) {
		if (!(s.st_mode & S_IFREG))
			log_error(ERR_FILE, CV1_UDL + filename + CV1_DEF + ": argument is not a file");
	}   
	else
		log_error(ERR_FILE, CV1_UDL + filename + CV1_DEF + ": " + strerror(errno));
	_filestream.open(filename, std::ifstream::in);
	if (!_filestream)
		log_error(ERR_FILE, CV1_UDL + filename + CV1_DEF + ": file does not exist or could not be opened");
	strs << _filestream.rdbuf();
	_filestream.close();
	return (strs.str());

}

unsigned int	stoui(std::string const &str) {

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

	try {
		degree = stoui(dstr);
		coefficient = std::stod(cstr);
	}
	catch (std::exception &e) {
		log_error(ERR_INV_TERM, e.what());
	}
	if (!coefficient)
		return ;
	coefficient *= (right_side) ? -1 : 1;
	if (!_coefficients.insert(std::make_pair(degree, coefficient)).second) {
		if (!(_coefficients.at(degree) += coefficient))
			_coefficients.erase(_coefficients.find(degree));
	}

}

bool			Parser::optionMatch(std::string const &str) {

	std::smatch	match;

	if (std::regex_match(str, match, _optionRegex)) {
		if (match[1].matched)
			_showSteps = true;
		else if (match[2].matched)
			_inputFromFile = true;
		return (true);
	}
	return (false);

}

bool			Parser::equationMatch(std::string const &str) {

	bool					complete;
	std::string				coef_str;
	std::string				deg_str;
	std::smatch				match;
	std::sregex_iterator	sr_begin = std::sregex_iterator(str.begin(), str.end(), _equationRegex);
	std::sregex_iterator	sr_end = std::sregex_iterator();

	complete = false;
	for (std::sregex_iterator it = sr_begin; it != sr_end; ++it) {
		match = *it;
		if (match[1].matched) {
			if (match[2].matched)
				coef_str = match[2].str();
			else if (match[3].matched)
				coef_str = match[3].str();
			else {
				if (_coefficients.empty())
					log_error(ERR_MISSING_LEFT_SIDE, (str.length() > 25) ? "\"" + str.substr(0, 25) + "...\"" : "\"" + str + "\"");
				coef_str = match[4].str();
				complete = true;
			}
			if (match[5].matched) {
				coef_str += (match[6].matched) ? match[6].str() : match[7].str();
				if (match[8].matched)
					deg_str = (match[9].matched) ? match[9].str() : "1";
				else
					deg_str = "0";
			}
			else {
				deg_str = (match[12].matched) ? match[12].str() : "1";
				if (match[13].matched)
					coef_str += (match[14].matched) ? match[14].str() : match[15].str();
				else
					coef_str += "1";
			}
			new_term(coef_str, deg_str, complete);
		}
		else if (match[16].matched)
			log_error(ERR_UNKNOWN_INPUT, (match.str().length() > 25) ? "\"" + match.str().substr(0, 25) + "...\"" : "\"" + match.str() + "\"");
	}
	if (!complete)
		log_error(ERR_MISSING_RIGHT_SIDE, (str.length() > 25) ? "\"" + str.substr(0, 25) + "...\"" : "\"" + str + "\"");
	return (true);

}

bool			Parser::run(char const *input) {

	std::string				str = input;

	if (!_inputFromFile && optionMatch(str))
		return (false);
	else {
		_coefficients.clear();
		equationMatch((_inputFromFile) ? get_file(str) : str);
		_solver.run(_showSteps);
		_showSteps = false; //
		return (true);
	}
	return (false);

}

bool			Parser::waiting_for_file() const { return (_inputFromFile); }

Parser::CV1Exception::CV1Exception(std::string const msg) : _msg(msg) {}

Parser::CV1Exception::~CV1Exception() throw() {}

char const		*Parser::CV1Exception::what() const throw() { return (_msg.c_str()); }

Parser::~Parser() {}
