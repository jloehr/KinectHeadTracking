# Kinect Head Tracking
Tracks your head position via Kinect and adjusts the rendered perspective.

## Requirements

* Kinect v2 Sensor (including PC-Adapter)
* Windows 10 Version 1511 System (that meets the Kinect v2 requirements)
* DirectX 11 Feature Level graphics card
* [Microsoft Visual C++ 2015 Redistributable Update 3](https://www.microsoft.com/en-us/download/details.aspx?id=52982)

## Build Requirements

* Visual Stduio 2015 Update 3
* Windows 10 SDK 10.0.10586
* Kinect SDK v2.0

## License 

This project is licenced under the MIT license (MIT)

## Settings File

_All length units are in Centimetres_

### Monitor

* _Monitorheight_: The height of your display; used to calculate the oblique viewing .

### Kinect

* _OffsetX_, _OffsetY_, _OffsetZ_: The offset of your Kinect Sensor from your display center; used to map the real world face model to the virtual world. The face models origin is the IR camera, that is approx. 4cm to the left from the Kinect center.
