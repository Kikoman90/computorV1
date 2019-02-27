/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 12:12:07 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/27 20:23:45 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_HPP
# define SOLVER_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include <map>

// exit program upon first error
# define CV1_INTERRUPT false

// display results with colors
# define CV1_COLORS true

# define CV1_DEF    "\033[0m"
# define CV1_YEL    "\033[33m"
# define CV1_COLA   "\033[34m"
# define CV1_COLB   "\033[35m"
# define CV1_COLC   "\033[36m"
# define CV1_ERR    "\033[4;31m"
# define CV1_UDL    "\033[4m"

struct  polynomial {
	double		a;
	double		b;
	double		c;
	double      d;
};

class Solver {

public:
    Solver(std::map<unsigned int, double, std::greater<unsigned int> > &coefficients);
    ~Solver();

    void    run(bool showSteps) const;

private:
    void    print_reduced_form(bool colors) const;
    void    print_poly_info(bool showSteps, polynomial &poly, unsigned int poly_degree) const;

    std::map<unsigned int, double, std::greater<unsigned int> > const &_coefficients;


};

#endif