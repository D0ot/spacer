/*
 * @file : spacer.h
 * @author : d0ot at github.com
 *      spacer header
 */

#include <iostream>
#include <string>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <memory>
#include <map>

namespace Spacer
{

class Node;
class Node
{
  private:

    // name of the node
    std::string m_name;

    // index in parent's vector
    size_t m_index;

    // pointer to parent's pointer
    Node* m_parent;

    // central point translation related to parent
    Eigen::Vector3d m_transRelToPar;

    // the anchor point to parent
    // translation related to central with central is (0,0,0)
    Eigen::Vector3d m_anchor;

    // Rotate angle and axis
    Eigen::AngleAxisd m_angleAndAxis;

    // children nodes
    std::vector<std::shared_ptr<Node>> m_children;

    // Points related to center
    std::vector<Eigen::Vector3d> *m_points;

    // dest points
    std::unique_ptr<std::vector<Eigen::Vector3d>> m_destPoints;

  public:

    Node(const std::string& name, const Eigen::Vector3d& trtp,
         const Eigen::Vector3d& anchor, const Eigen::AngleAxisd& ana);
    Node(const Node& n);
    ~Node();
    Node(Node &&n) = delete;
    Node& operator=(const Node& n);
    Node& operator=(Node&& n) = delete;

    bool attachToParent(Node* p, const size_t index);
    bool detachFromParent();

    Node* addChild(const std::string& name, const Eigen::Vector3d& trtp, 
                   const Eigen::Vector3d& anchor, const Eigen::AngleAxisd& ana);

    // maybe
    // will be removed 
    bool delChild(const size_t index);

    // the points are all related value
    // related to central point, with central point is (0,0,0)
    // transform in place. remember to backup data.
    void transform(std::vector<Eigen::Vector3d>& points);

    Eigen::Affine3d getTransformLocal();
    Eigen::Affine3d getTransformGlobal();

    // find the child named "name"
    // it's performance is low
    Node* findChild(const std::string& name);


    // set points set
    // source
    void setSourcePointSet(std::vector<Eigen::Vector3d>* points);

    // get points set
    // transformed
    std::vector<Eigen::Vector3d>* getDestPointSet(void);



    const std::string& getName()const {return m_name;};

};



}
