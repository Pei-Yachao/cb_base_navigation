/*******************************
 *                             *
 *  Author: Rein Appeldoorn    *
 *  Date:   2013-03-06         *
 *                             *
 *******************************/

#ifndef cb_local_planner_LOCAL_PLANNER_INTERFACE_
#define cb_local_planner_LOCAL_PLANNER_INTERFACE_

#include <geometry_msgs/PoseStamped.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <nav_core/base_local_planner.h>
#include <actionlib/server/simple_action_server.h>

#include <cb_planner_msgs_srvs/LocalPlannerAction.h>
#include "visualization.h"

#include <head_ref/HeadReferenceAction.h>
#include <actionlib/client/action_client.h>

#define PI 3.14159265

using namespace cb_planner_msgs_srvs;

namespace cb_local_planner {

class LocalPlannerInterface {

public:

    LocalPlannerInterface(costmap_2d::Costmap2DROS& costmap);
    ~LocalPlannerInterface();

    void doSomeMotionPlanning();

private:

    //! ROS Communication
    ros::Publisher vel_pub_;
    ros::Subscriber topic_sub_;
    actionlib::SimpleActionServer<LocalPlannerAction>* action_server_;

    //! topic goal cb
    void topicGoalCallback(const LocalPlannerActionGoalConstPtr &goal);

    //! Action Server stuff
    void actionServerSetPlan();
    void actionServerPreempt();
    LocalPlannerGoal goal_;
    LocalPlannerFeedback feedback_;
    inline bool isGoalSet() { return goal_.plan.size() > 0; }

    //! Planners + loaders
    boost::shared_ptr<nav_core::BaseLocalPlanner> local_planner_;
    pluginlib::ClassLoader<nav_core::BaseLocalPlanner> lp_loader_;

    //! Frame names + Tranforms
    std::string robot_base_frame_, global_frame_;
    tf::TransformListener* tf_;
    tf::Stamped<tf::Pose> global_pose_;

    //! Costmaps
    costmap_2d::Costmap2DROS& costmap_;

    //! Helper functions
    bool updateEndGoalOrientation();

    //! Visualization
    Visualization vis;

    //! Head server
    actionlib::ActionClient<head_ref::HeadReferenceAction>* head_ref_ac_;
    actionlib::ActionClient<head_ref::HeadReferenceAction>::GoalHandle head_ref_gh_;
    void generateHeadReference(const geometry_msgs::Twist& cmd);

};

}

#endif