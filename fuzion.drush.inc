<?php

/**
 * @file
 * Provide Drush commands for Fuzion.
 */

/**
 * Validate whether pm-download is permitted.
 */
function drush_fuzion_pm_download_pre_validate() {
//  return _fuzion_validate_clean_git();
}

/**
 * Validate whether pm-enable is permitted.
 */
function drush_fuzion_pm_enable_pre_validate() {
  // return _fuzion_validate_clean_git();
}

/**
 * Check if a site's codebase is clean.
 *
 * @see https://redmine.fuzion.co.nz/issues/7205
 */
function _fuzion_validate_clean_git() {
  $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  drush_log('Checking git status before modifying codebase.', 'ok');
  $result = drush_shell_cd_and_exec($path, 'git status --porcelain');
  if ($output = drush_shell_exec_output()) {
    $error_message = '`git status` does not look clean. Please commit changes before modifying code further.';
    drush_log($error_message, 'warning');
    drush_log(implode("\n", $output), 'warning');
  }
}