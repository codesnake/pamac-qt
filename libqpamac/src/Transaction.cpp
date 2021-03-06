#include "Transaction.h"
#include "Utils.h"
#include <QQmlEngine>
#include <atomic>
#include <QVariant>

bool LibQPamac::Transaction::run(const QStringList& toInstall, const QStringList& toRemove, const QStringList& toLoad,
                                   const QStringList& toBuild, const QStringList& tempIgnore, const QStringList& overwriteFiles,
                                 const QList<SnapPackage>& toInstallSnap, const QList<SnapPackage>& toRemoveSnap)
{

    for(auto& name : toInstall){
        pamac_transaction_add_pkg_to_install(m_handle,name.toUtf8());
    }

    for(auto& name : toRemove){
        pamac_transaction_add_pkg_to_remove(m_handle,name.toUtf8());
    }

    for(auto& name : toLoad){
        pamac_transaction_add_path_to_load(m_handle,name.toUtf8());
    }

    for(auto& name : toBuild){
        pamac_transaction_add_aur_pkg_to_build(m_handle,name.toUtf8());
    }

    for(auto& name : tempIgnore){
        pamac_transaction_add_temporary_ignore_pkg(m_handle,name.toUtf8());
    }

    for(auto& name : overwriteFiles){
        pamac_transaction_add_overwrite_file(m_handle,name.toUtf8());
    }

    for(auto& pkg :toInstallSnap){
        pamac_transaction_add_snap_to_install(m_handle,pkg.handle());
    }
    for(auto& pkg: toRemoveSnap){
        pamac_transaction_add_snap_to_remove(m_handle,pkg.handle());
    }

    setProperty("started",true);
    auto result = bool(pamac_transaction_run(m_handle));
    this->setProperty("progress",0);
    this->setProperty("started",false);
    return result;
}

bool LibQPamac::Transaction::runSysupgrade(bool forceRefresh)
{
    pamac_transaction_add_pkgs_to_upgrade(m_handle,forceRefresh);

    setProperty("started",true);
    auto result = bool(pamac_transaction_run(m_handle));

    this->setProperty("progress",0);
    this->setProperty("started",false);
    return result;
}

void LibQPamac::Transaction::setDatabase(LibQPamac::Database *database)
{
    if (m_database == database) {
        return;
    }
    m_handle = pamac_transaction_new(database->getHandle());
    init();

    Q_EMIT databaseChanged(m_database);
}


void LibQPamac::Transaction::init()
{
    //methods override
        PAMAC_TRANSACTION_GET_CLASS(m_handle)->ask_commit=
                Utils::cify([=](PamacTransaction* self, PamacTransactionSummary* summary)->gboolean{
        auto engine = qmlEngine(this);
        return gboolean(engine->fromScriptValue<QVariant>(
                            this->m_requestCommit.call({engine->toScriptValue(TransactionSummary(summary))}))
                        .toBool());
    });
//    PAMAC_TRANSACTION_GET_CLASS(m_handle)->choose_optdeps=
//            Utils::cify([=](PamacTransaction* self, const gchar* pkgname, gchar** optdeps, int optdeps_length1)->GList*{

//        QStringList lst = Utils::cStringArrayToQStringList(optdeps,optdeps_length1);

//        auto engine = qmlEngine(this);
//        auto varList = engine->fromScriptValue<QList<bool>>(this->m_requestOptDepends.call({{QString::fromUtf8(pkgname),engine->toScriptValue(lst)}}));

//        GList* result = nullptr;

//        int minLength = std::min(optdeps_length1,varList.length());

//        for(int i =0;i<minLength;i++){
//            char* res = new char[lst[i].toUtf8().length()];
//            std::memcpy(res,lst[i].toUtf8(),lst[i].toUtf8().size());
//            result = g_list_prepend(result,res);
//        }


//        return result;
//    });
//    PAMAC_TRANSACTION_GET_CLASS(m_handle)->choose_provider=
//            Utils::cify([=](PamacTransaction* self, const gchar* depend, gchar** providers, int providers_length1)->gint{
//        QStringList lst = Utils::cStringArrayToQStringList(providers,providers_length1);


//        auto engine = qmlEngine(this);

//        return engine->fromScriptValue<int>(this->m_requestChooseProvider.call({{QString::fromUtf8(depend),engine->toScriptValue(lst)}}));
//    });

//    PAMAC_TRANSACTION_GET_CLASS(m_handle)->ask_import_key=
//            Utils::cify([=](PamacTransaction* self, const gchar* pkgname, const gchar* key, const gchar* owner)->gboolean{

//        auto engine = qmlEngine(this);
//        return gboolean(engine->fromScriptValue<bool>(this->m_requestImportKey.call({{QString::fromUtf8(pkgname),QString::fromUtf8(key),QString::fromUtf8(owner)}})));
//    });
//    PAMAC_TRANSACTION_GET_CLASS(m_handle)->ask_edit_build_files =
//            Utils::cify([=](PamacTransaction* self, PamacTransactionSummary* summary)->gboolean{
//        auto engine = qmlEngine(this);

//        return false;
//    });

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_action",
                     reinterpret_cast<GCallback>(+Utils::cify([this](GObject* obj,char* action){
                         Q_UNUSED(obj);
                         Q_EMIT this->emitAction(QString::fromUtf8(action));
                     })),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_action_progress",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* action,char* status,double progress,Transaction* t){
                         Q_UNUSED(obj);

                         Q_EMIT t->emitActionProgress(QString::fromUtf8(action),QString::fromUtf8(status),progress);
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_error",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,char** details,int size,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->emitError(QString::fromUtf8(message),
                         Utils::cStringArrayToQStringList(details,size));
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_warning",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* warning,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->emitWarning(QString::fromUtf8(warning));
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->startPreparing();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->stopPreparing();
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"emit_script_output",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->emitScriptOutput(QString::fromUtf8(message));
                     }),this);

    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"important_details_outpout",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool message,Transaction* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->importantDetailsOutput(message);
                     }),this);





    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_waiting",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->startWaiting();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_waiting",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->stopWaiting();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_downloading",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->startDownloading();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_downloading",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->stopDownloading();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"start_building",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->startBuilding();
                     }),this);
    g_signal_connect(static_cast<PamacTransaction*>(m_handle),"stop_building",reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_EMIT t->stopBuilding();
                     }),this);

    for(auto signal : {&Transaction::startWaiting,&Transaction::startBuilding,&Transaction::startPreparing,&Transaction::startDownloading}){
        connect(this,signal,[=](){
            setProperty("indeterminate",true);});
    }

    for(auto signal : {&Transaction::stopWaiting,&Transaction::stopBuilding,&Transaction::stopPreparing,&Transaction::stopDownloading}){
        connect(this,signal,[=](){this->setProperty("indeterminate",false);});
    }

    connect(this,&Transaction::emitActionProgress,[=](const QString& action,const QString& status,double progress){
        this->setProperty("action",action);
        this->setProperty("progress",progress);

        QString details = this->property("details").toString();
        if(details.endsWith(action+"\n")){
            return;
        }
        details.append(action+"\n");
        this->setProperty("details",details);

    });
    connect(this,&Transaction::emitAction,[=](const QString& action){
        qDebug()<<action;
        this->setProperty("action",action);
        QString details = this->property("details").toString();
        details.append(action+"\n");
        this->setProperty("details",details);
    });
    connect(this,&Transaction::finished,[=](bool success){
        Q_UNUSED(success)

    });
    connect(this,&Transaction::emitScriptOutput,[=](QString message){
        QString details = this->property("details").toString();
        details.append(message+"\n");
        this->setProperty("details",details);
    });


}
