/* Copyright 2010 NVIDIA Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Developed by Mustard Seed Software, LLC 
 * http://mseedsoft.com
 * Modified by Matthew Johnson-Roberson 
 * from original source: Visualize Physics - Wave 
 * Copyright (C) 2012 Matthew Johnson-Roberson
 * See COPYING for license details
 */

#include <QApplication>
#include <QSplashScreen>
#include <QString>
#include <QErrorMessage>
#include <osg/Notify>
#include <iostream>
#include <sstream>
#include "BQTMainWindow.h"
#include "BQTVersion.h"

#if defined(WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define __WINLOG
#endif


inline void logMessage(const char* typeStr, const char* msg) {
    std::ostringstream oss;
    oss << typeStr << ": " << msg << std::endl;
    std::cerr << oss.str();
#if defined(__WINLOG)
    OutputDebugStringA(oss.str().c_str());
#endif
}

#if QT_VERSION < 0x050000
static QtMsgHandler defMsgHandler = NULL;
#else
static QtMessageHandler defMsgHandler = NULL;
#endif

/** Message handler for Qt logging system. */
#if QT_VERSION < 0x050000
void messageHandler(QtMsgType type, const char *msg) {
     switch (type) {
         case QtDebugMsg:
            logMessage("Debug", msg);
             break;
         case QtWarningMsg:
            logMessage("Warning", msg);
             break;
         case QtCriticalMsg:
            logMessage("Critical", msg);
             break;
         case QtFatalMsg:
            logMessage("Fatal", msg);
             break;
     }

     if(type != QtDebugMsg && defMsgHandler) {
         QWidget* win = QApplication::activeWindow();
         QMainWindow* main = qobject_cast<QMainWindow*>(win);
         if(main) {
             main->lower();
         }

         defMsgHandler(type, msg);
     }
}

#else
static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message){
#if (QT_VERSION >= 0x050000) && defined (__APPLE__)
#warning "hack for QTVER 5.0 on mac"
    string buggedFunc="-[QNSView mouseDragged:]";
    if(buggedFunc == context.function){
       // printf("Supressed error\n");
    return;
    }
#endif
    switch (type) {
        case QtDebugMsg:
            logMessage("Debug", qPrintable(message));
            break;
        case QtWarningMsg:
            logMessage("Warning", qPrintable(message));
            break;
        case QtCriticalMsg:
            logMessage("Critical", qPrintable(message));
            break;
        case QtFatalMsg:
            logMessage("Fatal", qPrintable(message));
            break;
    }

    if(type != QtDebugMsg && defMsgHandler) {
        QWidget* win = QApplication::activeWindow();
        QMainWindow* main = qobject_cast<QMainWindow*>(win);
        if(main) {
            main->lower();
        }
        defMsgHandler(type,context, message);
    }
}
#endif
/** Application launch functionn. */
int main(int argc, char *argv[]) {
    //using namespace ews::app::model;
    using namespace ews::app::widget;

#if defined(QT_DEBUG)                
  //  osg::setNotifyLevel(osg::INFO);
#endif                
    
    // To see Qt object dumps on macos, run with the environment variable
    // "DYLD_IMAGE_SUFFIX" set to "_debug".
    
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    QApplication::setOrganizationName(EWS_ORGANIZATION_NAME);
    QApplication::setOrganizationDomain(EWS_BUNDLE_ID);
    QApplication::setApplicationVersion(EWS_VERSION);
    QApplication::setApplicationName(EWS_APP_NAME);
    
    // First register the dialog error handler, then
    // get the function pointer to it by passing zero to the handler installer
    // then install our own.
    QErrorMessage::qtHandler();
#if QT_VERSION < 0x050000
    defMsgHandler = qInstallMsgHandler(0);
    qInstallMsgHandler(messageHandler);
#else
    defMsgHandler = qInstallMessageHandler(0);
    qInstallMessageHandler(messageHandler);
 #endif
 /*   QSplashScreen splash;
    QPixmap img(":/images/splash");
    
    if(img.isNull()) {
       qWarning() << "Couldn't load splash image";
    }
    
    splash.setPixmap(img);
    splash.showMessage(QObject::tr("Starting up...."));
    splash.show();
*/
    
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    SimulationState state;
    state.setObjectName("root");

    EWSMainWindow w(&state);
    w.setWindowTitle(EWS_APP_NAME);
#if defined(__APPLE__)
  //  w.setWindowIcon(QIcon(":/images/appicon.png"));
#else    
    QApplication::setWindowIcon(QIcon(":/images/appicon"));
#endif    


    w.show();
    
    //splash.finish(&w);
    if (QApplication::arguments().size() > 1)
            w.loadFile(QApplication::arguments().mid(1,-1));


    return a.exec();
}
