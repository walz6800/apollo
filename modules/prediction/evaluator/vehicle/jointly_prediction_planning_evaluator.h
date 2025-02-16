/******************************************************************************
 * Copyright 2021 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "modules/prediction/evaluator/evaluator.h"
#include "torch/extension.h"
#include "torch/script.h"
#include "modules/prediction/container/container_manager.h"
#include "modules/prediction/pipeline/vector_net.h"

namespace apollo {
namespace prediction {

using apollo::common::TrajectoryPoint;

class JointlyPredictionPlanningEvaluator : public Evaluator {
 public:
  /**
   * @brief Constructor
   */
  JointlyPredictionPlanningEvaluator();

  /**
   * @brief Destructor
   */
  virtual ~JointlyPredictionPlanningEvaluator() = default;

  /**
   * @brief Clear obstacle feature map
   */
  void Clear();

  /**
   * @brief Override Evaluate
   * @param Obstacle pointer
   * @param Obstacles container
   */
  bool Evaluate(Obstacle* obstacle_ptr,
                ObstaclesContainer* obstacles_container) override;


  /**
   * @brief Override Evaluate
   * @param ADC trajectory container
   * @param Obstacle pointer
   * @param Obstacles container
   */
  bool Evaluate(const ADCTrajectoryContainer* adc_trajectory_container,
                Obstacle* obstacle_ptr,
                ObstaclesContainer* obstacles_container) override;

  /**
   * @brief Extract all obstacles history
   * @param Obstacles container
   *        Feature container in a vector for receiving the obstacle history
   */
  bool ExtractObstaclesHistory(
      Obstacle* obstacle_ptr, ObstaclesContainer* obstacles_container,
      std::vector<std::pair<double, double>>* curr_pos_history,
      std::vector<std::pair<double, double>>* all_obs_length,
      std::vector<std::vector<std::pair<double, double>>>* all_obs_pos_history);

  /**
   * @brief Extract adc trajectory and convert world coord to obstacle coord
   * @param Obstacle pointer
   *        Feature container in a vector for receiving the obstacle history
   */
  bool ExtractADCTrajectory(
      std::vector<TrajectoryPoint>* trajectory_points,
      Obstacle* obstacle_ptr,
      std::vector<std::pair<double, double>>* acd_traj_curr_pos);

  /**
   * @brief Get the name of evaluator.
   */
  std::string GetName() override {
    return "JOINTLY_PREDICTION_PLANNING_EVALUATOR";
  }

 private:
  /**
   * @brief Load model file
   */
  void LoadModel();

 private:
  torch::jit::script::Module torch_vehicle_model_;
  at::Tensor torch_default_output_tensor_;
  torch::Device device_;
  VectorNet vector_net_;
};

}  // namespace prediction
}  // namespace apollo
