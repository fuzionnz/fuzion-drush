api = 2
core = 7.x

; Fetch stock CiviCRM from download.civicrm.org
projects[civicrm][type] = module
projects[civicrm][download][type] = get
projects[civicrm][download][url] = https://download.civicrm.org/civicrm-4.7.24-drupal.tar.gz

;; Report a different version to Drupal.
;; - Edit / clone https://gist.github.com/xurizaemon/9eb85b7d5d451706cc9fa5a6ee024bf1
;; - Change version update & filename.
;; - Copy "Raw" URL when saved, paste below.
projects[civicrm][patch][version] = https://gist.githubusercontent.com/xurizaemon/9eb85b7d5d451706cc9fa5a6ee024bf1/raw/055897a999f26b411ad6e59e5b00ea141e8545ff/4.7.24-to-4.7.24.1-fuzion.patch

;; Apply patches we want added.
; Open PRs
projects[civicrm][patch][CRM-20636] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10412.patch
projects[civicrm][patch][CRM-20533] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10908.patch
projects[civicrm][patch][CRM-20719] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10496.patch
projects[civicrm][patch][CRM-20728] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10501.patch
; Jitu - this is Fuzion patches, should be in fuzionnz/civicrm-core not civicrm/civicrm-core?
projects[civicrm][patch][multiple] = https://github.com/civicrm/civicrm-core/commit/e3832b5baea2c33fd9cf7280f1327866c3f4619e.patch

;; @TODO How to apply patches to {packages,drupal} better?
