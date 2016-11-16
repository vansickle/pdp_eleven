
#include "processorwindow.h"
#include "ui_processorwindow.h"

#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <cstdio>
#include <cstring>
#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QFileDialog>
#include <QPlainTextEdit>
#include "qhexedit.h"

extern "C"
{
    #include "memory.h"
    #include "system.h"
    #include "asm.h"
    #include "arraylist.h"
    #include "processor.h"
    #include "memory.h"
}

int intBase = 10;
QHexEdit* qhe;

ProcessorWindow::ProcessorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessorWindow)
    , monitorHeight(VIDEO_HEIGHT)
    , monitorWidth(VIDEO_WIDTH)
{
    ui->setupUi(this);
    ui->txtEditor->setText(QString("Assemble code, Load it to memory and then press 'Run'"));
    setRegisters();

    //FILE* fl = fopen("maze", "rb");
    //FILE* fl = fopen("/Users/vansickle/work/innopolis/ca/pdp_eleven/AppGui/maze", "rb");
    //FILE* fl = fopen("C:\\maze", "rb");
    //if(!fl)
    //    return ;

    //fseek(fl, 0, SEEK_END); // seek to end of file
    //int size = ftell(fl); // get current file pointer
    //fseek(fl, 0, SEEK_SET); // seek back to beginning of file

    //char* buffer = new char[size];
    //fread(buffer, sizeof(char), size, fl);

    //QImage image((unsigned char*)buffer, 256, 256,QImage::Format_Mono);

    //ui->monitor->setPixmap(QPixmap::fromImage(image));

    ui->intBaseComboBox->addItem("8");
    ui->intBaseComboBox->addItem("10");
    ui->intBaseComboBox->addItem("16");
    ui->intBaseComboBox->setCurrentIndex(1);

    memmoryInitialize();
    qhe = new QHexEdit(ui->memoryTab);
    ui->memoryTab->layout()->addWidget(qhe);

    QByteArray qByteArray = QByteArray::fromRawData(reinterpret_cast<const char*>(memoryGetPointer()), MEMORY_TOTAL_SIZE);
    qhe->setData(qByteArray);
}

ProcessorWindow::~ProcessorWindow()
{
    delete ui;
}


void ProcessorWindow::keyPressEvent( QKeyEvent *k )
{
    QString ch = k->text();
    if(!ch.isEmpty())
    {
        int asciNumber = (int)(ch[0].toLatin1());
        printf("%d", asciNumber);
    }
}

/*
 *  Public slot
 */

const char** split(QString text, int &size){
    QStringList qstrList = text.split('\n');

    const char **buf = (const char **)malloc(qstrList.size() * sizeof(const char *));

    size = qstrList.size();
    for (int i = 0; i < qstrList.size(); ++i)
    {
        QByteArray qarr = qstrList.at(i).toLocal8Bit();

        char *str = new char[qarr.size() + 1];
        strcpy(str, qarr.data());
        std::cout << str << std::endl;
        buf[i] = str;
    }
    return buf;
}

void ProcessorWindow::on_assemblyButton_clicked()
{
    QString asmText = ui->asmTextEdit->toPlainText();

    int length;
    const char** buf = split(asmText, length);


//    const char *buf[] = {
//        "; Program to copy and determine length of string",
//        ".origin 1000",
//        "start: mov #msga, r1",
//        "  mov #msgb, r2",
//        "  clr r0",
//        "l1: movb (r1)+, (r2)+",
//        "  beq done",
//        "  inc r0",
//        "  br l1",
//        "done: halt",
//        "msga: .string \"A string whose length is to be determined\"",
//        "msgb: .string \"Different string that should get overwritten\"",
//        ".end start"
//    };

    arrayClear();

    assembly(buf, length);

    QString machinCodeStr;

    char* outbuf = (char *)malloc((2*6+2)*arraySize()/2);
    for (int i = 0; i < arraySize(); i+=2){

        uint16_t addr = arrayGetValue(i);
        uint16_t instr = arrayGetValue(i+1);

        std::snprintf(outbuf+(2*6+2)*i/2, 2*6+2, "%o %06o\n", addr, instr);
    }

    for (int i = 0; i < arraySize(); i+=2) {
        machinCodeStr.append(outbuf+(2*6+2)*i/2);
    }

    ui->machineCodeEditor->clear();
    ui->machineCodeEditor->append(machinCodeStr);
}

void ProcessorWindow::setRegister(int number, QLineEdit* edit){
    QString reg;
    QTextStream ts(&reg);
    ts.setIntegerBase(intBase);
    ts << *getRegister(number);
    edit->setText(reg);
}

void ProcessorWindow::setRegisters(){
    this->setRegister(0, ui->r0edit);
    this->setRegister(1, ui->r1edit);
    this->setRegister(2, ui->r2edit);
    this->setRegister(3, ui->r3edit);
    this->setRegister(4, ui->r4edit);
    this->setRegister(5, ui->r5edit);
    this->setRegister(6, ui->r6edit);
    this->setRegister(7, ui->r7edit);
}

#include "logo.h"

void ProcessorWindow::on_runButton_clicked()
{
    ui->txtEditor->append(QString("Button 'RUN' is pressed"));
    evalCode();
    setRegisters();

    QImage image(memoryGetVideoRom(), VIDEO_HEIGHT, VIDEO_WIDTH, QImage::Format_Mono);
    ui->monitor->setPixmap(QPixmap::fromImage(image));
}


uint16_t sample_programm[] = {
0012701,
0000024,
0012702,
0000076,
0005000,
0112122,
0001402,
0005200,
0000774,
0000000,

0020101,
0072163,
0064562,
0063556,
0073440,
0067550,
0062563,
0066040,
0067145,
0072147,
0020150,
0071551,
0072040,
0020157,
0062542,
0062040,
0072145,
0071145,
0064555,
0062556,
0000144,

0064504,
0063146,
0071145,
0067145,
0020164,
0072163,
0064562,
0063556,
0072040,
0060550,
0020164,
0064163,
0072557,
0062154,
0063440,
0072145,
0067440,
0062566,
0073562,
0064562,
0072164,
0067145,
0000000
};

void ProcessorWindow::on_loadButton_clicked()
{
    systemInitialize();
//    arrayClear();

//    //temporary, while assembler works wrong
//    for (int i = 0; i < sizeof(sample_programm)/sizeof(sample_programm[0]); ++i) {
//        arrayPush(0);
//        arrayPush(sample_programm[i]);
////        arraySetValue(i,sample_programm[i]);
//    }

    setProgrammStart(arrayGetValue(0));
    for (int i = 0; i < arraySize(); i+=2){
        uint16_t *procMemory = (uint16_t*) getMemory(i);
        uint16_t machineWord = arrayGetValue(i+1);
        //TODO remake, init memory
        *procMemory = machineWord;
    }
}

void ProcessorWindow::on_intBaseComboBox_activated(const QString &arg1)
{
    qDebug() << "Activated" << arg1;
    intBase = arg1.toInt();
    setRegisters();
}

void ProcessorWindow::on_loadFromFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open Assembly Program"), QDir::current().absolutePath(), tr("Image Files (*.asm *.txt)"));
    qDebug() << fileName;

    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    ui->asmTextEdit->clear();
    ui->asmTextEdit->appendPlainText(in.readAll());

}

void ProcessorWindow::on_resetButton_clicked()
{
    prepareProcessor();
    setRegisters();
}

void ProcessorWindow::on_stepButton_clicked()
{
}

void ProcessorWindow::on_refreshMemoryButton_clicked()
{
    //TODO refresh using some hexedit funciton (without recreate bytearray)
    //TODO auto reload by timer
    QByteArray qByteArray = QByteArray::fromRawData(reinterpret_cast<const char*>(memoryGetPointer()), MEMORY_TOTAL_SIZE);
    qhe->setData(qByteArray);
}
