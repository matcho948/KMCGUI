#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KMCGUI.h"

class KMCGUI : public QMainWindow
{
    Q_OBJECT

public:
    KMCGUI(QWidget *parent = nullptr);
    ~KMCGUI();

private:
    Ui::KMCGUIClass ui;
};
