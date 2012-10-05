# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
QT += network webkit
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE3499E6F

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp mainwindow.cpp \
    localdatabasefile.cpp \
    affair.cpp \
    weekjudge.cpp \
    alldata.cpp \
    settingwindow.cpp \
    remotedatabasemanager.cpp \
    PublicFunction.cpp
HEADERS += mainwindow.h \
    localdatabasefile.h \
    affair.h \
    weekjudge.h \
    settings.h \
    alldata.h \
    settingwindow.h \
    remotedatabasemanager.h \
    PublicFunction.h
FORMS += mainwindow.ui \
    settingwindow.ui \
    remotedatabasemanager.ui
RESOURCES += \
    icon.qrc

#macx{
#    ICON = YFReminder2.svg
#}
win32{
    RC_FILE += YFReminder2.rc
}
# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()


