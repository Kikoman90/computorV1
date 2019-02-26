/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 12:12:07 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/26 17:55:17 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_HPP
# define SOLVER_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include <map>

# define CV1_DEF    "\033[0m"
# define CV1_YEL    "\033[33m"
# define CV1_ERR    "\033[4;31m"
# define CV1_UDL    "\033[4m"

struct  polynomial {
	double		a;
	double		b;
	double		c;
	int			degree;
};

class Solver {

public:
    Solver(std::map<unsigned int, double, std::greater<unsigned int> > &coefficients);
    ~Solver();

    void    run(bool showSteps);

private:
    std::map<unsigned int, double, std::greater<unsigned int> > &_coefficients;


};

#endif