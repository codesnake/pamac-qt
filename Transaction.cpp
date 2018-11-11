#include "Transaction.h"
PamacQt::Transaction::Transaction(Database *db, QObject *parent):QObject(parent),
    transaction(std::shared_ptr<PamacTransaction>(pamac_transaction_new(*db),g_object_unref)),
    m_database(db)
{
    init();
}
void PamacQt::Transaction::init(){
    g_signal_connect(transaction.get(),"get_authorization_finished",
                     reinterpret_cast<GCallback>(+[](GObject* obj,bool authorized,Transaction* t){
        emit t->getAuthorizationFinished(authorized);
    }),this);

}
