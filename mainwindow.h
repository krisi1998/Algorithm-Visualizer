#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionNew_Window_triggered();

    void on_actionHelp_triggered();

    void on_sizeSlider_valueChanged(int value);

    void on_visualise_button_clicked();

    void on_generate_button_clicked();

    void on_stop_button_clicked();

    void on_speedSlider_sliderReleased();

    void on_insert_button_clicked();

    void on_rangeBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    const int array_size;
    int current_array_size;
    double array_item_width;
    const double min_SceneView_size;
    unsigned int animation_speed;
    unsigned int array_range;
    bool stop_indicator = false;

    QVector<int> array;
    QVector<QGraphicsRectItem*> bar_items;
    //QVector<QGraphicsTextItem*> text_items;

    const QBrush gray_brush;
    const QBrush red_brush;
    const QBrush green_brush;
    const QBrush yellow_brush;
    const QPen black_pen;

    void bubleSort();
    void selectionSort();
    void insertionSort();
    void mergeSort(int l,int r);
    void merge(int l,int m,int r);
    void quickSort(int low,int high);
    int partition(int low,int high);

    /* ~~~ Custom Methods ~~~ */
    //Genarates a new array with random values and draws the corrensponding bars on scene
    void generateArray();

    //Draws the given array as bars on scene
    void updateScene(QVector<int> array);

    //Highlight bar items with the given color
    //Give a negative index value to skip that highlight
    void highlightBarItems(int i, int j, QBrush color);
};

#endif // MAINWINDOW_H
