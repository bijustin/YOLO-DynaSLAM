# Dynamic Yolov3 SLAM

Dynamic Yolov3 SLAM is a visual SLAM system that is robust in dynamic scenarios for RGB-D configuration.

We provide one example to run the SLAM system in the [TUM dataset](http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets) as RGB-D.

## Getting Started
- Install ORB-SLAM2 prerequisites: C++11 or C++0x Compiler, Pangolin, OpenCV and Eigen3  (https://github.com/raulmur/ORB_SLAM2).
- Install boost libraries with the command `sudo apt-get install libboost-all-dev`.
- Install python 2.7, keras and tensorflow, and download the `yolov3.weights` model from this address: https://pjreddie.com/media/files/yolov3.weights. 
- Clone this repo:
```bash
git clone https://github.com/bijustin/Flow-DynaSLAM.git
cd Flow-DynaSLAM
```
```
cd Flow-DynaSLAM
chmod +x build.sh
./build.sh
```
- Place the `yolov3.weights` model in the folder `Flow-DynaSLAM/src/yolo/`.

## RGB-D Example on TUM Dataset
- Download a sequence from http://vision.in.tum.de/data/datasets/rgbd-dataset/download and uncompress it.

- Associate RGB images and depth images executing the python script [associate.py](http://vision.in.tum.de/data/datasets/rgbd-dataset/tools):

  ```
  python associate.py PATH_TO_SEQUENCE/rgb.txt PATH_TO_SEQUENCE/depth.txt > associations.txt
  ```
These associations files are given in the folder `./Examples/RGB-D/associations/` for the TUM dynamic sequences.

- Execute the following command. Change `TUMX.yaml` to TUM1.yaml,TUM2.yaml or TUM3.yaml for freiburg1, freiburg2 and freiburg3 sequences respectively. Change `PATH_TO_SEQUENCE_FOLDER` to the uncompressed sequence folder. Change `ASSOCIATIONS_FILE` to the path to the corresponding associations file. `YOLO`is an optional parameter.

  ```
  ./Examples/RGB-D/rgbd_tum_yolo Vocabulary/ORBvoc.txt Examples/RGB-D/TUMX.yaml PATH_TO_SEQUENCE_FOLDER ASSOCIATIONS_FILE (YOLO)
  ```
  
If `YOLO` is **not** provided, only the geometrical approach is used to detect dynamic objects. 

If `YOLO` is provided, Yolov3 is used to segment the potential dynamic content of every frame. 

## Acknowledgements
Our code builds on [ORB-SLAM2] and [DynaSLAM](https://github.com/raulmur/ORB_SLAM2, https://github.com/BertaBescos/DynaSLAM).

# Dynamic Yolov3 SLAM
