## Project information
**WebCamCap** is tool for 2D/3D motion capture with active markers (LED diodes) written in C++11. It doesn't require any special hardware, just common webcam with VGA (640 x 480) resolution and diffused white LED diodes for best results.

## Features
- 2D Motion Capture
- 3D Motion Capture
- Real time motion capture data transfer (API provided)
- Virtual scene preview (OpenGL)
- export animation to .fbx format

## Requirements
- Qt 5.9
- OpenCV 3.0
- OpenGL
- GLU
- FBX SDK

## Docker
There are 2 docker-compose files. App for running **WebCamCap** and Devel with Qt Creator included. Use command ``` docker-compose up ``` in directory with docker-compose.yml file.

## Videos
For all videos check my WebCamCap [Playlist](https://www.youtube.com/playlist?list=PL06R3YeyHsEbsVQlwZ6-BCoW4DKWrXiek)
