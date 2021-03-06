/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 13:10:15 by fsidler           #+#    #+#             */
/*   Updated: 2019/03/07 19:01:32 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Solver.hpp"

Solver::Solver(std::map<unsigned int, double, std::greater<unsigned int> > &coefficients) : _coefficients(coefficients) {}

void    Solver::print_reduced_form(bool colors) const {
    
    std::map<unsigned int, double, std::greater<unsigned int> >::const_iterator c_it;
    double                                                                      coef;
    unsigned int                                                                deg;

    std::cout << "  * Reduced form : ";
    if (_coefficients.empty()) {
        std::cout << "0 = 0" << std::endl;
        return ;
    }
    for (c_it = _coefficients.cbegin(); c_it != _coefficients.cend(); ++c_it) {
        deg = c_it->first;
        coef = c_it->second;
        if (colors) {
            switch (deg) {
                case 0:
                    std::cout << CV1_BLU;
                    break;
                case 1:
                    std::cout << CV1_MAG;
                    break;
                case 2:
                    std::cout << CV1_CYA;
                    break;
                default:
                    std::cout << CV1_DEF;
            }
        }
        if (coef) {
            if (c_it != _coefficients.cbegin()) {
                if (coef < 0) {
                    std::cout << "- ";
                    coef *= -1;
                }
                else
                    std::cout << "+ ";
            }
            std::cout << coef << CV1_DEF;
            if (deg == 1)
                std::cout << 'X';
            else if (deg > 1)
                std::cout << "X^" << deg;
            std::cout << ' ';
        }
    }
    std::cout << CV1_DEF << "= 0" << std::endl;

}

void    Solver::get_polynomial(polynomial &poly) const {

    poly.a = (_coefficients.find(2) != _coefficients.end()) ? _coefficients.at(2) : 0;
    poly.b = (_coefficients.find(1) != _coefficients.end()) ? _coefficients.at(1) : 0;
    poly.c = (_coefficients.find(0) != _coefficients.end()) ? _coefficients.at(0) : 0;

}

void    Solver::print_poly_info(bool showSteps, polynomial const poly, unsigned int const poly_degree) const {
    
    static std::string const poly_names[] = {
        " (constant)",
        " (linear)",
        " (quadratic)",
        " (cubic)",
        " (quartic)",
        " (quintic)",
        " (sextic)",
        " (septic)"
    };

    if (showSteps && (poly_degree == 1 || poly_degree == 2)) {
        std::cout << "  *\n";
        std::cout << "  * The polynomial equation of degree " << poly_degree << poly_names[poly_degree] << " is of the form : ";
        if (poly_degree == 2)
            std::cout << CV1_COLA('a') << "x^2 + ";
        std::cout << CV1_COLB('b') << "x + " << CV1_COLC('c') << " = 0\n  * with : ";
        if (poly_degree == 2)
            std::cout << CV1_COLA("a = " << poly.a) << "; ";
        std::cout << CV1_COLB("b = " << poly.b) << "; " << CV1_COLC("c = " << poly.c) << ";\n";
    }
    else {
        std::cout << "  * Polynomial degree : " << poly_degree;
        if (poly_degree <= 7)
            std::cout << poly_names[poly_degree];
        std::cout << std::endl;
    }

}

double	ft_fabs(double n) {

	int64_t	casted;

	casted = *reinterpret_cast<int64_t*>(&n);
	casted &= 0x7FFFFFFFFFFFFFFF;
	return (*reinterpret_cast<double*>(&casted));

}

// behaviour is identical to that of std::modf(double, double*)
double  ft_modf(double value, double *iptr = NULL) {

	struct cv1_f64  *f64ptr;
	int             exponent;
	int             mask_bits;
	double          tmp;

	f64ptr = reinterpret_cast<struct cv1_f64*>(&value);
	exponent = (f64ptr->high32 >> CV1_F64_EXP_SHIFT) & CV1_F64_EXP_MASK;
	exponent -= CV1_F64_EXP_BIAS;
	if (exponent < 0) { // the value has no integral part (decimal digits only)
		*iptr = 0;
		return (value);
	}
	mask_bits = 52 - exponent;
	if (mask_bits <= 0) { // all mantissa bits will be raised to an integral value by the exponent (no decimal digits)
		*iptr = value;
		return (0);
	}
	tmp = value;
	if (mask_bits >= 32) { // low32 will be decimal, we are only concerned with the first 20 bits of the mantissa
		f64ptr->low32 = 0;
		mask_bits -= 32;
		f64ptr->high32 &= ~((1 << mask_bits) - 1); // the mantissa bits that will be decimal are set to 0
	}
	else
		f64ptr->low32 &= ~((1 << mask_bits) - 1); // the mantissa bits that will be decimal are set to 0
	*iptr = value;
	return (tmp - value);

}

// modulo for doubles
double	ft_fmod(double n1, double n2) {

    double  trunc_div;

    ft_modf(n1 / n2, &trunc_div);
    return (n1 - trunc_div * n2);

}

// finding sqrt(S) is the same as solving f(x) = x^2 - S = 0.
// we use newton's method for finding the root of this equation:
// x[n+1] = x[n] - f(x[n]) / f'(x[n]) = x[n] - (x[n] - S / 2x[n])
//        = 0.5 * (x[n] + S / x[n])
// as we iterate, x converges towards the root
double  ft_sqrt(double n) {

    double  last, current;
    double  precision = 0.000001;

    last = n / 2;
    while (true) {
        current = 0.5 * (last + n / last);
        if (ft_fabs(current - last) < precision)
            return (current);
        last = current;
    }
    return (current);

}

double  highest_common_factor(double d1, double d2) {

    if (d2 == 0)
        return (d1);
    return (highest_common_factor(d2, ft_fmod(d1, d2)));

}

bool    Solver::displayable_as_fraction(double &numerator, double &denominator) const {

    double  num_int, den_int;
    double  hcf;

    //if (ft_modf(numerator, &num_int) || ft_modf(denominator, &den_int))
    if (ft_modf(numerator, &num_int) >= CV1_EPSILON ||\
        ft_modf(denominator, &den_int) >= CV1_EPSILON)
        return (false);
    numerator = num_int;
    denominator = den_int;
    hcf = highest_common_factor(numerator, denominator);
    numerator /= hcf;
    denominator /= hcf;
    return (true);

}

void    Solver::fraction_display(std::stringstream &strs, double numerator, double denominator, bool space) const {

    double  num, den;
    bool    negative;
    
    num = ft_fabs(numerator);
    den = ft_fabs(denominator);
    negative = ((numerator < 0 && denominator > 0) || (numerator > 0 && denominator < 0));
    if (displayable_as_fraction(num, den)) {
        if (negative)
            strs << ((space) ? " - " : "-");
        else if (space)
            strs << " + ";
        (den == 1) ? strs << num : strs << num << '/' << den;
    }
    else {
        if (space) {
            strs << ((negative) ? " - " : " + ");
            strs << num / den;
        }
        else
            strs << numerator / denominator;
    }
    
}

void    Solver::solve_linear(bool showSteps, polynomial &poly) const {
    
    std::stringstream   strs;
    double              num, den;

    num = -poly.c;
    den = poly.b;
    if (ft_fabs(num) <= CV1_EPSILON)
        strs << '0';
    else if (CV1_FRACTIONS)
        fraction_display(strs, num, den);
    else
		strs << num / den;

    if (showSteps)
        std::cout << "  *\n";
    std::cout << "  * The real " << CV1_COLS(strs.str()) << " is solution to the equation" << std::endl;

}

void    Solver::case_zero(bool showSteps, polynomial &poly) const {

    std::stringstream   strs;
    double              num, den;

    num = -poly.b;
    den = 2 * poly.a;
    if (ft_fabs(num) <= CV1_EPSILON) {
        num = 0;
        strs << '0';
    }
    else if (CV1_FRACTIONS)
		fraction_display(strs, num, den);
	else
		strs << num / den;

    if (showSteps) {
        std::cout << "  *\n";
        std::cout << "  * Let " << CV1_COLS('r') << " be the solution :\n";
        std::cout << "  * " << CV1_COLS('r') << " = -" << CV1_COLB('b') << " / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *   = -(" << CV1_COLB(poly.b) << ") / 2 * " << CV1_COLA(poly.a) << std::endl;
        std::cout << "  *   = " << num << " / " << den << std::endl;
        std::cout << "  *   " << CV1_COLS("= " << strs.str()) << std::endl;
        std::cout << "  *\n";
    }

    std::cout << "  * The equation has one real solution : " << CV1_COLS(strs.str()) << std::endl;

}

void    Solver::case_positive(bool showSteps, polynomial &poly) const {

    std::stringstream   strs1, strs2;
    double              sqrtDelta;
    double              num, den;

    sqrtDelta = ft_sqrt(poly.delta);
    num = -poly.b - sqrtDelta;
    den = 2 * poly.a;
    if (ft_fabs(num) <= CV1_EPSILON) {
        num = 0;
        strs1 << '0';
    }
    else if (CV1_FRACTIONS)
        fraction_display(strs1, num, den);
    else
        strs1 << num / den;

    if (showSteps) {
        std::cout << "  *\n";
        std::cout << "  * Let " << CV1_COLS("r1") << " and " << CV1_COLS("r2") << " be the solutions :\n";
        std::cout << "  * " << CV1_COLS("r1") << " = (-" << CV1_COLB('b') << " - sqrt(" << CV1_COLD('D') << ")) / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = (-(" << CV1_COLB(poly.b) << ") - sqrt(" << CV1_COLD(poly.delta) << ")) / 2 * " << CV1_COLA(poly.a) << std::endl;
        std::cout << "  *    = (" << -poly.b << " - " << sqrtDelta << ") / " << den << std::endl;
        std::cout << "  *    = " << num << " / " << den << std::endl;
        std::cout << "  *    " << CV1_COLS("= " << strs1.str()) << std::endl;
    }

    num = -poly.b + sqrtDelta;
    if (ft_fabs(num) <= CV1_EPSILON) {
        num = 0;
        strs2 << '0';
    }
    else if (CV1_FRACTIONS)
        fraction_display(strs2, num, den);
    else
        strs2 << num / den;

    if (showSteps) {
        std::cout << "  * " << CV1_COLS("r2") << " = (-" << CV1_COLB('b') << " + sqrt(" << CV1_COLD('D') << ")) / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = (-(" << CV1_COLB(poly.b) << ") + sqrt(" << CV1_COLD(poly.delta) << ")) / 2 * " << CV1_COLA(poly.a) << std::endl;
        std::cout << "  *    = (" << -poly.b << " + " << sqrtDelta << ") / " << den << std::endl;
        std::cout << "  *    = " << num << " / " << den << std::endl;
        std::cout << "  *    " << CV1_COLS("= " << strs2.str()) << std::endl;
        std::cout << "  *\n";
    }

    std::cout << "  * The equation has two real solutions : " << CV1_COLS(strs1.str()) << " and " << CV1_COLS(strs2.str()) << std::endl;

}

void    Solver::case_negative(bool showSteps, polynomial &poly) const {

    std::stringstream   strs1, strs2;
    double              num_r, num_i, den;
    double              res_i;
    bool                r_exists = true;

    num_r = -poly.b;
    num_i = ft_sqrt(ft_fabs(poly.delta));
    den = 2 * poly.a;
    if (ft_fabs(num_r) <= CV1_EPSILON) {
        num_r = 0;
        r_exists = false;
    }
    else {
        if (CV1_FRACTIONS)
            fraction_display(strs1, num_r, den);
        else
            strs1 << num_r / den;
        strs2 << strs1.str(); // same real part for both solutions;
    }

    if (CV1_FRACTIONS) {
        fraction_display(strs1, -num_i, den, r_exists);
        fraction_display(strs2, num_i, den, r_exists);
    } else {
        res_i = -num_i / den;
        if (r_exists) {
            if (res_i < 0) {
                strs1 << " - " << -res_i;
                strs2 << " + " << -res_i;
            }
            else {
                strs1 << " + " << res_i;
                strs2 << " - " << res_i;
            }
        }
        else {
            strs1 << res_i;
            strs2 << -res_i;
        }
    }
    strs1 << " * i";
    strs2 << " * i";

    if (showSteps) {
        std::cout << "  *\n";
        std::cout << "  * Let " << CV1_COLS("r1") << " and " << CV1_COLS("r2") << " be the solutions :\n";
        std::cout << "  * " << CV1_COLS("r1") << " = (-" << CV1_COLB('b') << " - sqrt(" << CV1_COLD('D') << ")) / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = (-" << CV1_COLB('b') << " - sqrt(-" << CV1_COLD('D') << " * i^2)) / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = -" << CV1_COLB('b') << " / 2" << CV1_COLA('a') << " - i * sqrt(-" << CV1_COLD('D') << ") / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = -(" << CV1_COLB(poly.b) << ") / 2 * " << CV1_COLA(poly.a) << " - i * sqrt(" << CV1_COLD(-poly.delta) << ") / 2 * " << CV1_COLA(poly.a) << std::endl;
        std::cout << "  *    = " << num_r << " / " << den << " - i * " << num_i << " / " << den << std::endl;
        std::cout << "  *    " << CV1_COLS("= " << strs1.str()) << std::endl;

        std::cout << "  * " << CV1_COLS("r2") << " = (-" << CV1_COLB('b') << " + sqrt(" << CV1_COLD('D') << ")) / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = (-" << CV1_COLB('b') << " + sqrt(-" << CV1_COLD('D') << " * i^2)) / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = -" << CV1_COLB('b') << " / 2" << CV1_COLA('a') << " + i * sqrt(-" << CV1_COLD('D') << ") / 2" << CV1_COLA('a') << std::endl;
        std::cout << "  *    = -(" << CV1_COLB(poly.b) << ") / 2 * " << CV1_COLA(poly.a) << " + i * sqrt(" << CV1_COLD(-poly.delta) << ") / 2 * " << CV1_COLA(poly.a) << std::endl;
        std::cout << "  *    = " << num_r << " / " << den << " + i * " << num_i << " / " << den << std::endl;
        std::cout << "  *    " << CV1_COLS("= " << strs2.str()) << std::endl;
        std::cout << "  *\n";
    }

    std::cout << "  * The equation has two complex solutions : " << CV1_COLS(strs1.str()) << " and " << CV1_COLS(strs2.str()) << std::endl;

}

void    Solver::solve_quadratic(bool showSteps, polynomial &poly) const {

    poly.delta = poly.b * poly.b - 4 * poly.a * poly.c;

    if (showSteps) {
        std::cout << "  *\n";
        std::cout << "  * Let " << CV1_COLD('D') << " be the discriminant :\n";
        std::cout << "  * " << CV1_COLD('D') << " = " << CV1_COLB('b') << "^2 - 4" << CV1_COLA('a') << CV1_COLC('c') << std::endl;
        std::cout << "  *   = " << CV1_COLB(poly.b) << "^2 - 4 * " << CV1_COLA(poly.a) << " * " << CV1_COLC(poly.c) << std::endl;
        std::cout << "  *   " << CV1_COLD("= " << poly.delta) << std::endl;
        std::cout << "  *\n";
    }
    std::cout << "  * The discriminant " << CV1_COLD('D') << " is equal to " << CV1_COLD(poly.delta) << std::endl;
 
    if (poly.delta == 0)
        case_zero(showSteps, poly);
    else if (poly.delta > 0)
        case_positive(showSteps, poly);
    else
        case_negative(showSteps, poly);

}

void    Solver::solve(bool showSteps, polynomial &poly, unsigned int const poly_degree) const {

    switch (poly_degree) {

        case 0:
            std::cout << "  * " << ((poly.c) ? "No real numbers are solution of the equation\n" : "All real numbers are solution of the equation\n");
            break;
        case 1:
            solve_linear(showSteps, poly);
            break;
        case 2:
            solve_quadratic(showSteps, poly);
            break;
        default:
            std::cout << "  * ComputorV1 does not solve polynomial equations of degree higher than 2\n";

    }
    
}

void    Solver::run(bool showSteps) const {

    polynomial      poly;
    unsigned int    poly_degree;

    poly_degree = (_coefficients.empty()) ? 0 : _coefficients.begin()->first;
    print_reduced_form((CV1_COLORS && (poly_degree == 1 || poly_degree == 2)));
    if (poly_degree <= 2)
        get_polynomial(poly);
    print_poly_info(showSteps, poly, poly_degree);
    solve(showSteps, poly, poly_degree);

}

Solver::~Solver() {}