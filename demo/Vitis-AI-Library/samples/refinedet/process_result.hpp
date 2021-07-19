/*
 * Copyright 2019 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
static cv::Mat process_result(cv::Mat &image,
                              const vitis::ai::RefineDetResult &result,
                              bool is_jpeg) {
  auto img = image.clone();
  for (auto &box : result.bboxes) {
    float x = box.x * (img.cols);
    float y = box.y * (img.rows);
    int xmin = x;
    int ymin = y;
    int xmax = x + (box.width) * (img.cols);
    int ymax = y + (box.height) * (img.rows);
    float score = box.score;
    xmin = std::min(std::max(xmin, 0), img.cols);
    xmax = std::min(std::max(xmax, 0), img.cols);
    ymin = std::min(std::max(ymin, 0), img.rows);
    ymax = std::min(std::max(ymax, 0), img.rows);
    cpx[i]  = (xmax[i] + xmin[i]) / 2;
    cpy[i]  = (ymax[i] + ymin[i]) / 2;
    dim[i]    = (xmax[i] - xmin[i]) + (ymax[i] - ymin[i]);

    LOG_IF(INFO, is_jpeg) << "RESULT2: "
                          << "\txmin:" << xmin[i] << "\tymin:" << ymin[i] << "\txmax:" << xmax[i]
                          << "\tymax:" << ymax[i] << "\tcpx:" << cpx[i] << "\tcpy:" << cpy[i] << "\tdim:" << dim[i] << "\t" << i << "\t" << score << "\n";
    i++;
  }
  for (j=0;j<i;j++)
  {
    sd = 0;
    min = 0;
    for (k=0;k<i;k++)
    {
      if (k!=j){
        min = (dim[j]>dim[k])?dim[k]:dim[j];
        dis = (sqrt(pow(cpx[k] - cpx[j], 2) + pow(cpy[k] - cpy[j], 2) * 1.0)) / min;
        LOG_IF(INFO, is_jpeg) << "distance"<< "\tdis:" << dis;
        if (dis <= 0.5){
          sd++;
          break;
        }
      }
    }
    if (sd>0)
      label = 3;
    else
      label = 1;
    if (label == 1) { // Green
      cv::rectangle(img, cv::Point(xmin[j], ymin[j]), cv::Point(xmax[j], ymax[j]),
                    cv::Scalar(0, 255, 0), 1, 1, 0);
    } else if (label == 2) {  // Blue
      cv::rectangle(img, cv::Point(xmin[j], ymin[j]), cv::Point(xmax[j], ymax[j]),
                    cv::Scalar(255, 0, 0), 1, 1, 0);
    } else if (label == 3) {   // Red
      cv::rectangle(img, cv::Point(xmin[j], ymin[j]), cv::Point(xmax[j], ymax[j]),
                    cv::Scalar(0, 0, 255), 1, 1, 0);
    }    
  }
  return img;
}
