PDAO - Project Dark Ages Online
====
How to build with Visual Studio 2013 (msvc110):

Download from
http://www.ogre3d.org/forums/viewtopic.php?t=69274
boost-1.55-rev86545-vc110-x64-02.11.2013
Dependencies-vc110-x64-02.11.2013
OGRE-SDK-1.9.0-vc110-x64-02.11.2013
plugins-vc110-x64-02.11.2013

extract them all within the deps folder

Media folder can be downloaded from
http://project-dao.at.vu/media.rar

Create a folder structure like this:
- Project
- - build
- - deps
- - media
- - PDAO (<- git clone)

Use CMake within PDAO and specify the build directory as output.
Copy the dll's from Ogre RelWithDebInfo to the build folder.
Copy media folder to build folder
Copy config folder from PDAO to build folder
In VS select as build RelWithDebInfo
