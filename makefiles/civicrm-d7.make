; We haven't used this in a while, you shouldn't either :)
api = 2
core = 7.x

projects[civicrm][type] = module
projects[civicrm][download][type] = git
projects[civicrm][download][url] = git@github.com:fuzionnz/civicrm-core.git
; We should use proper PHP/Drupal versions, eg 4.4.11-rc1
projects[civicrm][download][revision] = 4.4.11rc1

projects[civicrm-drupal][type] = module
projects[civicrm-drupal][download][type] = git
projects[civicrm-drupal][download][url] = git@github.com:fuzionnz/civicrm-drupal.git
projects[civicrm-drupal][download][revision] =
projects[civicrm-drupal][directory_name] = civicrm/drupal
projects[civicrm-drupal][overwrite] = true
