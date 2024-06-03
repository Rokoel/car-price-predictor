# About this project
This project focuses on a desktop app that can _predict car prices_ using a big dataset.
It has GUI written in **c++** using **QtGUI** framework and the **Qt Creator** app.

## Clone and Build

### 0. Prerequisites
You should have the **Qt Creator** and **git** installed. You should be able to access **git** from your project folder (it should be in your ```PATH```).

### 1. Clone the repository to you project folder
Use the following command while being in your project folder:
```bash
git clone git@github.com:Rokoel/car-price-predictor.git
```

### 2. Open the project folder with Qt Creator
In Qt Creator:
``` Open project -> Navigate to the project folder -> Choose the CMakeLists.txt file -> Use all the suggested kits ```

### 3. Download the car_prices.csv file from Kaggle
Go to [Kaggle Page](https://www.kaggle.com/datasets/syedanwarafridi/vehicle-sales-data) and download the zip archive.

### 4. Move the car_prices.csv into the project folder
Unzip the downloaded archive into any location and move the ```car_prices.csv``` into the project folder:  
  
**car-price-predictor**  
&nbsp;├── car-price-predictor_ru_RU.ts  
&nbsp;├── **car_prices.csv**  
&nbsp;├── CMakeLists.txt  
&nbsp;├── CMakeLists.txt.user  
&nbsp;├── dialog.cpp  
&nbsp;├── dialog.h  
&nbsp;├── dialog.ui  
&nbsp;├── main.cpp  
&nbsp;├── mainwindow.cpp  
&nbsp;├── mainwindow.h  
&nbsp;├── mainwindow.ui  
&nbsp;├── predictor.cpp  
&nbsp;└── resources.qrc  

### 5. Press the Build button in the Qt Creator
Press the ```Build``` button in the Qt Creator while in your project.

### 6. You're all set!
You can now run the project via the ```Run``` button and make any changes to the project. Congratulations!
