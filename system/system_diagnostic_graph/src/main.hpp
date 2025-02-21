// Copyright 2023 The Autoware Contributors
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

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include "core/types.hpp"
#include "core/update.hpp"

#include <rclcpp/rclcpp.hpp>

namespace system_diagnostic_graph
{

class MainNode : public rclcpp::Node
{
public:
  MainNode();

private:
  DiagGraph graph_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Subscription<DiagnosticArray>::SharedPtr sub_input_;
  rclcpp::Publisher<DiagnosticGraph>::SharedPtr pub_graph_;
  rclcpp::Publisher<OperationModeAvailability>::SharedPtr pub_modes_;
  void on_timer();
  void on_diag(const DiagnosticArray::ConstSharedPtr msg);
};

}  // namespace system_diagnostic_graph

#endif  // MAIN_HPP_
