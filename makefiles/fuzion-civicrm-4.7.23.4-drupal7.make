api = 2
core = 7.x

; Fetch stock CiviCRM from download.civicrm.org
projects[civicrm][type] = module
projects[civicrm][download][type] = get
projects[civicrm][download][url] = https://download.civicrm.org/civicrm-4.7.23-drupal.tar.gz

;; Report a different version to Drupal
projects[civicrm][patch][version] = https://gist.githubusercontent.com/xurizaemon/9eb85b7d5d451706cc9fa5a6ee024bf1/raw/aafc38217fdb1f1c84ec9fc591ca02261198c039/4.7.23-to-4.7.23.3-fuzion.patch

;; Apply patches we want added.
; Open PRs
projects[civicrm][patch][CRM-20636] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10412.patch
projects[civicrm][patch][CRM-20533] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10414.patch
projects[civicrm][patch][CRM-20719] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10496.patch
projects[civicrm][patch][CRM-20728] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10501.patch
projects[civicrm][patch][multiple] = https://github.com/civicrm/civicrm-core/commit/e3832b5baea2c33fd9cf7280f1327866c3f4619e.patch

;; @TODO How to apply patches to {packages,drupal} better?

