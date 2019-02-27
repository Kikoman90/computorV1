/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 13:10:15 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/27 20:16:42 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Solver.hpp"

Solver::Solver(std::map<unsigned int, double, std::greater<unsigned int> > &coefficients) : _coefficients(coefficients) {}

void    Solver::print_reduced_form(bool colors) const {
    
    std::map<unsigned int, double, std::greater<unsigned int> >::const_iterator c_it;
    double                                                                      coef;
    unsigned int                                                                deg;

    std::cout << "Reduced form: ";
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
                    std::cout << CV1_COLA;
                    break;
                case 1:
                    std::cout << CV1_COLB;
                    break;
                case 2:
                    std::cout << CV1_COLC;
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

void    Solver::print_poly_info(bool showSteps, polynomial &poly, unsigned int poly_degree) const {
    
    (void)showSteps;
    (void)poly;
    (void)poly_degree;
    /*std::cout << "Polynomial degree: " << poly_degree << std::endl;
    if (poly_degree == 1 || poly_degree == 2) {
        if (poly_degree == 2 && _coefficients.find(2) != _coefficients.end())
            poly.a = _coefficients.at(2);
        if (_coefficients.find(1) != _coefficients.end())
            poly.b = _coefficients.at(1);
        if (_coefficients.find(0) != _coefficients.end())
            poly.c = _coefficients.at(0);
        if (showSteps) {
            std::cout << "The equation is of the form " << CV1_COLA
        }
    }*/

}

void    Solver::run(bool showSteps) const {

    polynomial      poly;
    unsigned int    poly_degree;

    (void)poly;
    (void)showSteps;
    poly_degree = _coefficients.begin()->first;
    print_reduced_form((CV1_COLORS && (poly_degree == 1 || poly_degree == 2)));
    //print_poly_info(showSteps, poly, poly_degree);
}

Solver::~Solver() {}