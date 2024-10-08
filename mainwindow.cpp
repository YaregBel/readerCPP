#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtWidgets>
#include <QLabel>
#include <QMainWindow>
#include <QFontMetrics>
#include <QTextEdit>

#include <QPushButton>
#include <QVBoxLayout>
#include <QStringList>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), currentPage(0)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Создаем label, в котором будет отображаться весь текст на странице
    // infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to"
    //                           "invoke a context menu</i>"));
    // Чтобы функции ниже работали, их нужно определить в этом файле.
    // infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    // infoLabel->setAlignment(Qt::AlignCenter);

    label = new QLabel(this);
    //label->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    //label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //label->setAlignment(Qt::AlignCenter);

    // Кнопки, для перемещения между страницами
    nextButton = new QPushButton("Next", this);
    prevButton = new QPushButton("Previous", this);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5,5,5,5);
    //layout->addWidget(topFiller);
    layout->addWidget(label);
    layout->addWidget(prevButton);
    layout->addWidget(nextButton);
    //layout->addWidget(bottomFiller);

    widget->setLayout(layout);

    // Получаем размеры label'a для решения проблемы получения
    // некоретных размеров.
    QSize initialSize = label->size();
    qDebug() << "Начальный размер QLabel (sizeHint):" << initialSize;

    QString filename = "d:/buff.txt";
    QFile file(filename);
    if ( file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        qDebug() << data;
        text = tr(data);
    }
    file.close();

    paginateText();
    updatePageDisplay();
    // Подключаем слоты для кнопок
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextPage);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::prevPage);

    createActions();
    createMenus();
    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Menus"));
    //setMinimumSize(160, 160);
    //resize(480, 320);



}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // Получаем новый размер QLabel при изменении размеров окна
    QSize currentSize = label->size();
    qDebug() << "Текущий размер QLabel после изменения:" << currentSize;

    pages.clear();
    label->clear();
    currentPage = 0;

    paginateText();
    updatePageDisplay();
}


void MainWindow::paginateText() {
    QFontMetrics metrics(label->font());
    int labelWidth = label->width();
    int labelHeight = label->height();

    qDebug() << labelWidth << labelHeight;

    QStringList currentPageLines;
    QStringList words = text.split(' '); // Разбиваем текст на слова
    QString currentLine;

    foreach (const QString &word, words) {
        QString tempLine = currentLine.isEmpty() ? word : currentLine + " " + word;
        if (metrics.horizontalAdvance(tempLine) <= labelWidth) {
            currentLine = tempLine;
        } else {
            currentPageLines.append(currentLine);
            currentLine = word;
        }

        if (metrics.height() * currentPageLines.count() >= labelHeight) {
            pages.append(currentPageLines.join('\n'));
            currentPageLines.clear();
        }
    }

    if (!currentLine.isEmpty()) {
        currentPageLines.append(currentLine);
    }

    if (!currentPageLines.isEmpty()) {
        pages.append(currentPageLines.join('\n'));
    }

}



void MainWindow::updatePageDisplay() {

    qDebug() << pages.length();
    if (currentPage >= 0 && currentPage < pages.size()) {
        label->setText(pages[currentPage]);
    }

    prevButton->setEnabled(currentPage > 0);
    nextButton->setEnabled(currentPage < pages.size() - 1);
}

void MainWindow::nextPage() {
    if (currentPage < pages.size() - 1) {
        currentPage++;
        updatePageDisplay();
    }
}

void MainWindow::prevPage() {
    if (currentPage > 0) {
        currentPage--;
        updatePageDisplay();
    }
}

MainWindow::~MainWindow()
{
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event){

    QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}
#endif //QT_NO_CONTEXTMENU


void MainWindow::newFile(){
    infoLabel->setText(tr("Invoked <b>File|New</b>"));

    QString filename = "d:/buff.txt";
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly)){

        QTextStream stream(&file);
        stream << "something";

    }
}

void MainWindow::open(){

    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), "", tr("Image Files (*.txt *.html *.epub *.fb2)"));

    //qDebug() << fileName;

    QFile file(fileName);
    if ( file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        qDebug() << data;
        text = tr(data);
    }
    file.close();

    paginateText();
    updatePageDisplay();
}

void MainWindow::save(){
    infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void MainWindow::print()
{
    infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void MainWindow::undo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::redo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::cut()
{
    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::copy()
{
    infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
}

void MainWindow::paste()
{
    infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void MainWindow::bold()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
}

void MainWindow::italic()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
}

void MainWindow::leftAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
}

void MainWindow::rightAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
}

void MainWindow::justify()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Justify</b>"));
}

void MainWindow::center()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Center</b>"));
}

void MainWindow::setLineSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void MainWindow::setParagraphSpacing(){
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Paragraph Spacing</b>"));
}
void MainWindow::about(){
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Menu</b> example shows how to create "
                          "menu-bar menus and context menus."));
}

void MainWindow::aboutQt()
{
    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

void MainWindow::createActions(){
    newAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                         tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);


    openAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                         tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing fil"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                          tr("&Save..."));
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save  the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    printAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrint),
                           tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);

    exitAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit),
                          tr("&Exit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    undoAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo),
                          tr("Undo"));
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the application"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    redoAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo),
                          tr("&Redo"));
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the application"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    cutAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCut),
                         tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    copyAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy),
                          tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    pasteAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste),
                           tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

    boldAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextBold),
                          tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, &QAction::triggered, this, &MainWindow::bold);

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::FormatTextItalic),
                            tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, &QAction::triggered, this, &MainWindow::italic);

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, &QAction::triggered, this, &MainWindow::setLineSpacing);

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, &QAction::triggered,
            this, &MainWindow::setParagraphSpacing);

    aboutAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
                           tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);

    leftAlignAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyLeft),
                               tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, &QAction::triggered, this, &MainWindow::leftAlign);

    rightAlignAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyRight),
                                tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, &QAction::triggered, this, &MainWindow::rightAlign);

    justifyAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyFill),
                             tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, &QAction::triggered, this, &MainWindow::justify);

    centerAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyCenter),
                            tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, &QAction::triggered, this, &MainWindow::center);

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);
}

void MainWindow::createMenus(){

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);

    fileMenu->addSeparator();

    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
    helpMenu->addSeparator();

    formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct);
}
