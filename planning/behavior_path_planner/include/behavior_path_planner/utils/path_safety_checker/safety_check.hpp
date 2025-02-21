// Copyright 2023 TIER IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BEHAVIOR_PATH_PLANNER__UTILS__PATH_SAFETY_CHECKER__SAFETY_CHECK_HPP_
#define BEHAVIOR_PATH_PLANNER__UTILS__PATH_SAFETY_CHECKER__SAFETY_CHECK_HPP_

#include "behavior_path_planner/data_manager.hpp"
#include "behavior_path_planner/marker_utils/utils.hpp"
#include "behavior_path_planner/utils/path_safety_checker/path_safety_checker_parameters.hpp"

#include <tier4_autoware_utils/geometry/boost_geometry.hpp>
#include <tier4_autoware_utils/geometry/geometry.hpp>

#include <autoware_auto_perception_msgs/msg/predicted_object.hpp>
#include <autoware_auto_perception_msgs/msg/predicted_path.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>

#include <tf2/utils.h>

#ifdef ROS_DISTRO_GALACTIC
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#else
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#endif

#include <utility>
#include <vector>

namespace behavior_path_planner::utils::path_safety_checker
{

using autoware_auto_perception_msgs::msg::PredictedObject;
using autoware_auto_perception_msgs::msg::PredictedPath;
using autoware_auto_perception_msgs::msg::Shape;
using behavior_path_planner::utils::path_safety_checker::CollisionCheckDebug;
using geometry_msgs::msg::Pose;
using geometry_msgs::msg::Twist;
using tier4_autoware_utils::calcYawDeviation;
using tier4_autoware_utils::Point2d;
using tier4_autoware_utils::Polygon2d;
using vehicle_info_util::VehicleInfo;

namespace bg = boost::geometry;

bool isTargetObjectOncoming(
  const geometry_msgs::msg::Pose & vehicle_pose, const geometry_msgs::msg::Pose & object_pose);

bool isTargetObjectFront(
  const geometry_msgs::msg::Pose & ego_pose, const Polygon2d & obj_polygon,
  const vehicle_info_util::VehicleInfo & vehicle_info);
bool isTargetObjectFront(
  const PathWithLaneId & path, const geometry_msgs::msg::Pose & ego_pose,
  const vehicle_info_util::VehicleInfo & vehicle_info, const Polygon2d & obj_polygon);

Polygon2d createExtendedPolygon(
  const Pose & base_link_pose, const vehicle_info_util::VehicleInfo & vehicle_info,
  const double lon_length, const double lat_margin, CollisionCheckDebug & debug);
Polygon2d createExtendedPolygon(
  const Pose & obj_pose, const Shape & shape, const double lon_length, const double lat_margin,
  CollisionCheckDebug & debug);

PredictedPath convertToPredictedPath(
  const std::vector<PoseWithVelocityStamped> & path, const double time_resolution);

double calcRssDistance(
  const double front_object_velocity, const double rear_object_velocity,
  const RSSparams & rss_params);

double calcMinimumLongitudinalLength(
  const double front_object_velocity, const double rear_object_velocity,
  const BehaviorPathPlannerParameters & params);

boost::optional<PoseWithVelocityStamped> calcInterpolatedPoseWithVelocity(
  const std::vector<PoseWithVelocityStamped> & path, const double relative_time);

boost::optional<PoseWithVelocityAndPolygonStamped> getInterpolatedPoseWithVelocityAndPolygonStamped(
  const std::vector<PoseWithVelocityStamped> & pred_path, const double current_time,
  const VehicleInfo & ego_info);
/**
 * @brief Iterate the points in the ego and target's predicted path and
 *        perform safety check for each of the iterated points.
 * @param planned_path The predicted path of the ego vehicle.
 * @param predicted_ego_path Ego vehicle's predicted path
 * @param ego_current_velocity Current velocity of the ego vehicle.
 * @param target_object The predicted object to check collision with.
 * @param target_object_path The predicted path of the target object.
 * @param common_parameters The common parameters used in behavior path planner.
 * @param front_object_deceleration The deceleration of the object in the front.(used in RSS)
 * @param rear_object_deceleration The deceleration of the object in the rear.(used in RSS)
 * @param debug The debug information for collision checking.
 * @return true if distance is safe.
 */
bool checkCollision(
  const PathWithLaneId & planned_path,
  const std::vector<PoseWithVelocityStamped> & predicted_ego_path,
  const ExtendedPredictedObject & target_object,
  const PredictedPathWithPolygon & target_object_path,
  const BehaviorPathPlannerParameters & common_parameters, const RSSparams & rss_parameters,
  const double hysteresis_factor, CollisionCheckDebug & debug);

/**
 * @brief Iterate the points in the ego and target's predicted path and
 *        perform safety check for each of the iterated points.
 * @param planned_path The predicted path of the ego vehicle.
 * @param predicted_ego_path Ego vehicle's predicted path
 * @param ego_current_velocity Current velocity of the ego vehicle.
 * @param target_object The predicted object to check collision with.
 * @param target_object_path The predicted path of the target object.
 * @param common_parameters The common parameters used in behavior path planner.
 * @param front_object_deceleration The deceleration of the object in the front.(used in RSS)
 * @param rear_object_deceleration The deceleration of the object in the rear.(used in RSS)
 * @param debug The debug information for collision checking.
 * @return a list of polygon when collision is expected.
 */
std::vector<Polygon2d> getCollidedPolygons(
  const PathWithLaneId & planned_path,
  const std::vector<PoseWithVelocityStamped> & predicted_ego_path,
  const ExtendedPredictedObject & target_object,
  const PredictedPathWithPolygon & target_object_path,
  const BehaviorPathPlannerParameters & common_parameters, const RSSparams & rss_parameters,
  const double hysteresis_factor, CollisionCheckDebug & debug);

/**
 * @brief Check collision between ego path footprints with extra longitudinal stopping margin and
 * objects.
 * @return Has collision or not
 */
bool checkCollisionWithExtraStoppingMargin(
  const PathWithLaneId & ego_path, const PredictedObjects & dynamic_objects,
  const double base_to_front, const double base_to_rear, const double width,
  const double maximum_deceleration, const double margin, const double max_stopping_margin);
}  // namespace behavior_path_planner::utils::path_safety_checker

#endif  // BEHAVIOR_PATH_PLANNER__UTILS__PATH_SAFETY_CHECKER__SAFETY_CHECK_HPP_
