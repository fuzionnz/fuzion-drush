;
api = 2
core = 7.x

projects[civicrm][type] = module
projects[civicrm][download][type] = get
projects[civicrm][download][url] = https://download.civicrm.org/civicrm-4.7.22-drupal.tar.gz

; Merged in 4.7.23
projects[civicrm][patch][CRM-20812] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10606.patch

; Open PRs
projects[civicrm][patch][CRM-20636] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10412.patch
projects[civicrm][patch][CRM-20719] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10496.patch
projects[civicrm][patch][CRM-20728] = https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-core/pull/10501.patch

; Drupal patches are harder?
; For now, we'll manually modify the PR patches
; https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-drupal/pull/454.patch
; https://patch-diff.githubusercontent.com/raw/civicrm/civicrm-drupal/pull/458.patch

; Mark this version as Fuzion patched. Needs updating each time we make a new revision.
https://gist.githubusercontent.com/xurizaemon/47e5c080569bd8d6c323a4b6436d8c41/raw/a978293fb581a3993152fbe8d95293cdcfba2d7b/4.7.22.1-fuzion.patch