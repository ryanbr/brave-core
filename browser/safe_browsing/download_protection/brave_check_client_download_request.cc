/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/browser/safe_browsing/download_protection/brave_check_client_download_request.h"

namespace safe_browsing {

BraveCheckClientDownloadRequest::BraveCheckClientDownloadRequest(
    download::DownloadItem* item,
    const CheckDownloadCallback& callback,
    DownloadProtectionService* service,
    const scoped_refptr<SafeBrowsingDatabaseManager>& database_manager,
    BinaryFeatureExtractor* binary_feature_extractor)
    : CheckClientDownloadRequest(item, callback, service, database_manager,
                                 binary_feature_extractor) {
}

void BraveCheckClientDownloadRequest::SendRequest() {
  // Don't connect to the download protection remote lookup server
  // (brave/brave-browser#4341).
  FinishRequest(DownloadCheckResult::UNKNOWN, REASON_PING_DISABLED);
  return;
}

}  // namespace safe_browsing
