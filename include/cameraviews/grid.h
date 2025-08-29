#ifndef VIEWGRID_H
#define VIEWGRID_H

#include "camera.h"

typedef struct GridCameraView {
    CameraView base;
    int row;
    int col;
    int rows; // total rows in the grid
    int cols; // total columns in the grid
} GridCameraView;

GridCameraView* GridCameraViewCreate(Camera* camera, int row, int col, int rows, int cols);

#endif

