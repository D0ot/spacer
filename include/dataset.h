#ifndef DATASET_H_
#define DATASET_H_

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <string>

struct BodyPart
{
    std::string name;
    std::string parent;
    Eigen::Vector3d translation;
    double mass;
    Eigen::Vector3d anchor;
    Eigen::Vector3d axis;
    double min;
    double max;
    
};

BodyPart parts[] =
{
    BodyPart{"neck", "torso", Eigen::Vector3d(0,0,90), 0.05, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,1), -120, 120},
    BodyPart{"head", "neck", Eigen::Vector3d(0,0,65), 0.35, Eigen::Vector3d(0,0,-5), Eigen::Vector3d(1,0,0), -45, 45},

    BodyPart{"shoulder_r", "torso", Eigen::Vector3d(98,0,75), 0.07, Eigen::Vector3d(0,0,0), Eigen::Vector3d(1,0,0), -120, 120},
    BodyPart{"shoulder_l", "torso", Eigen::Vector3d(-98,0,75), 0.07, Eigen::Vector3d(0,0,0), Eigen::Vector3d(1,0,0), -120, 120},

    BodyPart{"upperarm_r", "shoulder_r", Eigen::Vector3d(10,20,0), 0.15, Eigen::Vector3d(10,20,0), Eigen::Vector3d(0,0,1), -95, 1},
    BodyPart{"upperarm_l", "shoulder_l", Eigen::Vector3d(-10,20,0), 0.15, Eigen::Vector3d(-10,20,0), Eigen::Vector3d(0,0,1), -1, 95},
    
    BodyPart{"elbow_r", "upperarm_r", Eigen::Vector3d(-10,70,9), 0.035, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,1,0), -120, 120},
    BodyPart{"elbow_l", "upperarm_l", Eigen::Vector3d(10,70,9), 0.035, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,1,0), -120, 120},

    BodyPart{"lowerarm_r", "elbow_r", Eigen::Vector3d(0,50,0), 0.2, Eigen::Vector3d(0,50,0), Eigen::Vector3d(0,0,1), -1, 90},
    BodyPart{"lowerarm_l", "elbow_l", Eigen::Vector3d(0,50,0), 0.2, Eigen::Vector3d(0,50,0), Eigen::Vector3d(0,0,1), -90, 1},

    BodyPart{"hip1_r", "torso", Eigen::Vector3d(55,-10,-115), 0.09, Eigen::Vector3d(0,0,0), Eigen::Vector3d(-0.707,0,0.707), -90, 1},
    BodyPart{"hip1_l", "torso", Eigen::Vector3d(-55,-10,-115), 0.09, Eigen::Vector3d(0,0,0), Eigen::Vector3d(-0.707,0,-0.707), -90, 1},

    BodyPart{"hip2_r", "hip1_r", Eigen::Vector3d(0,0,0), 0.125, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,1,0), -45, 25},
    BodyPart{"hip2_l", "hip1_l", Eigen::Vector3d(0,0,0), 0.125, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,1,0), -25, 45},

    BodyPart{"thigh_r", "hip2_r", Eigen::Vector3d(0,10,-40), 0.275, Eigen::Vector3d(0,10,-40), Eigen::Vector3d(1,0,0), -25, 100},
    BodyPart{"thigh_l", "hip2_l", Eigen::Vector3d(0,10,-40), 0.275, Eigen::Vector3d(0,10,-40), Eigen::Vector3d(1,0,0), -25, 100},

    BodyPart{"shank_r","thigh_r", Eigen::Vector3d(0,5,-125), 0.225, Eigen::Vector3d(0,-10,45), Eigen::Vector3d(1,0,0), -130, 1},
    BodyPart{"shank_l","thigh_l", Eigen::Vector3d(0,5,-125), 0.225, Eigen::Vector3d(0,-10,45), Eigen::Vector3d(1,0,0), -130, 1},

    BodyPart{"ankle_r","shank_r", Eigen::Vector3d(0,-10,-55), 0.125, Eigen::Vector3d(0,0,0), Eigen::Vector3d(1,0,0), -45, 75},
    BodyPart{"ankle_l","shank_l", Eigen::Vector3d(0,-10,-55), 0.125, Eigen::Vector3d(0,0,0), Eigen::Vector3d(1,0,0), -45, 75},

    BodyPart{"foot_r","ankle_r", Eigen::Vector3d(0,30,-40), 0.2, Eigen::Vector3d(0,30,-40), Eigen::Vector3d(0,1,0), -25, 45},
    BodyPart{"foot_l","ankle_l", Eigen::Vector3d(0,30,-40), 0.2, Eigen::Vector3d(0,30,-40), Eigen::Vector3d(0,1,0), -45, 25},

};

void switchYandZ(BodyPart *p, size_t n);

void dataSetInit()
{
    switchYandZ(parts, sizeof(parts)/sizeof(BodyPart));
}

void switchYandZ(BodyPart *p, size_t n)
{
    auto swyz = [](Eigen::Vector3d& x) {
        std::swap(x.z(), x.y());
    };
    for(size_t i = 0; i < n; ++i)
    {
        swyz(p[i].translation);
        swyz(p[i].anchor);
        swyz(p[i].axis);

    }
}



#endif