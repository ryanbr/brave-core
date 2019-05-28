/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_SAFE_BROWSING_DOWNLOAD_PROTECTION_CHECK_CLIENT_DOWNLOAD_REQUEST_H_
#define BRAVE_BROWSER_SAFE_BROWSING_DOWNLOAD_PROTECTION_CHECK_CLIENT_DOWNLOAD_REQUEST_H_

#include "chrome/browser/safe_browsing/download_protection/check_client_download_request.h"

namespace safe_browsing {

class BraveCheckClientDownloadRequest : public CheckClientDownloadRequest {
public:
  BraveCheckClientDownloadRequest(
      download::DownloadItem* item,
      const CheckDownloadCallback& callback,
      DownloadProtectionService* service,
      const scoped_refptr<SafeBrowsingDatabaseManager>& database_manager,
      BinaryFeatureExtractor* binary_feature_extractor);

protected:
  void SendRequest() override;
};

}  // namespace safe_browsing

#endif  // BRAVE_BROWSER_SAFE_BROWSING_DOWNLOAD_PROTECTION_CHECK_CLIENT_DOWNLOAD_REQUEST_H_
