#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QMouseEvent>
#include <QtCore/QObject>
#include <QtWidgets/QGridLayout>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define SHARED_MEM_NAME "/OS_APP"
const int BUFFERSIZE = 1024;

int n = 3, m = 3;
using namespace std;

void writeToOs(void *ptr, const std::string &message) {
    qDebug() << "Message:" << QString::fromStdString(message);

    int message_length = message.length();
    memcpy(ptr, &message_length, sizeof(message_length));
    memcpy((char *) ptr + sizeof(message_length), message.c_str(), message_length);
}

int main(int argc, char **argv) {
    pid_t pid = getpid();
    int shm_fd = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    void *ptr = mmap(0, BUFFERSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    srand(time(0));
    n = rand() % 8 + 1;
    m = rand() % 8 + 1;
    string msg_to_os = "New Process: " + to_string(pid) + " With " + to_string(n * m) + " Pages\n";
    writeToOs(ptr, msg_to_os);

    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(600, 400);
    window.setWindowTitle("Program 1");

    auto *grid = new QGridLayout();
    auto addButtons = [&]()-> void {
        int cnt = 1;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                QString buttonName = QString("Button %1").arg(cnt++);
                QPushButton *button = new QPushButton(buttonName);

                grid->addWidget(button, i, j);
                QObject::connect(button, &QPushButton::clicked, [button, ptr, pid]() {
                    string msg = "Acess Process " + to_string(pid) + " Page " + button->text().toStdString();
                    writeToOs(ptr, msg);
                });
            }
        }
    };
    addButtons();

    QPushButton *showStatusButton = new QPushButton("Show Status");
    QObject::connect(showStatusButton, &QPushButton::clicked, [ptr, pid]() {
        string statusMsg = "Show Status clicked for process " + to_string(pid);
        writeToOs(ptr, statusMsg);
    });

    QVBoxLayout *outerLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *hCenterLayout = new QHBoxLayout();

    topLayout->addStretch();
    topLayout->addWidget(showStatusButton);
    topLayout->addStretch();

    QWidget *gridWidget = new QWidget();
    gridWidget->setLayout(grid);

    hCenterLayout->addStretch();
    hCenterLayout->addWidget(gridWidget);
    hCenterLayout->addStretch();

    outerLayout->addLayout(topLayout);
    outerLayout->addStretch();
    outerLayout->addLayout(hCenterLayout);
    outerLayout->addStretch();

    window.setLayout(outerLayout);
    window.show();

    // Handle Closing
    QObject::connect(&app, &QApplication::aboutToQuit, [ptr, pid]() {
        qDebug() << "Application is closing! Performing cleanup...";
        string delMsg = "Close " + to_string(pid);
        writeToOs(ptr, delMsg);
    });

    return app.exec();
}
