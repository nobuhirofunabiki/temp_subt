//// in LaserLoopClosure.cc

if (!pu::Get("uwb_height_calibration", uwb_height_calibration_)) return false;
if (!pu::Get("uwb_proximity_threshold", uwb_proximity_threshold_)) return false;

// Height calibration
Pose3 last_pose = values_.at<Pose3>(pose_key);
gtsam::Point3 last_loc = last_pose.translation();
double diff_current_pose_last_node;
diff_current_pose_last_node = (cur_pose.translation.Eigen()-last_loc.vector()).norm();

auto last_observation2 = uwbid_obsnum_range_[uwb_key].end();
last_observation2--;
auto last_observation_num2 = last_observation2->first;
double range_diff = uwbid_obsnum_range_[uwb_key][last_observation_num2].first- uwbid_obsnum_range_[uwb_key][last_observation_num2-1].first;
if (range_diff > 0) {
range = sqrt(pow(sqrt(pow(range, 2)-pow(uwb_height_calibration_, 2))-pow(diff_current_pose_last_node, 2), 2)+pow(uwb_height_calibration_, 2));
} else {
range = sqrt(pow(sqrt(pow(range, 2)-pow(uwb_height_calibration_, 2))+pow(diff_current_pose_last_node, 2), 2)+pow(uwb_height_calibration_, 2));
}

// Initial estimate calculation of a UWB node position
Eigen::Matrix3d A_ini;
Eigen::Vector3d b_ini;
for (int i=0; i<3; i++){
    A_ini(i,0) = 2*(node_loc_[i+1].x()-node_loc_[0].x());
    A_ini(i,1) = 2*(node_loc_[i+1].y()-node_loc_[0].y());
    A_ini(i,2) = 2*(node_loc_[i+1].z()-node_loc_[0].z());
    b_ini(i) = pow(node_loc_[i+1].norm(),2)-pow(node_loc_[0].norm(),2)-pow(range_obs_[i+1],2)+pow(range_obs_[0],2);

}
target_est_init_ = A_ini.inverse()*b_ini;
target_est_[0] = target_est_init_;
std::cout << "Initial estimate of UWB position is " << target_est_[0] << std::endl;
// TODO: Values of these parameters should be loaded from config file
double epsilon_uwb = 1.0;
const double epsilon_uwb_threshold = 0.0001;
int counter_lsm = 0;
const int counter_lsm_threshold = 20;
Eigen::VectorXd delta_range(node_loc_.size());
Eigen::MatrixXd A(node_loc_.size(), 3);
Eigen::VectorXd delta_Xu(node_loc_.size());
// Least-square method to calculate a uwb node position
// TODO: Should this calculation be implemented as a function?
while (epsilon_uwb > epsilon_uwb_threshold && counter_lsm < counter_lsm_threshold){
    for (int i=0; i<3; i++){
    range_est_[i] = (target_est_[0]-node_loc_[i]).norm();
    delta_range(i) = range_obs_[i]-range_est_[i];
    A(i,0) = (target_est_[0].x()-node_loc_[i].x())/range_est_[i];
    A(i,1) = (target_est_[0].y()-node_loc_[i].y())/range_est_[i];
    A(i,2) = (target_est_[0].z()-node_loc_[i].z())/range_est_[i];
    }
    delta_Xu = (A.transpose()*A).inverse()*A.transpose()*delta_range;
    target_est_[0] << target_est_[0]+delta_Xu;
}
