#include <MainWindow.h>
#include "vtkAutoInit.h"
MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
	ui.setupUi(this);
}

MainWindow::~MainWindow()
{
}