#include "Transaction.h"
#include "Utils.h"
#include "QQuickDialog.h"
#include <QQmlComponent>
#include <QtDBus/QtDBus>

void send_unity_launcherentry_message(const QVariantMap& message){
    QDBusMessage signal = QDBusMessage::createSignal("/",
                                                     "com.canonical.Unity.LauncherEntry",
                                                     "Update");

    signal << "application://pamac-qt.desktop";

    signal << message;

    QDBusConnection::sessionBus().send(signal);
}

PamacQt::Transaction::Transaction(Database *db, QObject *parent):QObject(parent),
    transaction(std::shared_ptr<PamacTransaction>(pamac_transaction_new(*db),g_object_unref)),
    m_database(db)
{
    init();
}

void PamacQt::Transaction::startWritePamacConfig(const QVariantMap &map){
    GHashTable* tabl = g_hash_table_new(g_direct_hash,
                                        g_direct_equal);

    for(QVariantMap::const_iterator it = map.constBegin(), total = map.constEnd();it!=total;++it){

        size_t length = ulong(it.key().toUtf8().length());
        char* var = new char[length+1];
        std::memcpy(var,it.key().toUtf8(),length);
        var[length]='\0';


        g_hash_table_insert(tabl,var,Utils::qVariantToGVariant(it.value()));
    }

    pamac_transaction_start_write_pamac_config(transaction.get(),tabl);
}

void PamacQt::Transaction::start(const QStringList& toInstall, const QStringList& toRemove, const QStringList& toLoad,
                                 const QStringList& toBuild, const QStringList& tempIgnore, const QStringList& overwriteFiles)
{
    using Utils::qStringListToCStringVector;

    auto install = qStringListToCStringVector(toInstall);
    auto remove = qStringListToCStringVector(toRemove);
    auto load = qStringListToCStringVector(toLoad);
    auto build = qStringListToCStringVector(toBuild);
    auto ignore = qStringListToCStringVector(tempIgnore);
    auto overwrite = qStringListToCStringVector(overwriteFiles);

    pamac_transaction_start(transaction.get(),install.data(),int(install.size()),
                            remove.data(),int(remove.size()),
                            load.data(),int(load.size()),
                            build.data(),int(build.size()),
                            ignore.data(),int(ignore.size()),
                            overwrite.data(),int(overwrite.size()));
    setProperty("started",true);

}

void PamacQt::Transaction::startSysupgrade(bool forceRefresh, bool enableDowngrade, const QStringList &tempIgnore, const QStringList &overwriteFiles)
{
    using Utils::qStringListToCStringVector;

    auto ignore = qStringListToCStringVector(tempIgnore);
    auto overwrite = qStringListToCStringVector(overwriteFiles);

    pamac_transaction_start_sysupgrade(transaction.get(),forceRefresh,enableDowngrade,
                                       ignore.data(),int(ignore.size()),
                                       overwrite.data(),int(overwrite.size()));
    setProperty("started",true);
}
void PamacQt::Transaction::init()
{
    //methods override
    PAMAC_TRANSACTION_GET_CLASS(transaction.get())->ask_commit=
            [](PamacTransaction* self, PamacTransactionSummary* summary)->gboolean{

        QQuickDialog dlg(QUrl("qrc:/src/qml/TransactionSummaryDialog.qml"));
        dlg.setWindowTitle("Transaction Summary");

        dlg.setQQuickWidgetProperty("summary",QVariant::fromValue(TransactionSummary(summary)));

        return dlg.exec();


    };
    PAMAC_TRANSACTION_GET_CLASS(transaction.get())->choose_optdeps=
            [](PamacTransaction* self, const gchar* pkgname, gchar** optdeps, int optdeps_length1)->GList*{
        QQuickDialog dlg(QUrl("qrc:/src/qml/TransactionOptDependsDialog.qml"));
        dlg.setWindowTitle("Choose optional dependecies for"+QString::fromUtf8(pkgname));

        QStringList lst = Utils::cStringArrayToQStringList(optdeps,optdeps_length1);

        dlg.setQQuickWidgetProperty("pkgName",QString::fromUtf8(pkgname));
        dlg.setQQuickWidgetProperty("opDeps",lst);

        dlg.exec();

        QVariant var = dlg.qQuickWidgetProperty("opted");
        GList* result = nullptr;

        if(var.canConvert(QMetaType::QVariantList)){
            QVariantList varList = var.toList();

            int minLength = std::min(optdeps_length1,varList.length());

            for(int i =0;i<minLength;i++){
                if(varList[i].toBool()){
                    char* res = new char[lst[i].toUtf8().length()];
                    std::memcpy(res,lst[i].toUtf8(),lst[i].toUtf8().size());
                    result = g_list_prepend(result,res);
                }
            }
            g_list_foreach(result,[](void* data,void*){
                std::cout<<((char*)data)<<std::endl;
            },nullptr);

        }
        return result;
    };
    PAMAC_TRANSACTION_GET_CLASS(transaction.get())->choose_provider=
            [](PamacTransaction* self, const gchar* depend, gchar** providers, int providers_length1)->gint{
        std::cout<<(depend)<<std::endl;
        return 0;
    };


    g_signal_connect(transaction.get(),"get_authorization_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool authorized,Transaction* t){
                         Q_UNUSED(obj);
                         emit t->getAuthorizationFinished(authorized);
                     }),this);


    g_signal_connect(transaction.get(),"finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool success,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Finished with result: "<<success<<std::endl;
                         emit t->finished(success);
                     }),this);

    g_signal_connect(transaction.get(),"emit_action",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* action,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Action: "<<action<<std::endl;
                         emit t->emitAction(QString::fromUtf8(action));
                     }),this);

    g_signal_connect(transaction.get(),"emit_action_progress",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* action,char* status,double progress,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Progress: "<<action<<status<<progress<<std::endl;

                         emit t->emitActionProgress(QString::fromUtf8(action),QString::fromUtf8(status),progress);
                     }),this);

    g_signal_connect(transaction.get(),"emit_error",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,char** details,int size,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Error: "<<message<<std::endl<<"Details:"<<std::endl;
                         for(int i =0;i<size;i++){
                             std::cout<<"- "<<details[i]<<std::endl;
                         }
                         emit t->emitError(QString::fromUtf8(message),
                         Utils::cStringArrayToQStringList(details,size));
                     }),this);
    g_signal_connect(transaction.get(),"emit_warning",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* warning,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Warning: "<<warning<<std::endl;
                         emit t->emitWarning(QString::fromUtf8(warning));
                     }),this);
    g_signal_connect(transaction.get(),"start_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Started preparing"<<std::endl;
                         emit t->startPreparing();
                     }),this);
    g_signal_connect(transaction.get(),"stop_preparing",
                     reinterpret_cast<GCallback>(+[](GObject* obj,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Stopped preparing"<<std::endl;
                         emit t->stopPreparing();
                     }),this);

    g_signal_connect(transaction.get(),"emit_script_output",
                     reinterpret_cast<GCallback>(+[](GObject* obj,char* message,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Script: "<<message<<std::endl;
                         emit t->emitScriptOutput(QString::fromUtf8(message));
                     }),this);

    g_signal_connect(transaction.get(),"important_details_outpout",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool message,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"Important details: "<<message<<std::endl;
                         emit t->importantDetailsOutput(message);
                     }),this);
    g_signal_connect(transaction.get(),"sysupgrade_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool success,Transaction* t){
                         Q_UNUSED(obj);
                         std::cout<<"System upgrade finished with rusult: "<<success<<std::endl;
                         emit t->finished(success);
                     }),this);


    connect(this,&Transaction::startPreparing,[=](){this->setProperty("indeterminate",true);});
    connect(this,&Transaction::stopPreparing,[=](){this->setProperty("indeterminate",false);});

    connect(this,&Transaction::emitActionProgress,[=](const QString& action,const QString& status,double progress){
        this->setProperty("action",action);
        this->setProperty("progress",progress);

        QString details = this->property("details").toString();
        details.append(action+" "+status+"\n");
        this->setProperty("details",details);

    });
    connect(this,&Transaction::emitAction,[=](const QString& action){
        this->setProperty("action",action);
        QString details = this->property("details").toString();
        details.append(action+"\n");
        this->setProperty("details",details);
    });
    connect(this,&Transaction::finished,[=](bool success){
        Q_UNUSED(success);
        this->setProperty("progress",0);
        this->setProperty("started",false);
    });
    connect(this,&Transaction::emitScriptOutput,[=](QString message){
        QString details = this->property("details").toString();
        details.append(message+"\n");
        this->setProperty("details",details);
    });

    connect(this,&Transaction::startedChanged,[=](bool started){
        send_unity_launcherentry_message({{"progress-visible",started},
                                          {"progress",0}});
    });

    connect(this,&Transaction::progressChanged,[=](double progress){
        send_unity_launcherentry_message({{"progress",progress}});
    });
}
