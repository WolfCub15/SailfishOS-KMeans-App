#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include "KMeansClass.h"

int main(int argc, char *argv[])
{
    const QMetaObject meta = KMeansClass::staticMetaObject;
    QObject *obj = meta.newInstance();
    QObject::connect(obj, SIGNAL(calcKMeans()), obj, SLOT(print()));


    QGuiApplication *app = SailfishApp::application(argc, argv);
    QQuickView *view = SailfishApp::createView();
    view->setSource(SailfishApp::pathTo("qml/KMeans.qml"));
    qmlRegisterType<KMeansClass>("MyModule", 1, 0, "KMeansClass");
    view->show();

    return app->exec();
}
