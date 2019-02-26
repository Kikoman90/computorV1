/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:58:31 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/26 18:58:05 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser() : _solver(_coefficients), _showSteps(false), _inputFromFile(false) {

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

	// option regex match -> -f || -s || -sf || -sf
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

void			Parser::show_map() const {
	for (auto item : _coefficients)	{
		std::cout << item.second;
		if (item.first)
			std::cout << "X^" << item.first;
		std::cout << std::endl;
	}
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
	coefficient *= (right_side) ? -1 : 1;
	if (!_coefficients.insert(std::make_pair(degree, coefficient)).second)
		_coefficients.at(degree) += coefficient;

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

	bool				complete;
	std::smatch			match;
	std::string			coef_str;
	std::string			deg_str;
	//std::string const lol = str;
	//std::string const	arg = str;
	//size_t				offset = 0;

	std::sregex_iterator	sr_begin = std::sregex_iterator(str.begin(), str.end(), _equationRegex);
	std::sregex_iterator	sr_end = std::sregex_iterator();

	// EGIh erwighuarehjOi;defhwiu abh sdk; gmerdhoi fjweub fgsj giownx,jgoeiwj oierjdo ho
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
					log_error(ERR_MISSING_LEFT_SIDE, "\"" + str + "\"");
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
				coef_str += "1";
				deg_str = (match[11].matched) ? match[11].str() : "1";
			}
			new_term(coef_str, deg_str, complete);
		}
		else if (match[12].matched)
			log_error(ERR_UNKNOWN_INPUT, "\"" + match.str() + "\"");
	}
	if (!complete)
		log_error(ERR_MISSING_RIGHT_SIDE, "\"" + str + "\"");
	show_map();
	return (true);

	/*
	// while (!str.empty() && std::regex_search(str, match, _equationRegex)) {
	while (offset != str.length() && std::regex_search(str.begin() + offset, str.end(), match, _equationRegex)) {

		offset += match.length();
		if (match[1].matched) {
			if (match[2].matched)
				coef_str = match[2].str();
			else if (match[3].matched)
				coef_str = match[3].str();
			else {
				if (_coefficients.empty())
					log_error(ERR_MISSING_LEFT_SIDE, "\"" + str + "\"");
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
				coef_str += "1";
				deg_str = (match[11].matched) ? match[11].str() : "1";
			}
			new_term(coef_str, deg_str, complete);
		}
		else if (match[12].matched)
			log_error(ERR_UNKNOWN_INPUT, "\"" + match.str() + "\"");
		//str = match.suffix().str();
		//offset += match.length();
	}
	*/

}

bool			Parser::run(char const *input) {

	std::string				str = input;

	if (!_inputFromFile && optionMatch(str))
		return (false);
	else {
		_coefficients.clear();
		equationMatch((_inputFromFile) ? get_file(str) : str);
		_solver.run(_showSteps);
		return (true);
	}
	return (false);

}

bool			Parser::waiting_for_file() const { return (_inputFromFile); }

Parser::CV1Exception::CV1Exception(std::string const msg) : _msg(msg) {}

Parser::CV1Exception::~CV1Exception() throw() {}

char const		*Parser::CV1Exception::what() const throw() { return (_msg.c_str()); }

Parser::~Parser() {}
