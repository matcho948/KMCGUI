#include "KMCGUI.h"
#include <QtWidgets/QApplication>
#include "kmc_runner.h"
#include <QLABEL>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KMCGUI w;
    try
    {
        KMC::Runner runner;

        KMC::Stage1Params stage1Params;
        stage1Params
            .SetKmerLen(3)
            .SetInputFiles({ "C:\\Users\\match\\Desktop\\test1.fq" });

        auto stage1Result = runner.RunStage1(stage1Params);

        KMC::Stage2Params stage2Params;

        stage2Params
            .SetOutputFileName("31mers");

        auto stage2Result = runner.RunStage2(stage2Params);

        std::string test = std::to_string(stage2Result.nTotalKmers);
        std::string test2 = std::to_string(stage2Result.nUniqueKmers);
        //print some stats
        QLabel* label = new QLabel(&w);
        label->setText("Total kmers: " + QString::fromUtf8(test.c_str()) + "\n");
        label->show();
        QLabel* label2 = new QLabel(&w);
        label2->setText("Total unique: " + QString::fromUtf8(test2.c_str()));
        label2->show();

    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    w.show();
    return a.exec();
}
