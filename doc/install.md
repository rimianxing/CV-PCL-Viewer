## Installation tutorial

All required installation packages are provided in this link: [CV-PCL Viewer packages](https://pan.baidu.com/s/1FnxM5cQXYQgbCaKPJD-A3A?pwd=676n).

### 1. Visual Studio 2019 Community

Double click the **Visual Studio 2019 Community_16.4.29519.181.exe**, then click the continue button.
<div align=center>
<img src="../img_readme/visualstudio1.jpg" width="300px">
</div>

Select the component that need to be installed as shown in the figure, and change the installation path, then click the install button.
<div align=center>
<img src="../img_readme/visualstudio2.jpg" width="800px">
</div>

Log in to your Visual Studio account.
<div align=center>
<img src="../img_readme/visualstudio3.jpg" width="300px">
</div>

### 2. Qt 5.15.2

Double click the **qt-unified-windows-x86-4.3.0-1-online.exe**, then log in to your Qt account and click the next step button.
<div align=center>
<img src="../img_readme/qt1.jpg" width="800px">
</div>

Check the box then click the next step button.
<div align=center>
<img src="../img_readme/qt2.jpg" width="800px">
</div>

Check the box then click the next step button.
<div align=center>
<img src="../img_readme/qt3.jpg" width="800px">
</div>

Change the installation path then click the next step button.
<div align=center>
<img src="../img_readme/qt4.jpg" width="800px">
</div>

Filter out old versions.
<div align=center>
<img src="../img_readme/qt5.jpg" width="800px">
</div>

Check the **MSVC 2019 64-bit** box and **MinGW 8.1.0 64-bit** box, maintain default for others, then click the next step button.
<div align=center>
<img src="../img_readme/qt6.jpg" width="800px">
</div>

Check the box then click the next step button.
<div align=center>
<img src="../img_readme/qt7.jpg" width="800px">
</div>

Click the next step button.
<div align=center>
<img src="../img_readme/qt8.jpg" width="800px">
</div>

Click the install button, wait for some time until the installation is complete.
<div align=center>
<img src="../img_readme/qt9.jpg" width="800px">
</div>

### 3. Cmake 3.30.0

Double click the **cmake-3.30.0-rc4-windows-x86_64.msi** to install.

### 4. OpenCV 4.5.3

Double click the **opencv-4.5.3-vc14_vc15.exe**, extract to the path as shown in the figure. 
<div align=center>
<img src="../img_readme/opencv1.jpg" width="300px">
</div>

<div align=center>
<img src="../img_readme/opencv2.jpg" width="800px">
</div>

Extract **opencv_contrib-4.5.3.zip** to the path as shown in the figure.
<div align=center>
<img src="../img_readme/opencv3.jpg" width="400px">
</div>

Create a new folder named msvc-build.
<div align=center>
<img src="../img_readme/opencv4.jpg" width="800px">
</div>

Open Cmake (cmake-gui), choose the paths, then click the Configure button.
<div align=center>
<img src="../img_readme/opencv5.jpg" width="400px">
</div>

Check the configure then click the finish button.
<div align=center>
<img src="../img_readme/opencv6.jpg" width="400px">
</div>

There will be some red error messages.
<div align=center>
<img src="../img_readme/opencv7.jpg" width="400px">
</div>

Search opencv_extra, change the value.
<div align=center>
<img src="../img_readme/opencv8.jpg" width="400px">
</div>

Search nonfree, check the box.
<div align=center>
<img src="../img_readme/opencv9.jpg" width="400px">
</div>

Search world, check the box.
<div align=center>
<img src="../img_readme/opencv10.jpg" width="400px">
</div>

There will be no more red error messages after clicking the Configure button **twice**, then click the Generate button.
<div align=center>
<img src="../img_readme/opencv11.jpg" width="400px">
</div>

Click the Open Project button.
<div align=center>
<img src="../img_readme/opencv12.jpg" width="400px">
</div>

Click the batch generation button.
<div align=center>
<img src="../img_readme/opencv13.jpg" width="800px">
</div>

Check the box then click the generate button.
<div align=center>
<img src="../img_readme/opencv14.jpg" width="400px">
</div>

<div align=center>
<img src="../img_readme/opencv15.jpg" width="400px">
</div>

Wait for some time until the generation is complete.
<div align=center>
<img src="../img_readme/opencv16.jpg" width="800px">
</div>

### 5. PCL 1.12.1

Double click the **PCL-1.12.1-AllInOne-msvc2019-win64.exe**, then click the next step button.
<div align=center>
<img src="../img_readme/pcl1.jpg" width="400px">
</div>

Click the I agree button.
<div align=center>
<img src="../img_readme/pcl2.jpg" width="400px">
</div>

Check the box then click the next step button.
<div align=center>
<img src="../img_readme/pcl3.jpg" width="400px">
</div>

Change the installation path then click the next step button. **Please note that there is no space in the path here.**
<div align=center>
<img src="../img_readme/pcl4.jpg" width="400px">
</div>

Click the next step button.
<div align=center>
<img src="../img_readme/pcl5.jpg" width="400px">
</div>

Click the install button.
<div align=center>
<img src="../img_readme/pcl6.jpg" width="400px">
</div>

Wait for some time until the installation is complete, then click the finish button.
<div align=center>
<img src="../img_readme/pcl7.jpg" width="400px">
</div>

Double click the **OpenNI-Windows-x64-2.2.msi** in the path as shown in the figure.
<div align=center>
<img src="../img_readme/pcl8.jpg" width="800px">
</div>

Click the Next button.
<div align=center>
<img src="../img_readme/pcl9.jpg" width="400px">
</div>

Click the Remove button.
<div align=center>
<img src="../img_readme/pcl10.jpg" width="400px">
</div>

Click the Remove button.
<div align=center>
<img src="../img_readme/pcl11.jpg" width="400px">
</div>

Wait for some time until the removal is complete, then click the finish button.
<div align=center>
<img src="../img_readme/pcl12.jpg" width="400px">
</div>

Double click the **OpenNI-Windows-x64-2.2.msi** again, change the installation path, then click the Install button.
<div align=center>
<img src="../img_readme/pcl13.jpg" width="400px">
</div>

Wait for some time until the installation is complete, then click the finish button.
<div align=center>
<img src="../img_readme/pcl14.jpg" width="400px">
</div>

Open **pcl-1.12.1-pdb-msvc2019-win64.zip**, select all these pdb files and extract them to the path as shown in the figure.
<div align=center>
<img src="../img_readme/pcl15.jpg" width="400px">
</div>

Copy the **pcl1.12.1.pri** to the path as shown in the figure.
<div align=center>
<img src="../img_readme/pcl16.jpg" width="800px">
</div>

### 6. VTK 9.1.0

Delete the VTK folder in the path as shown in the figure.
<div align=center>
<img src="../img_readme/vtk1.jpg" width="800px">
</div>

Extract **VTK-9.1.0.rar** to the path as shown in the figure.
<div align=center>
<img src="../img_readme/vtk2.jpg" width="400px">
</div>

### 7. Configure environment variables

Click the Advanced System Settings button.
<div align=center>
<img src="../img_readme/environment1.jpg" width="800px">
</div>

Click the environment variables button.
<div align=center>
<img src="../img_readme/environment2.jpg" width="300px">
</div>

Choose the Path line, then click the edit button.
<div align=center>
<img src="../img_readme/environment3.jpg" width="400px">
</div>

Create some new environment variables as follows, then click the confirm button in sequence.
<div align=center>
<img src="../img_readme/environment4.jpg" width="400px">
</div>
