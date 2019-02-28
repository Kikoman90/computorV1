/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 13:10:15 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/28 19:19:32 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Solver.hpp"

Solver::Solver(std::map<unsigned int, double, std::greater<unsigned int> > &coefficients) : _coefficients(coefficients) {}

void    Solver::print_reduced_form(bool colors) const {
    
    std::map<unsigned int, double, std::greater<unsigned int> >::const_iterator c_it;
    double                                                                      coef;
    unsigned int                                                                deg;

    std::cout << "  * Reduced form: ";
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

void    Solver::solve_linear(bool showSteps, polynomial &poly) const {
    (void)showSteps;
    (void)poly;
}

void    Solver::solve_quadratic(bool showSteps, polynomial &poly) const {
    (void)showSteps;
    (void)poly;
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