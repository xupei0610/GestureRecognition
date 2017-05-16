#define coders_prey ""\
    "                             _ooOoo_                             "\
    "                            o8888888o                            "\
    "                            88\".\"88                            "\
    "                            (| -_- |)                            "\
    "                            O\  =  /O                            "\
    "                         ____/`---'\____                         "\
    "                       .'  \\|     |\"  `.                       "\
    "                      /  \\|||  :  |||\"  \                      "\
    "                     /  _||||| -:- |||||-  \                     "\
    "                     |   | \\\  -  \"/ |   |                     "\
    "                     | \_|  ''\---/''  |   |                     "\
    "                     \  .-\__  `-`  ___/-. /                     "\
    "                   ___`. .'  /--.--\  `. . __                    "\
    "                ."" '<  `.___\_<|>_/___.'  >'"".                 "\
    "               | | :  `- \`.;`\ _ /`;.`/ - ` : | |               "\
    "               \  \ `-.   \_ __\ /__ _/   .-` /  /               "\
    "          ======`-.____`-.___\_____/___.-`____.-'======          "\
    "                             '=---='                             "\
    "          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^          "\
    "                     佛祖保佑        永无BUG                       "\
    "          God of coders blesses us. No bug will be met.          "

#include <QApplication>

#include "GestureControlSystem.h"
#include "CommandInputter.h"
#include "GestureAnalyst.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto h = new HandDetector;
    auto s = new SampleCollector;
    auto g = new GestureAnalyst;
    auto c = new CommandInputter;

    GestureControlSystem gcs(h, s, g, c);
    gcs.run();

    auto res = a.exec();

    delete h;
    delete s;
    delete g;
    delete c;

    return res;

    return 0;
}
