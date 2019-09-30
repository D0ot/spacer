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
           , m_anchor(anchor),m_angleAndAxis(ana),m_parent(nullptr){
           }


Node::Node(const Node& n)
{

    (*this) = n;
}

Node::~Node()
{
}


Node& Node::operator=(const Node& n)
{
    m_transRelToPar = n.m_transRelToPar;
    m_anchor = n.m_anchor;
    m_angleAndAxis = n.m_angleAndAxis;

    for(auto &node_ptr : n.m_children)
    {
        m_children.push_back(std::make_shared<Node>(*node_ptr));
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
    auto child = std::make_shared<Node>(name, trtp, anchor, ana);
    m_children.push_back(child);
    child->attachToParent(this, m_children.size()-2);
    child->m_parent = this;
    return child.get();
}


bool Node::delChild(const size_t index)
{
    if(index >= m_children.size())
    {
        return false;
    }

    size_t curSize = m_children.size();

    m_children[index] = m_children.back();

    m_children.resize(curSize - 1);

    m_children[index]->m_index = index;

}


void Node::setSourcePointSet(std::vector<Eigen::Vector3d>* points)
{
    if(!points)
    {
        m_destPoints = std::make_unique<std::vector<Eigen::Vector3d>>();
    }

    m_destPoints->resize(points->size());

    Eigen::Affine3d affine = getTransformGlobal();

    for(size_t i = 0; i < points->size(); ++i)
    {
        m_destPoints->at(i) = affine * points->at(i);
    }

}


std::vector<Eigen::Vector3d>* Node::getDestPointSet()
{
    return m_destPoints.get();
}


void Node::transform(std::vector<Eigen::Vector3d>& points)
{
    auto affine = getTransformGlobal();
    for(auto & x: points)
    {
        x = affine*x;
    }
}

Eigen::Affine3d Node::getTransformLocal()
{
    Eigen::Affine3d affine = Eigen::Affine3d::Identity();
    affine.translate(m_transRelToPar);
    affine.translate(m_anchor);
    affine.rotate(m_angleAndAxis);
    affine.translate(-m_anchor);
    return affine;
}

Eigen::Affine3d Node::getTransformGlobal()
{
    if(m_parent)
    {
        return m_parent->getTransformGlobal() * getTransformLocal();
    }
    else
    {
        return getTransformLocal();
    }
}

Node* Node::findChild(const std::string& name)
{
    auto i = std::find_if(m_children.begin(), m_children.end(),[&name](std::shared_ptr<Node> &p){
        return p->getName() == name;
    });

    if(i != m_children.end())
    {
        return (*i).get;
    } else
    {
        return nullptr;
    }
}

}







