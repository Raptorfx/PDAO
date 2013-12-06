PDAO - Project Dark Ages Online
====
How to build with Visual Studio 2013 (msvc110):

Download Dependencies, Boost, Ogre and Addon from
http://www.ogre3d.org/forums/viewtopic.php?t=69274

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
