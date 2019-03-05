/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 12:12:07 by fsidler           #+#    #+#             */
/*   Updated: 2019/03/01 19:22:10 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_HPP
# define SOLVER_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include <map>
# include <cmath>

// exit program upon first error
# define CV1_INTERRUPT false

// display resolution with colors
# define CV1_COLORS true

// display results as irreducible fractions if possible
# define CV1_FRACTIONS true

// use 32 bits floating point numbers : CV1_USE_FP
// use doubles. 64 bits FP numbers : CV1_USE_DBL
# define CV1_USE_FP 0x1
# define CV1_USE_DBL 0x2
# define CV1_MODE CV1_USE_DBL

# define CV1_DEF "\033[0m"
# define CV1_GRE "\033[32m"
# define CV1_YEL "\033[33m"
# define CV1_BLU "\033[34m"
# define CV1_MAG "\033[35m"
# define CV1_CYA "\033[36m"

# define CV1_EQ(x) "\033[4;97;102m" << x << CV1_DEF
# define CV1_UDL(x) "\033[4m" + x + CV1_DEF
# define CV1_ERR(x) "\033[4;31m" << x << CV1_DEF

# if CV1_COLORS
#  define CV1_COLA(x) CV1_BLU << x << CV1_DEF
#  define CV1_COLB(x) CV1_MAG << x << CV1_DEF
#  define CV1_COLC(x) CV1_CYA << x << CV1_DEF
#  define CV1_COLD(x) CV1_YEL << x << CV1_DEF
#  define CV1_COLS(x) CV1_GRE << x << CV1_DEF
# else
#  define CV1_COLA(x) x
#  define CV1_COLB(x) x
#  define CV1_COLC(x) x
#  define CV1_COLD(x) x
#  define CV1_COLS(x) x
# endif

struct  polynomial {
	double		a;
	double		b;
	double		c;
	double      delta;
};

class Solver {

public:
    Solver(std::map<unsigned int, double, std::greater<unsigned int> > &coefficients);
    ~Solver();

    void    run(bool showSteps) const;

private:
    void    print_reduced_form(bool colors) const;
    void    get_polynomial(polynomial &poly) const;
    void    print_poly_info(bool showSteps, polynomial const poly, unsigned int const poly_degree) const;

    bool    displayable_as_fraction(double &numerator, double &denominator) const;

    void    case_zero(bool showSteps, polynomial &poly) const;
    void    case_positive(bool showSteps, polynomial &poly) const;
    void    case_negative(bool showSteps, polynomial &poly) const;

    void    solve_linear(polynomial &poly) const;
    void    solve_quadratic(bool showSteps, polynomial &poly) const;
    
    void    solve(bool showSteps, polynomial &poly, unsigned int const poly_degree) const;

    std::map<unsigned int, double, std::greater<unsigned int> > const &_coefficients;

};

#endif