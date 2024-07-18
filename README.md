## CV-PCL Viewer
### 1. Software introduction
Welcome to CV-PCL Viewer! This software has simple image and video processing functions, as well as the ability to visualize point cloud and generate mesh from point cloud.

- **Image processing functions:** grayscale, mean filtering, edge detection, edge with original image, gamma transformation, turn left, turn right, horizontal flip, vertical flip, binarization, brightness adjustment, contrast adjustment, saturation adjustment, highlight adjustment, shadow adjustment, warm tone adjustment and watermark.

- **Video processing functions:** mosaic, scaling, binarization, grayscale, edge detection and smooth.

- **Point cloud processing functions:** change point size, point cloud display, mesh surface display, mesh wire display, point cloud and mesh surface overlay and generate mesh from point cloud.

### 2. Development environment
- Windows 11
- Qt 5.15.2
- Qt Creator 13.0.2
- Cmake 3.30.0
- OpenCV 4.5.3
- PCL 1.12.1
- VTK 9.1.0

You can refer to this article for the installation of the environment: [Installation tutorial (Chinese)](doc/install-zh.md) or [Installation tutorial (English)](doc/install-en.md).

>**Note:** when you run this software and find that the video cannot be opened, place these two files (**opencv_ffmpeg.dll** and **opencv_ffmpeg_64.dll**) in the **reference** folder to the OpenCV build folder, such as mine: "D:\opencv-4.0.1\build\3rdparty\ffmpeg", then recompile OpenCV, after that it will work.

### 3. Interface display
The **Show toolbox** button on the top toolbar is used to redisplay the **Toolbox** and **Adjust** side toolbars. The **Switch language** button is used to switch between Chinese and English. Clicking the **About** button will display some information about this software.

<div align=center>
<img src="img_readme/image_display.jpg" width="800px">
</div>

<p align="left">Figure 1. Chinese image processing interface</p>

<div align=center>
<img src="img_readme/video_display.jpg" width="800px">
</div>

<p align="left">Figure 2. Chinese video processing interface</p>

<div align=center>
<img src="img_readme/pointcloud_display.jpg" width="800px">
</div>

<p align="left">Figure 3. Chinese point cloud processing interface</p>

Switch language to English:

<div align=center>
<img src="img_readme/image_display2.jpg" width="800px">
</div>

<p align="left">Figure 4. English image processing interface</p>

<div align=center>
<img src="img_readme/video_display2.jpg" width="800px">
</div>

<p align="left">Figure 5. English video processing interface</p>

<div align=center>
<img src="img_readme/pointcloud_display2.jpg" width="800px">
</div>

<p align="left">Figure 6. English point cloud processing interface</p>

### 4. Image processing

Some images are provided in the **show** folder for testing, and you can open one or several at the same time. The image formats that support opening and saving include **jpg, jpeg, png, bmp, tif**.

When you only open one image, both the previous and next buttons are invalid. When you open two or more images, click the previous or next button to switch to the previous or next image.

<div align=center>
<img src="img_readme/original_image.jpg" width="800px">
</div>

<p align="left">Figure 7. Original image</p>

<div align=center>
<img src="img_readme/grayscale_image.jpg" width="800px">
</div>

<p align="left">Figure 8. Grayscale</p>

<div align=center>
<img src="img_readme/mean_filtering.jpg" width="800px">
</div>

<p align="left">Figure 9. Mean filtering</p>

<div align=center>
<img src="img_readme/edge_detection_image.jpg" width="800px">
</div>

<p align="left">Figure 10. Edge detection</p>

<div align=center>
<img src="img_readme/edge_with_original_image.jpg" width="800px">
</div>

<div align=center>
<img src="img_readme/edge_with_original_image2.jpg" width="800px">
</div>

<div align=center>
<img src="img_readme/edge_with_original_image3.jpg" width="800px">
</div>

<p align="left">Figure 11. Edge with original image (rate: 0.5:0.5, 0.8:0.2, 0.2:0.8)</p>

<div align=center>
<img src="img_readme/gamma_transformation.jpg" width="800px">
</div>

<p align="left">Figure 12. Gamma transformation</p>

<div align=center>
<img src="img_readme/binarization_image.jpg" width="800px">
</div>

<p align="left">Figure 13. Binarization (threshold = 88, value range: 0~255)</p>

>**Note:** the above operations were performed on the original image, but the following adjustments were made based on the previous processing.

<div align=center>
<img src="img_readme/brightness_adjustment.jpg" width="800px">
</div>

<p align="left">Figure 14. Brightness adjustment (value = -60, value range: -100~100)</p>

<div align=center>
<img src="img_readme/contrast_adjustment.jpg" width="800px">
</div>

<p align="left">Figure 15. Contrast adjustment (value = 0.7, value range: 0.2~5)</p>

<div align=center>
<img src="img_readme/saturation_adjustment.jpg" width="800px">
</div>

<p align="left">Figure 16. Saturation adjustment (value = -30, value range: -100~100)</p>

<div align=center>
<img src="img_readme/highlight_adjustment.jpg" width="800px">
</div>

<p align="left">Figure 17. Highlight adjustment (value = 30, value range: -100~100)</p>

<div align=center>
<img src="img_readme/shadow_adjustment.jpg" width="800px">
</div>

<p align="left">Figure 18. Shadow adjustment (value = -30, value range: -100~100)</p>

<div align=center>
<img src="img_readme/warm_tone_adjustment.jpg" width="800px">
</div>

<p align="left">Figure 19. Warm tone adjustment (value = 10, value range: -50~50)</p>

<div align=center>
<img src="img_readme/turn_left.jpg" width="800px">
</div>

<p align="left">Figure 20. Turn left</p>

<div align=center>
<img src="img_readme/turn_right.jpg" width="800px">
</div>

<p align="left">Figure 21. Turn right</p>

<div align=center>
<img src="img_readme/horizontal_flip.jpg" width="800px">
</div>

<p align="left">Figure 22. Horizontal flip</p>

<div align=center>
<img src="img_readme/vertical_flip.jpg" width="800px">
</div>

<p align="left">Figure 23. Vertical flip</p>

>**Note:** If you want to add a watermark to the image, please check the **Mark** checkbox, and then click the **Save image** button to generate an image with a watermark. In addition, you can replace the **water_mark.png** in the **images** folder with your own watermark.

<div align=center>
<img src="img_readme/watermark.jpg" width="800px">
</div>

<p align="left">Figure 24. Watermark</p>

### 5. Video processing

Some videos are provided in the **show** folder for testing, and you can open one of them. The video formats that support opening include **WMV, mp4, rmvb, flv**.

After opening the video, it will automatically start playing, and the progress bar below will update synchronously. The playback time will be displayed below the progress bar in the format of: playback duration/total duration, measured in seconds. In addition, you can also pull the progress bar to adjust the playback progress. What's more, by clicking the play/pause button, the video can be played or paused. If click on each video processing button, it will take effect from the next frame when the video restarts playing.

<div align=center>
<img src="img_readme/original_video.jpg" width="800px">
</div>

<p align="left">Figure 25. Original video</p>

<div align=center>
<img src="img_readme/mosaic.jpg" width="800px">
</div>

<p align="left">Figure 26. Mosaic (in the central area of the video frame)</p>

<div align=center>
<img src="img_readme/binarization_video.jpg" width="800px">
</div>

<p align="left">Figure 27. Binarization</p>

<div align=center>
<img src="img_readme/grayscale_video.jpg" width="800px">
</div>

<p align="left">Figure 28. Grayscale</p>

<div align=center>
<img src="img_readme/edge_detection_video.jpg" width="800px">
</div>

<p align="left">Figure 29. Edge detection</p>

<div align=center>
<img src="img_readme/smooth.jpg" width="800px">
</div>

<p align="left">Figure 30. Smooth</p>

>**Note:** the above operations were performed on the original video, but the following adjustments were made based on the previous processing.

<div align=center>
<img src="img_readme/scaling.jpg" width="800px">
</div>

<div align=center>
<img src="img_readme/scaling2.jpg" width="800px">
</div>

<p align="left">Figure 31. Scaling (2X, 0.5X)</p>

### 5. Point cloud processing

Some point cloud files are provided in the **clouds** folder for testing, and you can open one of them. Display in point cloud format by default. The point cloud formats that support opening and saving include **ply, pcd, obj**.

There are some precautions when saving point cloud files: 
- The file is saved in ASCII format by default. If the **Binary** checkbox is checked, it will be saved in binary format.
- obj files can not be saved in binary format.
- obj and ply files can only be saved in mesh format. After opening, they can display both point cloud and mesh.
- pcd files can only store point cloud. After opening, it would only display point cloud.

<div align=center>
<img src="img_readme/cloud_ply_obj.jpg" width="800px">
</div>

<p align="left">Figure 32. Open ply or obj file</p>

<div align=center>
<img src="img_readme/cloud_pcd.jpg" width="800px">
</div>

<p align="left">Figure 33. Open pcd file</p>

<div align=center>
<img src="img_readme/cloud_size.jpg" width="800px">
</div>

<div align=center>
<img src="img_readme/cloud_size2.jpg" width="800px">
</div>

<p align="left">Figure 34. Change point size (1, 5)</p>

<div align=center>
<img src="img_readme/mesh_surface.jpg" width="800px">
</div>

<p align="left">Figure 35. Mesh surface display</p>

<div align=center>
<img src="img_readme/mesh_wire.jpg" width="800px">
</div>

<p align="left">Figure 36. Mesh wire display</p>

<div align=center>
<img src="img_readme/overlay.jpg" width="800px">
</div>

<p align="left">Figure 37. Point cloud and mesh surface overlay</p>

>**Note:** first, click the **Generate** button, wait until the status bar prompts "Mesh generated successfully!". Then, click the **Surface**, **Wire**, or **Overlay** button to display the new mesh.

<div align=center>
<img src="img_readme/generate_mesh.jpg" width="800px">
</div>

<p align="left">Figure 38. Generate mesh from point cloud</p>

### 6. Acknowledgement

Many thanks to the authors of [QT5-Image-and-video-processing-software
](https://github.com/KyrieFeng23/QT5-Image-and-video-processing-software) and [CloudViewer](https://github.com/nightn/CloudViewer).
