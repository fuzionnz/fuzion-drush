api = 2
core = 7.x

; Fetch stock CiviCRM from download.civicrm.org
projects[civicrm][type] = module
projects[civicrm][download][type] = get
projects[civicrm][download][url] = https://download.civicrm.org/civicrm-4.7.22-drupal.tar.gz

;; Apply patches we want added.
; Merged in 4.7.23
projects[civicrm][patch][CRM-20812] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10606.patch
; Open PRs
projects[civicrm][patch][CRM-20636] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10412.patch
projects[civicrm][patch][CRM-20774] = https://github.com/civicrm/civicrm-core/commit/f4f835ed225bb18bc78f4e32006733b028938240.patch
projects[civicrm][patch][CRM-20533] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10414.patch
projects[civicrm][patch][CRM-20719] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10496.patch
projects[civicrm][patch][CRM-20728] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10501.patch

;; @TODO How to apply patches to {packages,drupal} better?
; For now, we'll manually modify the PR patches
; https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-drupal/pull/454.patch
; https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-drupal/pull/458.patch
