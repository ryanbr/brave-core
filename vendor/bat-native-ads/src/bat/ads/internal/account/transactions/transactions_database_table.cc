/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/account/transactions/transactions_database_table.h"

#include <functional>
#include <utility>
#include <vector>

#include "base/check.h"
#include "base/strings/stringprintf.h"
#include "base/time/time.h"
#include "bat/ads/internal/ads_client_helper.h"
#include "bat/ads/internal/base/database/database_bind_util.h"
#include "bat/ads/internal/base/database/database_column_util.h"
#include "bat/ads/internal/base/database/database_table_util.h"
#include "bat/ads/internal/base/database/database_transaction_util.h"
#include "bat/ads/internal/base/logging_util.h"
#include "bat/ads/internal/base/time/time_formatting_util.h"
#include "bat/ads/internal/legacy_migration/rewards/legacy_rewards_migration_transaction_constants.h"

namespace ads {
namespace database {
namespace table {

namespace {

constexpr char kTableName[] = "transactions";

int BindParameters(mojom::DBCommandInfo* command,
                   const TransactionList& transactions) {
  DCHECK(command);

  int count = 0;

  int index = 0;
  for (const auto& transaction : transactions) {
    BindString(command, index++, transaction.id);
    BindDouble(command, index++, transaction.created_at.ToDoubleT());
    BindString(command, index++, transaction.creative_instance_id);
    BindDouble(command, index++, transaction.value);
    BindString(command, index++, transaction.ad_type.ToString());
    BindString(command, index++, transaction.confirmation_type.ToString());
    BindDouble(command, index++, transaction.reconciled_at.ToDoubleT());

    count++;
  }

  return count;
}

TransactionInfo GetFromRecord(mojom::DBRecordInfo* record) {
  DCHECK(record);

  TransactionInfo transaction;

  transaction.id = ColumnString(record, 0);
  transaction.created_at = base::Time::FromDoubleT(ColumnDouble(record, 1));
  transaction.creative_instance_id = ColumnString(record, 2);
  transaction.value = ColumnDouble(record, 3);
  transaction.ad_type = AdType(ColumnString(record, 4));
  transaction.confirmation_type = ConfirmationType(ColumnString(record, 5));
  transaction.reconciled_at = base::Time::FromDoubleT(ColumnDouble(record, 6));

  return transaction;
}

}  // namespace

Transactions::Transactions() = default;

Transactions::~Transactions() = default;

void Transactions::Save(const TransactionList& transactions,
                        ResultCallback callback) {
  if (transactions.empty()) {
    callback(/* success */ true);
    return;
  }

  mojom::DBTransactionInfoPtr transaction = mojom::DBTransactionInfo::New();
  InsertOrUpdate(transaction.get(), transactions);

  AdsClientHelper::GetInstance()->RunDBTransaction(
      std::move(transaction),
      std::bind(&OnResultCallback, std::placeholders::_1, callback));
}

void Transactions::GetAll(GetTransactionsCallback callback) {
  const std::string query = base::StringPrintf(
      "SELECT "
      "id, "
      "created_at, "
      "creative_instance_id, "
      "value, "
      "ad_type, "
      "confirmation_type, "
      "reconciled_at "
      "FROM %s",
      GetTableName().c_str());

  mojom::DBCommandInfoPtr command = mojom::DBCommandInfo::New();
  command->type = mojom::DBCommandInfo::Type::READ;
  command->command = query;

  command->record_bindings = {
      mojom::DBCommandInfo::RecordBindingType::STRING_TYPE,  // id
      mojom::DBCommandInfo::RecordBindingType::DOUBLE_TYPE,  // created_at
      mojom::DBCommandInfo::RecordBindingType::
          STRING_TYPE,  // creative_instance_id
      mojom::DBCommandInfo::RecordBindingType::DOUBLE_TYPE,  // value
      mojom::DBCommandInfo::RecordBindingType::STRING_TYPE,  // ad_type
      mojom::DBCommandInfo::RecordBindingType::
          STRING_TYPE,                                      // confirmation_type
      mojom::DBCommandInfo::RecordBindingType::DOUBLE_TYPE  // reconciled_at
  };

  mojom::DBTransactionInfoPtr transaction = mojom::DBTransactionInfo::New();
  transaction->commands.push_back(std::move(command));

  AdsClientHelper::GetInstance()->RunDBTransaction(
      std::move(transaction), std::bind(&Transactions::OnGetTransactions, this,
                                        std::placeholders::_1, callback));
}

void Transactions::GetForDateRange(const base::Time from_time,
                                   const base::Time to_time,
                                   GetTransactionsCallback callback) {
  const std::string query = base::StringPrintf(
      "SELECT "
      "id, "
      "created_at, "
      "creative_instance_id, "
      "value, "
      "ad_type, "
      "confirmation_type, "
      "reconciled_at "
      "FROM %s "
      "WHERE created_at BETWEEN %f and %f ",
      GetTableName().c_str(), from_time.ToDoubleT(), to_time.ToDoubleT());

  mojom::DBCommandInfoPtr command = mojom::DBCommandInfo::New();
  command->type = mojom::DBCommandInfo::Type::READ;
  command->command = query;

  command->record_bindings = {
      mojom::DBCommandInfo::RecordBindingType::STRING_TYPE,  // id
      mojom::DBCommandInfo::RecordBindingType::DOUBLE_TYPE,  // created_at
      mojom::DBCommandInfo::RecordBindingType::
          STRING_TYPE,  // creative_instance_id
      mojom::DBCommandInfo::RecordBindingType::DOUBLE_TYPE,  // value
      mojom::DBCommandInfo::RecordBindingType::STRING_TYPE,  // ad_type
      mojom::DBCommandInfo::RecordBindingType::
          STRING_TYPE,                                      // confirmation_type
      mojom::DBCommandInfo::RecordBindingType::DOUBLE_TYPE  // reconciled_at
  };

  mojom::DBTransactionInfoPtr transaction = mojom::DBTransactionInfo::New();
  transaction->commands.push_back(std::move(command));

  AdsClientHelper::GetInstance()->RunDBTransaction(
      std::move(transaction), std::bind(&Transactions::OnGetTransactions, this,
                                        std::placeholders::_1, callback));
}

void Transactions::Update(
    const privacy::UnblindedPaymentTokenList& unblinded_payment_tokens,
    ResultCallback callback) {
  std::vector<std::string> transaction_ids;
  for (const auto& unblinded_payment_token : unblinded_payment_tokens) {
    transaction_ids.push_back(unblinded_payment_token.transaction_id);
  }
  transaction_ids.push_back(rewards::kMigrationUnreconciledTransactionId);

  const std::string query = base::StringPrintf(
      "UPDATE %s "
      "SET reconciled_at = %s "
      "WHERE reconciled_at == 0 "
      "AND (id IN %s OR creative_instance_id IN %s)",
      GetTableName().c_str(), TimeAsTimestampString(base::Time::Now()).c_str(),
      BuildBindingParameterPlaceholder(transaction_ids.size()).c_str(),
      BuildBindingParameterPlaceholder(1).c_str());

  mojom::DBCommandInfoPtr command = mojom::DBCommandInfo::New();
  command->type = mojom::DBCommandInfo::Type::READ;
  command->command = query;

  int index = 0;
  for (const auto& transaction_id : transaction_ids) {
    BindString(command.get(), index, transaction_id);
    index++;
  }

  BindString(command.get(), index,
             rewards::kMigrationUnreconciledTransactionId);
  index++;

  mojom::DBTransactionInfoPtr transaction = mojom::DBTransactionInfo::New();
  transaction->commands.push_back(std::move(command));

  AdsClientHelper::GetInstance()->RunDBTransaction(
      std::move(transaction),
      std::bind(&OnResultCallback, std::placeholders::_1, callback));
}

void Transactions::Delete(ResultCallback callback) {
  mojom::DBTransactionInfoPtr transaction = mojom::DBTransactionInfo::New();

  DeleteTable(transaction.get(), GetTableName());

  AdsClientHelper::GetInstance()->RunDBTransaction(
      std::move(transaction),
      std::bind(&OnResultCallback, std::placeholders::_1, callback));
}

std::string Transactions::GetTableName() const {
  return kTableName;
}

void Transactions::Migrate(mojom::DBTransactionInfo* transaction,
                           const int to_version) {
  DCHECK(transaction);

  switch (to_version) {
    case 18: {
      MigrateToV18(transaction);
      break;
    }

    default: {
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

void Transactions::InsertOrUpdate(mojom::DBTransactionInfo* transaction,
                                  const TransactionList& transactions) {
  DCHECK(transaction);

  if (transactions.empty()) {
    return;
  }

  mojom::DBCommandInfoPtr command = mojom::DBCommandInfo::New();
  command->type = mojom::DBCommandInfo::Type::RUN;
  command->command = BuildInsertOrUpdateQuery(command.get(), transactions);

  transaction->commands.push_back(std::move(command));
}

std::string Transactions::BuildInsertOrUpdateQuery(
    mojom::DBCommandInfo* command,
    const TransactionList& transactions) {
  DCHECK(command);

  const int count = BindParameters(command, transactions);

  return base::StringPrintf(
      "INSERT OR REPLACE INTO %s "
      "(id, "
      "created_at, "
      "creative_instance_id, "
      "value, "
      "ad_type, "
      "confirmation_type, "
      "reconciled_at) VALUES %s",
      GetTableName().c_str(),
      BuildBindingParameterPlaceholders(7, count).c_str());
}

void Transactions::OnGetTransactions(mojom::DBCommandResponseInfoPtr response,
                                     GetTransactionsCallback callback) {
  if (!response || response->status !=
                       mojom::DBCommandResponseInfo::StatusType::RESPONSE_OK) {
    BLOG(0, "Failed to get transactions");
    callback(/* success */ false, {});
    return;
  }

  TransactionList transactions;

  for (const auto& record : response->result->get_records()) {
    TransactionInfo info = GetFromRecord(record.get());
    transactions.push_back(info);
  }

  callback(/* success */ true, transactions);
}

void Transactions::MigrateToV18(mojom::DBTransactionInfo* transaction) {
  DCHECK(transaction);

  const std::string query = base::StringPrintf(
      "CREATE TABLE transactions "
      "(id TEXT NOT NULL PRIMARY KEY UNIQUE ON CONFLICT REPLACE, "
      "created_at TIMESTAMP NOT NULL, "
      "creative_instance_id TEXT, "
      "value DOUBLE NOT NULL, "
      "ad_type TEXT NOT NULL, "
      "confirmation_type TEXT NOT NULL, "
      "reconciled_at TIMESTAMP)");

  mojom::DBCommandInfoPtr command = mojom::DBCommandInfo::New();
  command->type = mojom::DBCommandInfo::Type::EXECUTE;
  command->command = query;

  transaction->commands.push_back(std::move(command));

  CreateTableIndex(transaction, "transactions", "id");
}

}  // namespace table
}  // namespace database
}  // namespace ads
