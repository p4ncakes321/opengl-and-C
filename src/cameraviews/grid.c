#include "cameraviews/grid.h"
#include <stdlib.h>

static void GridCameraViewResize(CameraView* view, int totalWidth, int totalHeight) {
    GridCameraView* gview = (GridCameraView*)view;

    int cellWidth  = totalWidth  / gview->cols;
    int cellHeight = totalHeight / gview->rows;

    view->viewport.width  = cellWidth;
    view->viewport.height = cellHeight;
    view->viewport.x = gview->col * cellWidth;
    view->viewport.y = gview->row * cellHeight;

    if (view->camera->getScrSizeFunc)
        view->camera->getScrSizeFunc(view->camera, totalWidth, totalHeight);
}

GridCameraView* GridCameraViewCreate(Camera* camera, int row, int col, int rows, int cols) {
    GridCameraView* view = (GridCameraView*)malloc(sizeof(GridCameraView));
    if (!view) return NULL;

    view->row = row;
    view->col = col;
    view->rows = rows;
    view->cols = cols;

    Viewport vp = {0, 0, 0, 0};
    CameraViewIntialize(&view->base, camera, vp, GridCameraViewResize);

    return view;
}

