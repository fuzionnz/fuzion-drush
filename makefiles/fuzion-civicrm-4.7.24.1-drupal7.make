api = 2
core = 7.x

; Fetch stock CiviCRM from download.civicrm.org
projects[civicrm][type] = module
projects[civicrm][download][type] = get
projects[civicrm][download][url] = https://download.civicrm.org/latest/civicrm-RC-drupal.tar.gz

;; Report a different version to Drupal.
;; - Edit / clone https://gist.github.com/xurizaemon/9eb85b7d5d451706cc9fa5a6ee024bf1
;; - Change version update & filename.
;; - Copy "Raw" URL when saved, paste below.
projects[civicrm][patch][version] = https://gist.githubusercontent.com/xurizaemon/9eb85b7d5d451706cc9fa5a6ee024bf1/raw/055897a999f26b411ad6e59e5b00ea141e8545ff/4.7.24-to-4.7.24.1-fuzion.patch

;; Apply patches we want added.
; Open PRs
;; Multiple patches mentioned in https://gist.github.com/jitendrapurohit/5fc7f612188f1e7a22b38c73d9c32e64
projects[civicrm][patch][multiple] = https://github.com/fuzionnz/civicrm-core/commit/a7ce3096491238e21a6146ca79ee67f6f50a77fd.patch

;; @TODO How to apply patches to {packages,drupal} better?
