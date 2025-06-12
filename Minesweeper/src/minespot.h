#ifndef MINESPOT_H
#define MINESPOT_H

#include <QPushButton>
#include <QMouseEvent>

class MineSpot : public QPushButton
{
    Q_OBJECT

    public:
        explicit MineSpot(QWidget *parent = nullptr);

        bool isCovered = true;
        bool isMine = false;
        bool isFlagged = false;
        int adjMines = 0;
        int row = 0;
        int col = 0;

        void updateAppearance();
        void openSpot();

    signals:
        void uncovered(MineSpot *spot);    // Left-click to uncover
        void flagged(MineSpot *spot);       // Right-click to flag

    public slots:
        // void openSpot();

    protected:
        void mousePressEvent(QMouseEvent *event) override;

    // private:
        
};

#endif    // MINESPOT_H
