#include <thread>
#include <ui_KMCGUI.h>
#include "configuration.h"

void configureSliders(Ui::KMCGUIClass ui)
{
    ui.kmerLengthSlider->setMinimum(1);
    ui.kmerLengthSlider->setMaximum(100);
    ui.kmerLengthSlider->setValue(1);
    ui.threadsSlider->setMinimum(1);
    ui.threadsSlider->setMaximum(64);
    ui.threadsSlider->setValue(std::thread::hardware_concurrency());
    ui.GBSlider->setMinimum(2);
    ui.GBSlider->setMaximum(128);
    ui.GBSlider->setValue(12);
    ui.signatureLengthSlider->setMinimum(5);
    ui.signatureLengthSlider->setMaximum(11);
    ui.signatureLengthSlider->setValue(9);
    ui.nBinsSlider->setMinimum(64);
    ui.nBinsSlider->setMaximum(2000);
    ui.nBinsSlider->setValue(512);
    ui.threadsSliderStage2->setMinimum(1);
    ui.threadsSliderStage2->setMaximum(64);
    ui.threadsSliderStage2->setValue(std::thread::hardware_concurrency());
    ui.GBSliderStage2->setMinimum(2);
    ui.GBSliderStage2->setMaximum(128);
    ui.GBSliderStage2->setValue(12);
}

void configureCheckboxes(Ui::KMCGUIClass ui)
{
    ui.canoncialKmersCheckBox->setChecked(1);
}