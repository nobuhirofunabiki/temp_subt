//// in BlamSlam.h

std::map<std::string, unsigned int> uwb_updatenumber_;

//// in BlamSlam.cc
// in ProcessUwbRangeData

ros::Time aug_time = minItr->second;
double aug_range = minItr->first;
Eigen::Vector3d aug_robot_position = map_uwbid_time_data_[uwb_id][aug_time].second;
std::map<ros::Time, std::pair<double, Eigen::Vector3d>> map_time_range_position;
map_time_range_position[aug_time].first = aug_range;
map_time_range_position[aug_time].second = aug_robot_position;

auto itr = uwb_updatenumber_.find(uwb_id);
if (itr != uwb_updatenumber_.end()) {
itr->second +=1;
}
else {
// Select two more range measurement 
// TODO: A more sophisticated method should be implemented.
int num_measurement = map_uwbid_time_data_[uwb_id].size();
int counter = 0;
int counter1 = num_measurement/5;
int counter2 = num_measurement/5*4;

for (auto itr = map_uwbid_time_data_[uwb_id].begin(); itr != map_uwbid_time_data_[uwb_id].end(); itr++) {
    counter++;
    if (counter == counter1 || counter == counter2) {
    ros::Time aug_time = itr->first;
    double aug_range = (itr->second).first;
    Eigen::Vector3d aug_robot_position = (itr->second).second;
    map_time_range_position[aug_time].first = aug_range;
    map_time_range_position[aug_time].second = aug_robot_position;
    }
}
uwb_updatenumber_[uwb_id] = 1;
}

if (loop_closure_.AddUwbFactor(uwb_id, map_time_range_position)) {


//// in LaserLoopClosure.h

bool AddUwbFactor(const std::string uwb_id,
                const std::map<ros::Time, std::pair<double, Eigen::Vector3d>> data);

//// in LaserLoopClosure.cc

bool LaserLoopClosure::AddUwbFactor(const std::string uwb_id, 
                                    const std::map<ros::Time, std::pair<double, Eigen::Vector3d>> data) {


