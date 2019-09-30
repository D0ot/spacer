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
#include "spacer.h"

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
void nodetest()
{
    Spacer::Node root("root", Eigen::Vector3d(0,0,0),
                       Eigen::Vector3d(0,0,0), 
                       Eigen::AngleAxisd(0, Eigen::Vector3d(1,1,1)));
    
    auto shoulder = root.addChild("shoulder", 
                Eigen::Vector3d(10,0,0), 
                Eigen::Vector3d(0,0,0), 
                Eigen::AngleAxisd(-PI/2, Eigen::Vector3d(1,0,0)));
    

    std::vector<Eigen::Vector3d> points;
    points.emplace_back(0,0,5);

    if(shoulder)
    {
        std::cout << "shoulder not null\n" ;
    }
    shoulder->tranform(points);

    for(auto& x:points)
    {
        std::cout << x << std::endl;
    }

    auto leg = shoulder->addChild("leg",
                Eigen::Vector3d(5, 5, 0),
                Eigen::Vector3d(0, 0, 0),
                Eigen::AngleAxisd(PI/2, Eigen::Vector3d(0,1,0)));

    std::vector<Eigen::Vector3d> points2;
    points2.emplace_back(0,0,5);

    if(leg)
    {
        std::cout << "left valid\n";
    }
    auto affine = leg->getTransformLocal();

    leg->tranform(points2);

    for(auto& x:points2)
    {
        std::cout << x << std::endl;
    }
}    



int main(void)
{
    //learnEigen3();
    nodetest();


    return 0;
}
