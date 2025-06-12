#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVector>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>


class MineSpot;

// class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public: 
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() = default;    // destructor

    private:
        static const int ROWS = 16;
        static const int COLS = 30;
        static const int MINES = 99;

        // winoow layouts
        QFrame *topBarFrame;
        QFrame *gridFrame;
        QVBoxLayout *mainLayout;
        
        QHBoxLayout *topBarLayout;
        QGridLayout *mineLayout;

        QIcon smileIcon;
        QIcon resetPressedIcon;
        QIcon failedIcon;
        QIcon wonIcon;

        QPushButton *resetButton;


        // 2D array of MineSpot pointers
        QVector<QVector<MineSpot*>> minefield;

        void createMinefield(QGridLayout *layout, QWidget *parentWidget); // Allocate spots
        void placeMines();
        void calculateNumbers();
        void centerWindow();

    private slots:
        void onSpotUncovered(MineSpot *spot);
        void onResetGame();

        // Helper functions for recursion
        
        // void openNeighbors(MineSpot *spot);
        void recursiveOpen(MineSpot* spot);
        bool isValid(int r, int c);
        QVector<MineSpot*> getNeighbors(int row, int col);


};

#endif //MAINWINDOW_H