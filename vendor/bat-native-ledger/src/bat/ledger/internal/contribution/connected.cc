/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/contribution/connected.h"
#include "bat/ledger/internal/ledger_impl.h"

namespace braveledger_contribution {

Connected::Connected(bat_ledger::LedgerImpl* ledger,
    Contribution* contribution) :
    ledger_(ledger),
    contribution_(contribution) {
}

Connected::~Connected() {
}

void Connected::Start(const std::string &viewing_id) {
  const auto reconcile = ledger_->GetReconcileById(viewing_id);

  // TODO check if token is valid


}

}  // namespace braveledger_contribution
