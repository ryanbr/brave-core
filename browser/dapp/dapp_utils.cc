/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/dapp/dapp_utils.h"

#include "brave/browser/dapp/wallet_installation_permission_request.h"
#include "brave/common/pref_names.h"
#include "chrome/browser/permissions/permission_request_manager.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"

bool DappDetectionEnabled(content::BrowserContext* browser_context) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  return profile->GetPrefs()->GetBoolean(kDappDetectionEnabled);
}

void RequestWallInstallationPermission(content::WebContents* web_contents) {
  DCHECK(web_contents);
  PermissionRequestManager::FromWebContents(web_contents)->AddRequest(
      new WalletInstallationPermissionRequest(web_contents));
}
