#include "mainwindow.h"
#include "minespot.h"

#include <QDebug>

#include <QRandomGenerator>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);  
    mainLayout = new QVBoxLayout(central);
    
    topBarFrame = new QFrame(this);
    topBarFrame->setFrameShape(QFrame::Panel);
    topBarFrame->setFrameShadow(QFrame::Sunken);
    topBarFrame->setLineWidth(4);
    topBarFrame->setStyleSheet("background-color: #c0c0c0;");

    topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 2, 0, 2);
    topBarLayout->setSpacing(5);
    
    smileIcon = QIcon("./icons/smile.png");
    resetPressedIcon = QIcon("./icons/resetPressed.png");
    failedIcon = QIcon("./icons/isFailed.png");
    wonIcon = QIcon("./icons/won.png");
    
    resetButton = new QPushButton(topBarFrame);
    
    resetButton->setIcon(smileIcon);
    resetButton->setIconSize(QSize(39, 39));
    resetButton->setFixedSize(39, 39);

    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetGame);
    connect(resetButton, &QPushButton::pressed, this, [=](){
        resetButton->setIcon(resetPressedIcon);
    });
    connect(resetButton, &QPushButton::released, this, [=]() {
        resetButton->setIcon(smileIcon);
    });

    topBarLayout->addStretch();
    topBarLayout->addWidget(resetButton);
    topBarLayout->addStretch();

    topBarFrame->setLayout(topBarLayout);
    
    gridFrame = new QFrame(this);
    gridFrame->setFrameShape(QFrame::Panel);
    gridFrame->setFrameShadow(QFrame::Sunken);
    gridFrame->setLineWidth(4);

    mineLayout = new QGridLayout();
    mineLayout->setSpacing(0);
    mineLayout->setContentsMargins(0, 0, 0, 0);
    gridFrame->setLayout(mineLayout);

    mainLayout->addWidget(topBarFrame);
    mainLayout->addWidget(gridFrame);

    createMinefield(mineLayout, central);
    placeMines();
    calculateNumbers();

    setCentralWidget(central);

    centerWindow();
}

void MainWindow::centerWindow()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int windowWidth = this->width();
    int windowHeight = this->height();

    int centerX = (screenGeometry.width() - windowWidth) / 2;
    int centerY = (screenGeometry.height() - windowHeight) / 2;

    this->move(centerX, centerY);
}

void MainWindow::createMinefield(QGridLayout *layout, QWidget *parentWidget)
{
    minefield.resize(ROWS);
    for (int i = 0; i < ROWS; i++) {
        minefield[i].resize(COLS);
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++)
        {
            MineSpot *spot = new MineSpot(parentWidget);
            spot->row = i;
            spot->col = j;
            layout->addWidget(spot, i, j);

            connect(spot, &MineSpot::uncovered, this, &MainWindow::onSpotUncovered);

            minefield[i][j] = spot;
        }
    }    
}

void MainWindow::onSpotUncovered(MineSpot *spot)
{
    // if (spot->isCovered) {
    //     spot->openSpot();
    // }

    if (spot->adjMines > 0) {
        spot->openSpot();
    }

    if (spot->isMine)
    {
        spot->setIcon(QIcon("./icons/explode.png"));
        resetButton->setIcon(QIcon("./icons/isFailed"));
        spot->isCovered = false;
        // Reveal all mines location, disable all spots
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++)
            {
                MineSpot *ms = minefield[i][j];
                // minefield[i][j]->isCovered = false;
                // minefield[i][j]->updateAppearance();

                if (ms->isCovered) {
                    ms->openSpot();
                    
                    if (ms->isFlagged && !ms->isMine) {
                        ms->setIcon(QIcon("./icons/isWrong.png"));
                    }
                } 

                // ms->setEnabled(false);
                // ms->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            }
        }
        return;
    }

    if (spot->adjMines == 0) {
        recursiveOpen(spot);
    }

    
    // recursiveOpen(spot);

    // check for a win condition
    bool allUncovered = true;
    for (int i = 0; i < ROWS; i++) {
        for (int j= 0; j < COLS; j++)
        {
            MineSpot *ms = minefield[i][j];
            if (!ms->isMine && ms->isCovered)
            {
                allUncovered = false;
                break;
            }
        }
    }
    if (allUncovered) {
        qDebug() << "win";
        resetButton->setIcon(wonIcon);
    }


}

void MainWindow::onResetGame()
{
    // placeholder
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j)
        {
            MineSpot *spot = minefield[i][j];
            if (spot)
            {
                mineLayout->removeWidget(spot);
                spot->deleteLater();
            }
        }
    }

    minefield.clear();

    createMinefield(mineLayout, centralWidget());
    placeMines();
    calculateNumbers();

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            qDebug() << "Cell" << i << j << "has" << minefield[i][j]->adjMines << "adj mines.";
    }
}
    
}

void MainWindow::placeMines()
{
    int placed = 0;
    while (placed < MINES)
    {
        int row = QRandomGenerator::global()->bounded(0, ROWS);
        int col = QRandomGenerator::global()->bounded(0, COLS);

        MineSpot *spot = minefield[row][col];
        if (!spot->isMine)
        {
            spot->isMine = true;
            placed++;
        }
    }
    
}

void MainWindow::calculateNumbers()
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) 
        {
            MineSpot *spot = minefield[i][j];
            if (!spot->isMine)
            {
                int mineCount = 0;

                for (int h = -1; h <= 1; h++) { 
                    for (int v = -1; v <= 1; v++) 
                    {
                        int nr = i + h;
                        int nc = j + v;
                        if (isValid(nr, nc) && minefield[nr][nc]->isMine) {
                            mineCount++;
                        }
                    }
                }
                spot->adjMines = mineCount;

                // qDebug() << "calculateNumber: " << spot->adjMines << "adj mines.";

            }

        }
    }
}

bool MainWindow::isValid(int r, int c)
{
    return (r >= 0 && r < ROWS && c >= 0 && c < COLS);
}



QVector<MineSpot*> MainWindow::getNeighbors(int row, int col)
{
    QVector<MineSpot*> neighbors;

    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++dc)
        {
            if (dr == 0 && dc == 0) continue;

            int nr = row + dr;
            int nc = col + dc;
            if (isValid(nr, nc))
            {
                neighbors.push_back(minefield[nr][nc]);
            }
        }
    }
    return neighbors;
}

void MainWindow::recursiveOpen(MineSpot *spot)
{
    if (!spot) {
        qDebug() << "recursiveOpen: null spot -> return";
        return;
    }

    if (!spot->isCovered) {
        qDebug() << "recursiveOpen: spot is already uncovered -> return";
        return;
    }
    if (spot->isMine) {
        qDebug() << "recursiveOpen: spot is a mine -> return";
        return;
    }
    if (spot->isFlagged) {
        qDebug() << "recursiveOpen: spot is flagged -> return";
        return;
    }
    
    // if (!spot || !spot->isCovered || spot->isMine) 
    //     return;

    // If we get here, we actually do open it
    qDebug() << "Spot" << spot->row << spot->col << "cover= " << spot->isCovered
             << "adj=" << spot->adjMines << " -> openSpot";

    spot->openSpot();
    qDebug() << "rightAfter openSpot: " << spot->adjMines << "adj mines.";
    
    if (spot->adjMines == 0)
    {
        // qDebug() << "Zero-adj mine at ("
        //          << spot->row << ","
        //          << spot->col << "), starting recursive open ...";

        auto neighbors = getNeighbors(spot->row, spot->col);
        for (MineSpot* n : neighbors)
            recursiveOpen(n);

    }

    // QVector<MineSpot*> neighbors = getNeighbors(spot->row, spot->col);

}
