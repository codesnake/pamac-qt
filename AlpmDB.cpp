#include "AlpmDB.h"

AlpmDB::AlpmDB(PamacDatabase *db, QObject *parent):
    QObject(parent),
    db(db){
    pamac_database_enable_appstream(db);
}

AlpmDB::AlpmDB(QString configFile, QObject *parent):
    QObject(parent){
    db = (pamac_database_new(pamac_config_new(configFile.toUtf8())));
    pamac_database_enable_appstream(db);
}
