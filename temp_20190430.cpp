//// BlamSlam.h

// UWB ID
std::unordered_map<std::string, gtsam::Key> uwb_id2key_hash;

int largest_uwb_id_;

//// BlamSlam.cc

std::string uwb_id = msg.id;
gtsam::Key cur_uwb_key;
if (uwb_id2key_hash.find(uwb_id)) != uwb_id2key_hash.end()){
    cur_uwb_key = uwb_id2key_hash[uwb_id];   
} else {
    ++largest_uwb_id_;
    cur_uwb_key = gtsam::Symbol('u', largest_uwb_id_);
    uwb_id2key_hash[uwb_id] = cur_artifact_key;
}

if (uwb_key2label_hash.find(uwb))
loop_closure_.AddFactor(    


//// LaserLoopClosure.h

ros::Publisher uwb_node_pub_;


//// LaserLoopClosure.cc

uwb_node_pub_ = nl.advertise<visualization_msgs::Marker>("uwb_markers", 10);

void LaserLoopClosure::PublishUwb(){ // for publishing UWB node
    // Publish edges between UWB and a pose node
    for (auto i = uwb_id2key_hash.begin(); i != uwb_id2key_hash.end(); i++){
        visualization_msgs::Marker m;
        hogehoge~~~;
        m.pose.position = 
        m.pose.orientation.x = 0.0;
        m.pose.orientation.y = 0.0;
        m.pose.orientation.z = 0.0;
        m.scale.x = 0.5f;
        m.scale.y = 0.5f;
        m.scale.z = 0.5f;
        m.color.r = 0.0f;
        m.color.g = 1.0f;
        m.color.b = 0.0f;
        m.type = visualization_msgs::Marker::CUBE;

        uwb_node_pub_.publish(m);
    }
}

Eigen::Vector3d LaserLoopClosure::GetUwbPosition(cosnt gtsam::Key uwb_key){
    return values_.at<Pose3>(uwb_key).translation().vector();
}



//// lidar_slam_husky.rviz

- Class: rviz/Marker
      Enabled: true
      Marker Topic: /husky/blam_slam/uwb_markers
      Name: UWB_nodes
      Namespaces:
        {}
      Queue Size: 100
      Value: true