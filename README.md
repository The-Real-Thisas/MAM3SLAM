# MAM3SLAM

MAM3SLAM is a new fully centralized multi-agent and multi-map monocular Visual Simultaneous Localization And Mapping (VSLAM) framework based on ORB-SLAM3. 

## License

ORB-SLAM3 is released under [GPLv3 license](https://github.com/UZ-SLAMLab/MAM3SLAM/LICENSE). For a list of all code/library dependencies (and associated licenses), please see [Dependencies.md](https://github.com/UZ-SLAMLab/MAM3SLAM/blob/master/Dependencies.md).

For a closed-source version of ORB-SLAM3 for commercial purposes, please contact the authors: orbslam (at) unizar (dot) es.

If you use ORB-SLAM3 in an academic work, please cite:
  
    @article{ORBSLAM3_TRO,
      title={{ORB-SLAM3}: An Accurate Open-Source Library for Visual, Visual-Inertial 
               and Multi-Map {SLAM}},
      author={Campos, Carlos AND Elvira, Richard AND G\´omez, Juan J. AND Montiel, 
              Jos\'e M. M. AND Tard\'os, Juan D.},
      journal={IEEE Transactions on Robotics}, 
      volume={37},
      number={6},
      pages={1874-1890},
      year={2021}
     }

# 2 - MAM3SLAM

If you use MAM3SLAM in an academic work, please cite:
  
    @article{ORBSLAM3_TRO,
      title={{ORB-SLAM3}: An Accurate Open-Source Library for Visual, Visual-Inertial 
               and Multi-Map {SLAM}},
      author={Campos, Carlos AND Elvira, Richard AND G\´omez, Juan J. AND Montiel, 
              Jos\'e M. M. AND Tard\'os, Juan D.},
      journal={IEEE Transactions on Robotics}, 
      volume={37},
      number={6},
      pages={1874-1890},
      year={2021}
     }

     @article{MAM,
      
      title={{MAM$^3$SLAM}: Towards underwater-robust multi-agent visual {SLAM}},
      author={Drupt, Juliette AND Comport, Andrew I. AND Dune, Claire AND Hugel, Vincent},
      journal={Ocean Engineering}, 
      year={2024}
     }


## Building MAM3SLAM

There is a docker image available to build MAM3SLAM. You can use it to build the project and run the examples. The image is based on Ubuntu 22.04 and includes all dependencies needed to run MAM3SLAM.

Clone the repository:
```
git clone --recursive https://github.com/The-Real-Thisas/MAM3SLAM
```

Make sure you are in the *MAM3SLAM* folder:
```
cd MAM3SLAM
```

Build the docker image:
```
sudo ./docker_build.sh
```

Run the docker container:
```
sudo ./docker_run.sh
```

This will open a shell in the container. 

You will first need to build the dependencies. The docker image includes the *Thirdparty* folder with all the dependencies needed to run MAM3SLAM along with a specific version of Pangolin which is confirmed to work. You can build them by running the following command in the docker image:

```
./build_deps.sh
```

This will build all the dependencies in the *Thirdparty* folder.

Then you can build MAM3SLAM. 

```
./build.sh
```

On changes to the code in the `src` folder you do not need to rebuild the dependencies. Just rerun the build command. 