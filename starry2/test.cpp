#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "utils.h"
#include "rotation.h"
#include "basis.h"
using namespace utils;
using namespace basis;
using namespace rotation;

/**
Working like a charm.

*/
void dIdtheta() {

    int y_deg = 1;
    int u_deg = 1;
    int lmax = 2;
    Vector<double> p_uy;
    Matrix<double> grad_p1;
    Matrix<double> grad_p2;
    double I1, I2, dIdtheta;
    double eps = 1.e-8;

    // Mock sph harm and limb darkening vectors
    Vector<double> y(9);
    Vector<double> p_u(9);
    y <<   2,
           3,
           5,
           7,
           0, 0, 0, 0, 0;
    p_u << 0,
           0,
           0.5,
           0,
           0, 0, 0, 0, 0;

    // Mock polynomial basis vector
    VectorT<double> pT(9);
    pT << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    double theta;

    // Mock rotation matrix
    Matrix<double> R(9, 9);
    R.setZero();
    R(0, 0) = 1;
    Matrix<double> dRdtheta(9, 9);
    dRdtheta.setZero();

    // Mock change of basis matrix
    Matrix<double> A1;
    A1 = Matrix<double>::Identity(9, 9);

    // Analytic derivative
    // dI/dtheta = p^T . d LD / d(A1 . R . y) . A1 . d(R . y) / dtheta
    theta = M_PI / 3;
    R(1, 1) = sin(theta);
    R(2, 2) = tan(theta);
    R(3, 3) = cos(theta);
    polymul(y_deg, Vector<double>(A1 * R * y), u_deg, p_u, lmax, p_uy, grad_p1, grad_p2);
    dRdtheta(1, 1) = cos(theta);
    dRdtheta(2, 2) = 1 / (cos(theta) * cos(theta));
    dRdtheta(3, 3) = -sin(theta);
    dIdtheta = pT * grad_p1 * A1 * dRdtheta * y;
    std::cout << "Analytic: " << dIdtheta << std::endl;

    // Numerical derivative
    theta = M_PI / 3 - eps;
    R(1, 1) = sin(theta);
    R(2, 2) = tan(theta);
    R(3, 3) = cos(theta);
    polymul(y_deg, Vector<double>(A1 * R * y), u_deg, p_u, lmax, p_uy);
    I1 = pT * p_uy;
    theta = M_PI / 3 + eps;
    R(1, 1) = sin(theta);
    R(2, 2) = tan(theta);
    R(3, 3) = cos(theta);
    polymul(y_deg, Vector<double>(A1 * R * y), u_deg, p_u, lmax, p_uy);
    I2 = pT * p_uy;
    dIdtheta = (I2 - I1) / (2 * eps);
    std::cout << "Numerical: " << dIdtheta << std::endl;

}
