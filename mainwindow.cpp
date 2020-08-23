#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "helpwindow.h"
#include "sleep.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    array_size(35),
    min_SceneView_size(1000.0),
    gray_brush(Qt::gray),
    red_brush(Qt::red),
    green_brush(Qt::green),
    yellow_brush(Qt::yellow),
    black_pen(Qt::black)
{
    //Basic setup
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //Setting up the Array size slider
    ui->sizeSlider->setRange(-25,35);
    ui->sizeSlider->setValue(0);

    //Setting up the Animation speed slider
    ui->speedSlider->setRange(0,250);
    ui->speedSlider->setValue(50);

    //Hiding stop button
    ui->stop_button->hide();

    //Setting default animation speed
    animation_speed = static_cast<uint>( ui->speedSlider->value() );

    //Setting default array range
    array_range = 10;

    //Setting up Array
    current_array_size = array_size;
    array_item_width = min_SceneView_size / static_cast<double>(current_array_size);
    array.resize(current_array_size);
    bar_items.resize(current_array_size);

    //Setting Array values and drawing it to the scene
    MainWindow::generateArray();
}

void MainWindow::generateArray()
{
    //Normalise bar item vertical size
    int v_size = 500 / array_range;

    for (int i = 0; i < current_array_size; i++)
    {
        //Give array random values from 0 to array_range
        //and draw that array on scene
        array.replace(i, qrand() % (array_range + 1) );
        bar_items.replace(i,scene->addRect(array_item_width*i*1.1,0,array_item_width,array.at(i)*v_size,black_pen,gray_brush));
    }
}

void MainWindow::updateScene(QVector<int> array)
{
    scene->clear();

    //Normalise bar item vertical size
    int v_size = 500 / array_range;

    //Draw array on scene
    for (int i = 0; i < current_array_size; i++)
    {
        bar_items.replace(i,scene->addRect(array_item_width*i*1.1,0,array_item_width,array.at(i)*v_size,black_pen,gray_brush));
    }

    QCoreApplication::processEvents();
}

void MainWindow::highlightBarItems(int i, int j, QBrush brush)
{
    //Normalise bar item vertical size
    int v_size = 500 / array_range;

    //If index is < 0 skip that highligh
    if(i >= 0)
        bar_items.replace(i,scene->addRect(array_item_width*i*1.1,0,array_item_width,array.at(i)*v_size,black_pen,brush));

    if(j >= 0)
        bar_items.replace(j,scene->addRect(array_item_width*j*1.1,0,array_item_width,array.at(j)*v_size,black_pen,brush));

    QCoreApplication::processEvents();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWindow win;
    win.setModal(true);
    win.exec();
}

void MainWindow::on_actionNew_Window_triggered()
{

}

void MainWindow::on_actionHelp_triggered()
{
    HelpWindow win;
    win.setModal(true);
    win.exec();
}

void MainWindow::on_sizeSlider_valueChanged(int value)
{
    //Calculating new array size and new item width
    current_array_size = array_size + value;
    array_item_width = min_SceneView_size / static_cast<double>(current_array_size);

    //Resizing
    array.resize(current_array_size);
    bar_items.resize(current_array_size);

    //Setting array values and drawing it to the scene
    scene->clear();
    generateArray();
}

void MainWindow::on_speedSlider_sliderReleased()
{
    //Get animation speed from speed slider when its released
    animation_speed = static_cast<uint>( ui->speedSlider->value() );
}

void MainWindow::on_rangeBox_currentIndexChanged(int index)
{
    //Update array range based of index of comboBox
    switch (index)
    {
    case 0:
        array_range = 10;
        break;
    case 1:
        array_range = 50;
        break;
    case 2:
        array_range = 100;
        break;
    case 3:
        array_range = 200;
        break;
    default:
        array_range = 500;
    }

    //Setting array values and drawing it to the scene
    scene->clear();
    generateArray();
}

void MainWindow::on_generate_button_clicked()
{
    //Setting array values and drawing it to the scene
    scene->clear();
    generateArray();
}

void MainWindow::on_stop_button_clicked()
{
    stop_indicator = true;
}

void MainWindow::on_insert_button_clicked()
{
    //Read input as string from line edit
    QString input_text = ui->lineEdit->text();

    if(input_text.isEmpty())
        return;

    //Check for letters or symbols
    for (int i = 0; i < input_text.length(); i++)
    {
        if(input_text.at(i).isLetter() || input_text.at(i).isSymbol() || input_text.at(i).isPunct())
            return;
    }

    //Split input in parts around 'space'
    QStringList input_list = input_text.split(" ", QString::SkipEmptyParts);

    //Copy those parts to array as unsigned integers
    for (int i = 0; i < input_list.length() && i < current_array_size; i++)
    {
        unsigned int input_value = input_list.at(i).toUInt();
        if( input_value < (array_range + 1) )
            array.replace(i, input_value );
    }

    //Draw changes in scene
    updateScene(array);
}

void MainWindow::on_visualise_button_clicked()
{
    //Disable ui interactions
    ui->generate_button->setEnabled(false);
    ui->insert_button->setEnabled(false);
    ui->sizeSlider->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->rangeBox->setEnabled(false);
    ui->visualise_button->setEnabled(false);
    ui->stop_button->setEnabled(true);

    //Replace visualise button with stop button
    ui->visualise_button->hide();
    ui->stop_button->show();

    //Get values from combo-boxes
    unsigned int alg_index = static_cast<unsigned int>( ui->comboBox->currentIndex() );

    //Chose the correct method to call
    switch (alg_index) {
    case 0:  //Buble Sort
        bubleSort();
        break;
    case 1:  //Selection Sort
        selectionSort();
        break;
    case 2:  //Insertion Sort
        insertionSort();
        break;
    case 3:  //Merge Sort
        mergeSort(0,array.size()-1);
        break;
    case 4:  //Quick Sort
        quickSort(0,array.size()-1);
        break;
    }

    //Set stop indicator to default
    stop_indicator = false;

    //Enable ui interactions after the called method is done
    ui->generate_button->setEnabled(true);
    ui->insert_button->setEnabled(true);
    ui->sizeSlider->setEnabled(true);
    ui->rangeBox->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->visualise_button->setEnabled(true);
    ui->stop_button->setEnabled(false);

    //Replace stop button with visualise button
    ui->stop_button->hide();
    ui->visualise_button->show();
}

void MainWindow::bubleSort()
{
    bool swapped;
       for (int i = 0; i < current_array_size-1; i++)
       {
         swapped = false;

         for (int j = 0; j < current_array_size-i-1; j++)
         {
            if (array.at(j) > array.at(j+1))
            {
               //Draw and wait
               highlightBarItems(j,j+1,red_brush);
               Sleep::qSleep(animation_speed);

               //move "j" to "j+1"
               int temp = array.at(j);
               array.replace(j,array.at(j+1));
               array.replace(j+1,temp);

               //Draw and wait
               updateScene(array);
               Sleep::qSleep(animation_speed);
               highlightBarItems(j,j+1,red_brush);
               Sleep::qSleep(animation_speed);
               updateScene(array);
               Sleep::qSleep(animation_speed);

               swapped = true;
            }

            //Check stop indicator
            if(stop_indicator)
                return;
         }

         // IF no elements were swapped by inner loop, then break
         if (swapped == false)
            break;
       }
}

void MainWindow::selectionSort()
{
    int min_id;

        // One by one move boundary of unsorted subarray
        for (int i = 0; i < current_array_size-1; i++)
        {
            // Find the minimum element in unsorted array
            min_id = i;

            for (int j = i+1; j < current_array_size; j++)
            {
                if (array.at(j) < array.at(min_id))
                {
                    min_id = j;
                }
            }

            //Draw and wait
            highlightBarItems(i,min_id,red_brush);
            Sleep::qSleep(animation_speed);

            // Swap the found minimum element with the first element
            int temp = array.at(min_id);
                array.replace(min_id,array.at(i));
                array.replace(i,temp);

            //Draw and wait
            updateScene(array);
            Sleep::qSleep(animation_speed);
            highlightBarItems(i,min_id,red_brush);
            Sleep::qSleep(animation_speed);
            MainWindow::updateScene(array);
            Sleep::qSleep(animation_speed);

            //Check stop indicator
            if(stop_indicator)
                return;
        }
}

void MainWindow::insertionSort()
{
    int i, key, j;

        for (i = 1; i < current_array_size; i++)
        {
            key = array.at(i);
            j = i - 1;

            /* Move elements of arr[0..i-1], that are
            greater than key, to one position ahead
            of their current position */
            while (j >= 0 && array.at(j) > key)
            {
                //Draw and wait
                highlightBarItems(i,j+1,red_brush);
                Sleep::qSleep(animation_speed);

                array.replace(j+1,array.at(j));
                j = j - 1;

                //Draw and wait
                updateScene(array);
                Sleep::qSleep(animation_speed);
            }

            //Draw and wait
            highlightBarItems(i,j+1,red_brush);
            Sleep::qSleep(animation_speed);

            array.replace(j+1,key);

            //Draw and wait
            updateScene(array);
            Sleep::qSleep(animation_speed);

            //Check stop indicator
            if(stop_indicator)
                return;
        }
}

void MainWindow::mergeSort(int l, int r)
{
    //Check stop indicator
    if(stop_indicator)
        return;

    /* l is for left index and r is right index of the
           sub-array of arr to be sorted */
            if (l < r)
            {
                // Same as (l+r)/2, but avoids overflow for
                // large l and h
                int m = l+(r-l)/2;

                // Sort first and second halves
                mergeSort(l, m);
                mergeSort(m+1, r);

                merge(l, m, r);
            }
}

void MainWindow::merge(int l, int m, int r)
{
    int i, j, k;

        int n1 = m - l + 1; //temp left array size
        int n2 =  r - m;    //temp right array size

        /* create temp arrays */
        int L[n1], R[n2];

        /* Copy data to temp arrays L[] and R[] */
        for (i = 0; i < n1; i++)
            L[i] = array.at(l + i);
        for (j = 0; j < n2; j++)
            R[j] = array.at(m + 1+ j);

        /* Merge the temp arrays back into arr[l..r]*/
        i = 0; // Initial index of first subarray
        j = 0; // Initial index of second subarray
        k = l; // Initial index of merged subarray
        while (i < n1 && j < n2)
        {
            if (L[i] <= R[j])
            {
                //Draw and wait
                highlightBarItems(l+i,m+j+1,red_brush);
                Sleep::qSleep(animation_speed);

                array.replace(k,L[i]);

                //Draw and wait
                updateScene(array);
                Sleep::qSleep(animation_speed);
                highlightBarItems(l+i,m+j+1,red_brush);
                Sleep::qSleep(animation_speed);
                updateScene(array);
                Sleep::qSleep(animation_speed);

                i++;
            }
            else
            {
                //Draw and wait
                highlightBarItems(l+i,m+j+1,red_brush);
                Sleep::qSleep(animation_speed);

                array.replace(k,R[j]);

                //Draw and wait
                updateScene(array);
                Sleep::qSleep(animation_speed);
                highlightBarItems(l+i,m+j+1,red_brush);
                Sleep::qSleep(animation_speed);
                updateScene(array);
                Sleep::qSleep(animation_speed);

                j++;
            }

            //Check stop indicator
            if(stop_indicator)
                return;

            k++;
        }

        /* Copy the remaining elements of L[], if there
           are any */
        while (i < n1)
        {
            //Draw and wait
            highlightBarItems(l+i,-5,red_brush);
            Sleep::qSleep(animation_speed);

            array.replace(k,L[i]);

            //Draw and wait
            updateScene(array);
            Sleep::qSleep(animation_speed);
            highlightBarItems(l+i,-5,red_brush);
            Sleep::qSleep(animation_speed);
            updateScene(array);
            Sleep::qSleep(animation_speed);

            i++;
            k++;

            //Check stop indicator
            if(stop_indicator)
                return;
        }

        /* Copy the remaining elements of R[], if there
           are any */
        while (j < n2)
        {
            //Draw and wait
            highlightBarItems(m+j,-5,red_brush);
            Sleep::qSleep(animation_speed);

            array.replace(k,R[j]);

            //Draw and wait
            updateScene(array);
            Sleep::qSleep(animation_speed);
            highlightBarItems(m+j,-5,red_brush);
            Sleep::qSleep(animation_speed);
            updateScene(array);
            Sleep::qSleep(animation_speed);

            j++;
            k++;

            //Check stop indicator
            if(stop_indicator)
                return;
        }
}

void MainWindow::quickSort(int low, int high)
{
    if (low < high)
        {
            /* pi is partitioning index, arr[p] is now
               at right place */
            int pi = partition(low, high);

            //Check if partition was ended by stop indicator
            if(pi == -200)
                return;

            // Separately sort elements before
            // partition and after partition
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
        }
}
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int MainWindow::partition(int low, int high)
{
    int pivot = array.at(high);    // pivot
        int i = (low - 1);  // Index of smaller element

        for (int j = low; j <= high- 1; j++)
        {
            // If current element is smaller than or
            // equal to pivot
            if (array.at(j) <= pivot)
            {
                i++;    // increment index of smaller element

                //Draw and wait
                highlightBarItems(i,j,red_brush);
                Sleep::qSleep(animation_speed);

                int temp = array.at(i);
                array.replace(i,array.at(j));
                array.replace(j,temp);

                //Draw and wait
                updateScene(array);
                Sleep::qSleep(animation_speed);
                highlightBarItems(i,j,red_brush);
                Sleep::qSleep(animation_speed);
                updateScene(array);
                Sleep::qSleep(animation_speed);
            }

            //Check stop indicator
            if(stop_indicator)
                return -200;
        }

        //Draw and wait
        highlightBarItems(i+1,high,red_brush);
        Sleep::qSleep(animation_speed);

        int temp = array.at(i+1);
        array.replace(i+1,array.at(high));
        array.replace(high,temp);

        //Draw and wait
        updateScene(array);
        Sleep::qSleep(animation_speed);
        highlightBarItems(i+1,high,red_brush);
        Sleep::qSleep(animation_speed);
        updateScene(array);
        Sleep::qSleep(animation_speed);

        //Check stop indicator
        if(stop_indicator)
            return -200;

        return (i + 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
