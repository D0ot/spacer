/*
 * @file : main.cc
 * @author : d0ot @ github.com
 *      to test spacer.h
 */


// for axis is (0, 0, 1)
// angle is PI/2.0 
// it can rorate (1, 0, 0) to (0, 1, 0)

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <cmath>

const double PI  =3.141592653589793238463;

void learnEigen3(void)
{
    Eigen::Vector3d p(0, 1, 0);
    Eigen::Vector3d z(0, 0, 0);
    Eigen::Vector3d axis(0, 0, 0);
    axis.normalize();

    auto t2 = Eigen::Affine3d::Identity();
    t2.translate(p).rotate(Eigen::AngleAxisd(PI/2, axis)).pretranslate(p);

    Eigen::Vector3d p2(0, 5, 0);
    p2 = t2 * p2;
    std::cout << p2;
}



int main(void)
{
    learnEigen3();
    std::cout << std::endl;
    return 0;
}
