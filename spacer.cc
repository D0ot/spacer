/**
 * @file : spacer.cc
 * @author : d0ot at github.com
 * @date : 2019/9/28
 *      calculate position in space by model
 */

#include <iostream>
#include <eigen3/Eigen/Dense>
#include "spacer.h"

namespace Spacer
{


Node::Node(const std::string& name, const Eigen::Vector3d& trtp,
           const Eigen::Vector3d& anchor, const Eigen::AngleAxisd& ana)
           : m_name(name), m_transRelToPar(trtp)
           , m_anchor(anchor),m_angleAndAxis(ana){}


Node::Node(const Node& n)
{
    (*this) = n;
}


Node& Node::operator=(const Node& n)
{
    m_transRelToPar = n.m_transRelToPar;
    m_anchor = n.m_anchor;
    m_angleAndAxis = n.m_angleAndAxis;

    for(auto &node_ptr : n.m_children)
    {
        m_children.emplace_back(*node_ptr);
    }
    m_points = n.m_points;
}

// should only be call from parent
bool Node::attachToParent(Node* p, const size_t index)
{
    if(!p)
    {
        // p is not a valid pointer
        return false;
    }

    m_index = index;
    m_parent = p;

}

// sould only be call from parent
bool Node::detachFromParent()
{
    m_parent = nullptr;
}



Node* Node::addChild(const std::string& name, const Eigen::Vector3d& trtp, 
               const Eigen::Vector3d& anchor, const Eigen::AngleAxisd& ana)
{
    std::shared_ptr<Node> child = std::make_shared<Node>(name, trtp, anchor, ana);
    m_children.push_back(child);
    child->attachToParent()

}



           

}







